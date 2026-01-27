#include "analyzedynamicdigraph.hpp"
#include "operationtype.hpp"

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <graph.dyn/dynamicdigraph.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <graph/arc.h>
#include <graph/vertex.h>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <graph.dyn/dynamicdigraphstatistics.h>
#include <vector>

using namespace Algora;

template <>
struct std::hash<std::pair<DynamicDiGraph::VertexIdentifier,
                           DynamicDiGraph::VertexIdentifier>> {
  std::size_t
  operator()(const std::pair<DynamicDiGraph::VertexIdentifier,
                             DynamicDiGraph::VertexIdentifier> &arc) const {
    const size_t tailHash = hash<DynamicDiGraph::VertexIdentifier>{}(arc.first);
    const size_t headHash =
        hash<DynamicDiGraph::VertexIdentifier>{}(arc.second);
    return headHash ^ (tailHash << 1);
  }
};

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

  std::vector<size_t> arcLifeTimes;

  std::unordered_map<std::pair<DynamicDiGraph::VertexIdentifier,
                               DynamicDiGraph::VertexIdentifier>,
                     unsigned>
      numArcOccurences;

  int arcObserverId = 0;
  diGraph->onArcAdd(&arcObserverId, [&arcBriths, &operationIndex,
                                     &numArcOccurences](Arc *pAddedArc) {
    if (arcBriths.count(pAddedArc)) {
      std::cout << "multi arc" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    arcBriths[pAddedArc] = operationIndex;
    const auto tailId = pAddedArc->getTail()->getId();
    const auto headId = pAddedArc->getHead()->getId();
    numArcOccurences[{tailId, headId}]++;
  });
  diGraph->onArcRemove(&arcObserverId, [&arcBriths, &operationIndex,
                                        &arcLifeTimeSum,
                                        &arcLifeTimes](Arc *pRemovedArc) {
    if (pRemovedArc == nullptr) {
      throw std::runtime_error("removed arc is null");
    }
    if (arcBriths.count(pRemovedArc) == 0) {
      std::cout << "Arc (" << pRemovedArc->getTail() << ", "
                << pRemovedArc->getHead()
                << ") is not contained in arcBirths map!" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    size_t currentArcLifeTime = operationIndex - arcBriths[pRemovedArc];
    arcLifeTimeSum += currentArcLifeTime;
    arcLifeTimes.push_back(currentArcLifeTime);
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
    size_t currentLifeTime = operationIndex - birth;
    arcLifeTimeSum += currentLifeTime;
    arcLifeTimes.push_back(currentLifeTime);
  }

  size_t averageArcLifeTime = arcLifeTimeSum / nArcAdditions;

  const double averageDensity = densitySum / nOperations;

  std::vector<unsigned> arcInsertionCounts;
  arcInsertionCounts.reserve(numArcOccurences.size());
  for (const auto &[arcInfo, numOccurences] : numArcOccurences) {
    arcInsertionCounts.push_back(numOccurences);
  }

  std::vector<size_t> indegrees;
  diGraph->mapVertices([&indegrees, diGraph](const Vertex* pVertex) {
    const size_t indegree = diGraph->getInDegree(pVertex, false);
    indegrees.push_back(indegree);
  });

  /* dynamicDiGraph.resetToBigBang();
  DynamicDiGraphStatistics statistics;
  statistics.analyzeDynamicDiGraph(&dynamicDiGraph);*/

  return {{"averageDensity", averageDensity},
          {"averageArcLifeTime", averageArcLifeTime},
          {"arcLifeTimes", arcLifeTimes},
          {"arcInsertionCounts", arcInsertionCounts},
        {"indegrees", indegrees}};
}

void analyzeDynamicDiGraphs(const Settings &settings) {
  json result;
  for (const auto &[graphDescription, pGraphInsantiator] :
       settings.graphInfos) {
    DynamicDiGraph dynamicDiGraph = pGraphInsantiator->instantiate();
    DynamicDiGraphAnalyzer analyzer;
    result[graphDescription] = analyzer.analyze(dynamicDiGraph);
  }
  std::string output = result.dump(2);

  if (settings.outputPath.empty()) {
    std::cout << output;
  } else {
    std::ofstream ofs{settings.outputPath};
    ofs << output;
  }
}