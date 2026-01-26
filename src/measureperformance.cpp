#include "measureperformance.hpp"
#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "operationstatistics.hpp"
#include "operationtype.hpp"
#include "settings.hpp"
#include "sourcevertices.hpp"

#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <graph/digraph.h>
#include <graph/vertex.h>
#include <limits>
#include <nlohmann/json.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

void measurePerformance(const Settings &settings) {
  json overallJson;

  for (unsigned iteration = 0; iteration < settings.iterationCount;
       iteration++) {
    std::cout << std::endl << "iteration " << iteration << std::endl;

    for (const auto &[graphDescription, pGraphInstantiator] :
         settings.graphInfos) {
      json graphJson;
      for (const auto &[algorithmType, algorithmSettings] :
           settings.algorithmInfos) {

        PerformanceMeasurer performanceMeasurer{*pGraphInstantiator,
                                                algorithmType,
                                                *algorithmSettings,
                                                settings.iterationCount,
                                                overallJson[graphDescription],
                                                settings.queryRatio};
        if (sourceVertexIds.count(graphDescription) == 1 &&
            iteration < sourceVertexIds.at(graphDescription).size()) {
          DynamicDiGraph::VertexIdentifier sourceVertexId =
              sourceVertexIds.at(graphDescription).at(iteration);
          performanceMeasurer.setSourceVertexId(sourceVertexId);
          if (graphDescription == "snap/as-caida") 
          {
            performanceMeasurer.setTimeStampZero(109);
          }
        }
        std::cout << "measuring performance of algorithm \""
                  << AlgorithmTypeNames.at(algorithmType) << *algorithmSettings
                  << "\" on graph \"" << graphDescription << "\"" << std::endl;

        try {
          performanceMeasurer.execute();
        } catch (const std::bad_alloc &badAlloc) {
          std::cerr << "bad alloc was thrown!" << std::endl;
        }
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

PerformanceMeasurer::PerformanceMeasurer(
    GraphInstantiator &graphInstantiator, AlgorithmType algorithmType,
    const AlgorithmSettings &algorithmSettings, unsigned iterationCount,
    json &outerJson, double queryRatio)
    : AlgorithmExecuter(graphInstantiator, algorithmType, algorithmSettings),
      iterationCount(iterationCount), outerJson(outerJson),
      algorithmType(algorithmType), algorithmSettings(algorithmSettings),
      queryRatio(queryRatio) {}

void PerformanceMeasurer::setSourceVertexId(
    DynamicDiGraph::VertexIdentifier newSourceVertexId) {
  sourceVertexId = newSourceVertexId;
  useSourceVertexId = true;
}

void PerformanceMeasurer::setTimeStampZero(size_t newTimeStampZero) {
  timeStampZero = newTimeStampZero;
}

void PerformanceMeasurer::execute() {
  std::map<OperationType, OperationStatistics> operationDurations;

  std::mt19937 randomEngine;

  DynamicDiGraph::DynamicTime maxTime = dynamicGraph.getMaxTime();
  const size_t nOperations = dynamicGraph.countArcAdditions(0, maxTime) +
                             dynamicGraph.countArcRemovals(0, maxTime) +
                             dynamicGraph.countVertexAdditions(0, maxTime);
  constexpr static double progressPercentage = .01;
  const size_t partOperations = static_cast<size_t>(
      static_cast<double>(nOperations) * progressPercentage);

  const auto queryIndex = queryRatio <= 0 ? std::numeric_limits<size_t>::max()
                                          : static_cast<size_t>(1 / queryRatio);

  json current_output;

  Vertex *pSourceVertex = nullptr;

  std::cout << std::endl;

  while (dynamicGraph.getCurrentTime() <= timeStampZero) {
    dynamicGraph.applyNextOperation();
    incrementOperationIndex(partOperations, nOperations);
  }


  std::cout << "graph initialized"
            << " (time stamp: " << dynamicGraph.getCurrentTime() << ")"
            << std::endl;

  if (useSourceVertexId) {
    std::string sourceVertexName = std::to_string(sourceVertexId);
    graph->mapVerticesUntil(
        [&pSourceVertex, sourceVertexName](Vertex *pVertex) {
          if (sourceVertexName == pVertex->getName()) {
            pSourceVertex = pVertex;
          }
        },
        [&pSourceVertex](const Vertex *pVertex) -> bool {
          return pSourceVertex;
        });

    if (pSourceVertex == nullptr) {
      throw std::runtime_error("vertex with id \""s + sourceVertexName +
                               "\" not found!");
    }
  } else {
    pSourceVertex = graph->vertexAt(0);
  }
  std::cout << "source vertex: " << pSourceVertex->getName() << std::endl;
  pAlgorithm->setSource(pSourceVertex);
  pAlgorithm->run();

  std::cout << std::endl;
  // execute all updates on the graph and measure the update durations
  while (applyNextOperationAndMeasure(operationDurations)) {
    incrementOperationIndex(partOperations, nOperations);
    if (operationIndex % queryIndex == 0) {
      queryAndMeasure(operationDurations, randomEngine);
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

  cleanup();
}

bool PerformanceMeasurer::applyNextOperationAndMeasure(
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

void PerformanceMeasurer::queryAndMeasure(
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

void PerformanceMeasurer::incrementOperationIndex(size_t partOperations,
                                                  size_t nOperations) {
  operationIndex++;
  if (partOperations > 0 && operationIndex % partOperations == 0 &&
      operationIndex > 0) {

    std::cout << "\x1b[1A\x1b[2K"; // clear line
    std::cout << static_cast<double>(operationIndex) /
                     static_cast<double>(nOperations) * 100.0
              << "% of the graph's operations executed" << std::endl;
  }
}