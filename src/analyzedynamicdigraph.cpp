#include "analyzedynamicdigraph.hpp"
#include "operationtype.hpp"
#include <graph.dyn/dynamicdigraph.h>

using namespace Algora;

json DynamicDiGraphAnalyzer::analyze(DynamicDiGraph &graph) {

  double densitySum = 0;

  const DynamicDiGraph::DynamicTime maxTime = graph.getMaxTime();
  const DynamicDiGraph::size_type nOperations =
      graph.countArcAdditions(0, maxTime) + graph.countArcRemovals(0, maxTime);

  size_t operationIndex = 0;

  size_t nArcs = 0;

  while (graph.applyNextOperation()) {

    const OperationType currentOperationType = getLastOperationType(graph);
    if (currentOperationType == OperationType::arcAddition) {
      nArcs++;
    } else if (currentOperationType == OperationType::arcRemoval) {
      nArcs--;
    }

    operationIndex++;
    const double currentDensity = (double)nArcs / (double)graph.getDiGraph()->getSize();
    densitySum += currentDensity;
    if ((operationIndex * 100) % nOperations == 0) {
      std::cout << (operationIndex * 100) / nOperations << "% executed"
                << std::endl;
    }
  }

  const double averageDensity = densitySum / nOperations;

  return {{"averageDensity", averageDensity}};
}

void analyzeDynamicDiGraphs(const Settings &settings) {
  json result;
  for (const auto &[graphDescription, pGraphInsantiator] :
       settings.graphInfos) {
    DynamicDiGraph dynamicDiGraph = pGraphInsantiator->instantiate();
    DynamicDiGraphAnalyzer analyzer;
    result[graphDescription] = analyzer.analyze(dynamicDiGraph);
  }
  std::cout << result;
}