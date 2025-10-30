#include "measureperformance.hpp"
#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "operationstatistics.hpp"
#include "operationtype.hpp"
#include "settings.hpp"

#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <nlohmann/json.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using json = nlohmann::json;

void measurePerformance(const Settings &settings) {
  json overallJson;

  for (unsigned iteration = 0; iteration < settings.iterationCount;
       iteration++) {
    for (const std::string &graphName : settings.graphNames) {
      json graphJson;
      for (const auto& [algorithmType, algorithmSettings] : settings.algorithmInfos) {

        PerformanceMeasurer performanceMeasurer{
            graphName, algorithmType, *algorithmSettings,
            settings.iterationCount, overallJson[graphName]};
        performanceMeasurer.execute();
      }
    }
  }

  std::string output = overallJson.dump(2);

  if (settings.outputPath.empty()) {
    std::cout << output;
  } else {
    std::ofstream ofs{settings.outputPath};
    ofs << output;
  }
}

namespace {

bool applyNextOperationAndMeasure(
    DynamicDiGraph &dynamicGraph,
    std::map<OperationType, OperationStatistics> &operationDurations) {
  const auto timeBeforeOperation = std::chrono::high_resolution_clock::now();
  const bool operationsLeft = dynamicGraph.applyNextOperation();
  const auto timeAfterOperation = std::chrono::high_resolution_clock::now();

  const std::chrono::nanoseconds duration =
      timeAfterOperation - timeBeforeOperation;
  const auto lastOperationType = getLastOperationType(dynamicGraph);
  operationDurations[lastOperationType].addOccurnece(duration);

  return operationsLeft;
}
} // namespace

PerformanceMeasurer::PerformanceMeasurer(const std::string &graphName,
                                         AlgorithmType algorithmType,
                                         const AlgorithmSettings& algorithmSettings,
                                         unsigned iterationCount,
                                         json &outerJson)
    : AlgorithmExecuter(graphName, algorithmType, algorithmSettings),
      iterationCount(iterationCount), outerJson(outerJson),
      algorithmType(algorithmType) {}

void PerformanceMeasurer::execute() {
  std::map<OperationType, OperationStatistics> operationDurations;

  DynamicDiGraph::DynamicTime maxTime = dynamicGraph.getMaxTime();
  size_t nOperations = dynamicGraph.countArcAdditions(0, maxTime) +
                       dynamicGraph.countArcRemovals(0, maxTime);
  constexpr static double progressPercentage = .01;
  const size_t partOperations = static_cast<size_t>(
      static_cast<double>(nOperations) * progressPercentage);

  json current_output;

  while (graph->getSize() < 1) {
    applyNextOperationAndMeasure(dynamicGraph, operationDurations);
  }

  auto source = graph->vertexAt(0);
  pAlgorithm->setSource(source);
  pAlgorithm->run();

  // execute all updates on the graph and measure the update durations
  while (applyNextOperationAndMeasure(dynamicGraph, operationDurations)) {
    if (operationIndex % partOperations == 0 && operationIndex > 0) {
      std::cout << static_cast<double>(operationIndex) /
                       static_cast<double>(nOperations) * 100.0
                << " percent of operations executed" << std::endl;
    }
    operationIndex++;
  }

  for (const auto &[operationType, statistics] : operationDurations) {
    std::cout << "average duration of operation \""
              << operationTypeNames.at(operationType)
              << "\": " << statistics.getAverageDuration().count() << "ns"
              << std::endl;

    current_output[operationTypeNames.at(operationType)] =
        statistics.getAverageDuration().count();
  }

  dynamicGraph.resetToBigBang();
  for (auto &[operationType, statistics] : operationDurations) {
    statistics.reset();
  }

  const std::string algorithmTypeName = AlgorithmTypeNames.at(algorithmType);

  outerJson[algorithmTypeName].push_back(current_output);

  cleanup();
}