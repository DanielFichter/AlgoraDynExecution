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
  if (algorithmType == AlgorithmType::SimpleESTreeTimeStampsFPM) {
    auto settings = dynamic_cast<const SimpleESTreeTimeStampsFPMSettings &>(
        algorithmSettings);
    if (settings.preferOlder) {
      auto pAlgorithmTimeStamps =
          dynamic_cast<SimpleESTreeTimeStampsFPM<false, true> *>(
              pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    } else {
      auto pAlgorithmTimeStamps =
          dynamic_cast<SimpleESTreeTimeStampsFPM<false, false> *>(
              pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    }

  } else if (algorithmType == AlgorithmType::ESTreeMLTimeStampsFPM) {
    auto pAlgorithmTimeStamps =
        dynamic_cast<ESTreeMLTimeStampsFPM<false, true> *>(pAlgorithm.get());
    pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
  }
}