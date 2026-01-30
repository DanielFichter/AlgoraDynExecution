#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "graphinstatiator.hpp"

AlgorithmExecuter::AlgorithmExecuter(GraphInstantiator &graphInstantiator,
                                     AlgorithmType algorithmType,
                                     const AlgorithmSettings &algorithmSettings)
    : dynamicGraph{graphInstantiator.instantiate()},
      graph{dynamicGraph.getDiGraph()},
      pAlgorithm{instantiate(algorithmType, algorithmSettings)} {
  std::cout << "graph has been instantiated" << std::endl;
  pAlgorithm->setGraph(graph);
  if (algorithmType == AlgorithmType::SimpleESTreeTimeStamps) {
    auto settings = dynamic_cast<const SimpleESTreeTimeStampsSettings &>(
        algorithmSettings);
    if (settings.preferOlder) {
      auto pAlgorithmTimeStamps =
          dynamic_cast<SimpleESTreeTimeStamps<false, true> *>(
              pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    } else {
      auto pAlgorithmTimeStamps =
          dynamic_cast<SimpleESTreeTimeStamps<false, false> *>(
              pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    }
  }
}