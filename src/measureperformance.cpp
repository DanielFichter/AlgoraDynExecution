#include "measureperformance.hpp"
#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "operationstatistics.hpp"
#include "operationtype.hpp"
#include "settings.hpp"

#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <graph/digraph.h>
#include <limits>
#include <nlohmann/json.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>

using json = nlohmann::json;

void measurePerformance(const Settings &settings) {
  json overallJson;

  for (unsigned iteration = 0; iteration < settings.iterationCount;
       iteration++) {
    for (const std::string &graphName : settings.graphNames) {
      json graphJson;
      for (const auto &[algorithmType, algorithmSettings] :
           settings.algorithmInfos) {

        PerformanceMeasurer performanceMeasurer{graphName,
                                                algorithmType,
                                                *algorithmSettings,
                                                settings.iterationCount,
                                                overallJson[graphName],
                                                settings.queryRatio};
        std::cout << "measuring performance of algorithm \""
                  << AlgorithmTypeNames.at(algorithmType) << *algorithmSettings
                  << "\" on graph \"" << graphName << "\"" << std::endl;

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

void queryAndMeasure(
    IncidenceListGraph *graph,
    std::unique_ptr<DynamicSingleSourceReachabilityAlgorithm> &pAlgorithm,
    std::map<OperationType, OperationStatistics> &operationDurations,
    std::mt19937 &randomEngine) {
  size_t vertexIndex = randomEngine() % graph->getSize();
  Vertex *vertexToQuery = graph->vertexAt(vertexIndex);
  const auto timeBeforeOperation = std::chrono::high_resolution_clock::now();
  pAlgorithm->query(vertexToQuery);
  const auto timeAfterOperation = std::chrono::high_resolution_clock::now();
  const std::chrono::nanoseconds duration =
      timeAfterOperation - timeBeforeOperation;
  operationDurations[OperationType::query].addOccurnece(duration);
}

} // namespace

PerformanceMeasurer::PerformanceMeasurer(
    const std::string &graphName, AlgorithmType algorithmType,
    const AlgorithmSettings &algorithmSettings, unsigned iterationCount,
    json &outerJson, double queryRatio)
    : AlgorithmExecuter(graphName, algorithmType, algorithmSettings),
      iterationCount(iterationCount), outerJson(outerJson),
      algorithmType(algorithmType), algorithmSettings(algorithmSettings),
      queryRatio(queryRatio) {}

void PerformanceMeasurer::execute() {
  std::map<OperationType, OperationStatistics> operationDurations;

  std::mt19937 randomEngine;

  DynamicDiGraph::DynamicTime maxTime = dynamicGraph.getMaxTime();
  const size_t nOperations = dynamicGraph.countArcAdditions(0, maxTime) +
                       dynamicGraph.countArcRemovals(0, maxTime);
  std::cout << "nOperations: " << nOperations << std::endl;
  constexpr static double progressPercentage = .01;
  const size_t partOperations = static_cast<size_t>(
      static_cast<double>(nOperations) * progressPercentage);

  const auto queryIndex = queryRatio <= 0 ? std::numeric_limits<size_t>::max()
                                          : static_cast<size_t>(1 / queryRatio);

  json current_output;

  while (graph->getSize() < 1) {
    applyNextOperationAndMeasure(dynamicGraph, operationDurations);
    incrementOperationIndex(partOperations, nOperations);
  }

  auto source = graph->vertexAt(0);
  pAlgorithm->setSource(source);
  pAlgorithm->run();

  // execute all updates on the graph and measure the update durations
  while (applyNextOperationAndMeasure(dynamicGraph, operationDurations)) {
    incrementOperationIndex(partOperations, nOperations);
    if (operationIndex % queryIndex == 0) {
      queryAndMeasure(graph, pAlgorithm, operationDurations, randomEngine);
    }
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

  const std::string algorithmName =
      AlgorithmTypeNames.at(algorithmType) + algorithmSettings.print();

  outerJson[algorithmName].push_back(current_output);

  std::cout << "operation index: " << operationIndex << std::endl;

  cleanup();
}

void PerformanceMeasurer::incrementOperationIndex(size_t partOperations,
                                                  size_t nOperations) {
  operationIndex++;
  if (partOperations > 0 && operationIndex % partOperations == 0 && operationIndex > 0) {
    std::cout << static_cast<double>(operationIndex) /
                     static_cast<double>(nOperations) * 100.0
              << "% of the graph's operations executed" << std::endl;
  }
}