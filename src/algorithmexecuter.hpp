#pragma once

#include "algorithmtype.hpp"
#include "io.hpp"
#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <graph.dyn/dynamicdigraph.h>
#include <graph.incidencelist/incidencelistgraph.h>

#include <memory>
#include <iostream>

using namespace Algora;
using namespace std::string_literals;

class AlgorithmExecuter {

public:
  virtual void execute() = 0;

  AlgorithmExecuter(const std::string &graphName, AlgorithmType algorithmType)
      : dynamicGraph{readKroneckerGraph("graphs/"s + graphName)},
        graph{dynamicGraph.getDiGraph()},
        pAlgorithm{instantiate(algorithmType)} {
          std::cout << "graph has been read" << std::endl;
    pAlgorithm->setGraph(graph);
  }

  virtual ~AlgorithmExecuter() = default;

protected:
  void cleanup() { pAlgorithm->unsetGraph(); }

  DynamicDiGraph dynamicGraph;
  IncidenceListGraph *graph;
  std::unique_ptr<DynamicSingleSourceReachabilityAlgorithm> pAlgorithm;
};
