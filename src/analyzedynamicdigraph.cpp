#include "analyzedynamicdigraph.hpp"
#include "operationtype.hpp"
#include <cstdlib>
#include <graph/arc.h>
#include <graph.dyn/dynamicdigraph.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <stdexcept>
#include <unordered_map>


using namespace Algora;

json DynamicDiGraphAnalyzer::analyze(DynamicDiGraph &dynamicDiGraph) {

  double densitySum = 0;

  IncidenceListGraph *diGraph = dynamicDiGraph.getDiGraph();

  const DynamicDiGraph::DynamicTime maxTime = dynamicDiGraph.getMaxTime();
  const DynamicDiGraph::size_type nArcAdditions =
      dynamicDiGraph.countArcAdditions(0, maxTime);
  const DynamicDiGraph::size_type nArcRemovals =
      dynamicDiGraph.countArcRemovals(0, maxTime);
  const DynamicDiGraph::size_type nOperations = nArcAdditions + nArcRemovals;

  size_t operationIndex = 0;

  size_t nArcs = 0;

  std::unordered_map<Arc *, size_t>
      arcBriths; // operation indices at which the arcs were inserted
  size_t arcLifeTimeSum = 0;

  int arcObserverId = 0;
  diGraph->onArcAdd(&arcObserverId,
                    [&arcBriths, &operationIndex](Arc *pAddedArc) {
                      if (arcBriths.count(pAddedArc)) {
                        std::cout << "multi arc" << std::endl;
                        std::exit(EXIT_FAILURE);
                      }
                      arcBriths[pAddedArc] = operationIndex;
                    });
  diGraph->onArcRemove(&arcObserverId, [&arcBriths, &operationIndex,
                                        &arcLifeTimeSum](Arc *pRemovedArc) {
                                          if (pRemovedArc == nullptr)
                                          {
                                            throw std::runtime_error("removed arc is null");
                                          }
                                          if (arcBriths.count(pRemovedArc) == 0) {
                                            std::cout << "Arc (" << pRemovedArc->getTail() << ", " << pRemovedArc->getHead() << ") is not contained in arcBirths map!" << std::endl;
                                            std::exit(EXIT_FAILURE);
                                          }
    arcLifeTimeSum += operationIndex - arcBriths[pRemovedArc];
    arcBriths.erase(pRemovedArc);
  });

  while (dynamicDiGraph.applyNextOperation()) {

    const OperationType currentOperationType =
        getLastOperationType(dynamicDiGraph);
    if (currentOperationType == OperationType::arcAddition) {
      nArcs++;
    } else if (currentOperationType == OperationType::arcRemoval) {
      nArcs--;
    }

    operationIndex++;
    const double currentDensity = (double)nArcs / (double)diGraph->getSize();
    densitySum += currentDensity;
    if ((operationIndex * 100) % nOperations == 0) {
      std::cout << (operationIndex * 100) / nOperations << "% executed"
                << std::endl;
    }
  }

  diGraph->removeOnArcAdd(&arcObserverId);
  diGraph->removeOnArcRemove(&arcObserverId);

  for (const auto &[pArc, birth] : arcBriths) {
    arcLifeTimeSum += operationIndex - birth;
  }

  size_t averageArcLifeTime = arcLifeTimeSum / nArcAdditions;

  const double averageDensity = densitySum / nOperations;

  return {{"averageDensity", averageDensity},
          {"averageArcLifeTime", averageArcLifeTime}};
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