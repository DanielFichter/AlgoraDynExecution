#include "measureperformance.hpp"
#include "algorithmexecuter.hpp"
#include "algorithmtype.hpp"
#include "operationstatistics.hpp"
#include "operationtype.hpp"

#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <cstddef>
#include <iostream>
#include <map>

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
                                         unsigned iterationCount)
    : AlgorithmExecuter(graphName, algorithmType),
      iterationCount(iterationCount) {}

void PerformanceMeasurer::execute() {
  std::map<OperationType, OperationStatistics> operationDurations;

  DynamicDiGraph::DynamicTime maxTime = dynamicGraph.getMaxTime();
  size_t nOperations = dynamicGraph.countArcAdditions(0, maxTime) +
                       dynamicGraph.countArcRemovals(0, maxTime);
  constexpr static double progressPercentage = .01;
  const size_t partOperations = static_cast<size_t>(
      static_cast<double>(nOperations) * progressPercentage);

  for (unsigned iteration = 0; iteration < iterationCount; iteration++) {
    while (graph->getSize() < 1) {
      applyNextOperationAndMeasure(dynamicGraph, operationDurations);
    }

    auto source = graph->vertexAt(0);
    pAlgorithm->setSource(source);
    pAlgorithm->run();

    while (applyNextOperationAndMeasure(dynamicGraph, operationDurations)) {
      if (operationIndex % partOperations == 0 && operationIndex > 0) {
        std::cout <<  static_cast<double>(operationIndex) / static_cast<double>(nOperations) * 100.0
                  << " percent of operations executed" << std::endl;
      }
      operationIndex++;
    }

    dynamicGraph.resetToBigBang();
  }

  for (const auto &[operationType, statistics] : operationDurations) {
    std::cout << "average duration of operation \""
              << operationTypeNames.at(operationType)
              << "\": " << statistics.getAverageDuration().count() << "ns"
              << std::endl;
  }

  cleanup();
}