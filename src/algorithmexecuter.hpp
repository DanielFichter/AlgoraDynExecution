#pragma once

#include "algorithmtype.hpp"
#include "io.hpp"
#include <algorithm.reachability.ss.es/simpleestree_timestamps_fpm.h>
#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <graph.dyn/dynamicdigraph.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <algorithm.reachability.ss.es/simpleestree_timestamps.h>
#include <algorithm.reachability.ss.es/estree-ml_timestamps_fpm.h>

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
    if (algorithmType == AlgorithmType::SimpleESTreeTimeStamps)
    {
      auto pAlgorithmTimeStamps = dynamic_cast<SimpleESTreeTimeStamps<false, true>*>(pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    }
    else if (algorithmType == AlgorithmType::SimpleESTreeTimeStampsFPM)
    {
      auto pAlgorithmTimeStamps = dynamic_cast<SimpleESTreeTimeStampsFPM<false, true>*>(pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    }
    else if (algorithmType == AlgorithmType::ESTreeMLTimeStampsFPM)
    {
      auto pAlgorithmTimeStamps = dynamic_cast<ESTreeMLTimeStampsFPM<false, true>*>(pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    }
  }

  virtual ~AlgorithmExecuter() = default;

protected:
  void cleanup() { pAlgorithm->unsetGraph(); }

  DynamicDiGraph dynamicGraph;
  IncidenceListGraph *graph;
  std::unique_ptr<DynamicSingleSourceReachabilityAlgorithm> pAlgorithm;
};
