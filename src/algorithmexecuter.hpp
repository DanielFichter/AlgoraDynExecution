#pragma once

#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "graphinstatiator.hpp"

#include <algorithm.reachability.ss.es/simpleestree_timestamps_fpm.h>
#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <graph.dyn/dynamicdigraph.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <algorithm.reachability.ss.es/simpleestree_timestamps.h>
#include <algorithm.reachability.ss.es/estree-ml_timestamps_fpm.h>

#include <memory>

using namespace Algora;
using namespace std::string_literals;

class AlgorithmSettings;
class GraphInstantiator;

class AlgorithmExecuter {

public:
  virtual void execute() = 0;

  AlgorithmExecuter(GraphInstantiator&, AlgorithmType algorithmType, const AlgorithmSettings&);

  virtual ~AlgorithmExecuter() = default;

protected:
  void cleanup() { pAlgorithm->unsetGraph(); }

  DynamicDiGraph dynamicGraph;
  IncidenceListGraph *graph;
  std::unique_ptr<DynamicSingleSourceReachabilityAlgorithm> pAlgorithm;
};
