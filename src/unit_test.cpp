#include "unit_test.hpp"

#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <algorithm.reachability.ss.es/simpleestree_timestamps.h>
#include <algorithm.reachability.ss.es/simpleestree_timestamps_fpm.h>
#include <algorithm.reachability.ss.es/estree-ml_timestamps_fpm.h>
#include <iostream>
#include <memory>

bool unit_test(const std::vector<std::pair<AlgorithmType, std::shared_ptr<AlgorithmSettings>>> &algorithmTypes) {

  bool result = true;
  for (const auto& [algorithmType, algorithmSettings] : algorithmTypes) {
    if(!unit_test(algorithmType, *algorithmSettings))
    {
        result = false;
    }
  }
  return result;
}

bool unit_test(AlgorithmType algorithmType, const AlgorithmSettings& algorithmSettings) {

  std::cout << "unit testing for algorithm type " << AlgorithmTypeNames.at(algorithmType) << algorithmSettings << std::endl;

  DynamicDiGraph dyDiGraph;
  dyDiGraph.addArc(1, 2, 0);
  dyDiGraph.addArc(1, 3, 0);
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
  std::unique_ptr<DynamicSingleSourceReachabilityAlgorithm> pAlgorithm =
      instantiate(algorithmType, algorithmSettings);

  IncidenceListGraph *graph = dyDiGraph.getDiGraph();
  pAlgorithm->setGraph(graph);
  if (algorithmType == AlgorithmType::SimpleESTreeTimeStampsFPM)
  {
    auto pAlgorithmTimeStamps = dynamic_cast<SimpleESTreeTimeStampsFPM<false, true>*>(pAlgorithm.get());
    pAlgorithmTimeStamps->setDyDiGraph(&dyDiGraph);
  }
  else if (algorithmType == AlgorithmType::ESTreeMLTimeStampsFPM)
  {
    auto pAlgorithmTimeStamps = dynamic_cast<ESTreeMLTimeStampsFPM<false, true>*>(pAlgorithm.get());
    pAlgorithmTimeStamps->setDyDiGraph(&dyDiGraph);
  }

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