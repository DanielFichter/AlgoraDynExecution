#pragma once

#include "algorithmtype.hpp"
#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistvertex.h"
#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <graph/vertex.h>

#include <iostream>
#include <memory>

using namespace Algora;

bool unit_test(AlgorithmType algorithmType) {
  DynamicDiGraph dyDiGraph;
  dyDiGraph.addArc(1, 2, 0);
  dyDiGraph.addArc(1, 3, 0);
  dyDiGraph.addArc(3, 4, 0);
  dyDiGraph.addArc(3, 4, 0);
  dyDiGraph.addArc(4, 3, 0);
  dyDiGraph.addArc(5, 6, 0);
  dyDiGraph.addArc(5, 7, 0);
  dyDiGraph.addArc(6, 8, 0);
  dyDiGraph.addArc(7, 6, 0);
  dyDiGraph.addArc(7, 8, 0);

  dyDiGraph.addArc(4, 6, 1);
  dyDiGraph.addArc(2, 5, 2);
  dyDiGraph.removeArc(1, 3, 3);
  dyDiGraph.removeArc(5, 6, 4);
  std::unique_ptr<DynamicSingleSourceReachabilityAlgorithm> pAlgorithm = instantiate(algorithmType);

  IncidenceListGraph *graph = dyDiGraph.getDiGraph();
  pAlgorithm->setGraph(graph);

  dyDiGraph.applyNextDelta();
  auto source = graph->vertexAt(0);
  pAlgorithm->setSource(source);
  pAlgorithm->run();
  pAlgorithm->dumpData(std::cout);

  dyDiGraph.applyNextDelta();
  pAlgorithm->dumpData(std::cout);
  dyDiGraph.applyNextDelta();
  pAlgorithm->dumpData(std::cout);
  dyDiGraph.applyNextDelta();
  pAlgorithm->dumpData(std::cout);
  dyDiGraph.applyNextDelta();
  pAlgorithm->dumpData(std::cout);

  return true;
}