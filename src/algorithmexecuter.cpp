#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"

AlgorithmExecuter::AlgorithmExecuter(const std::string &graphName,
                                     AlgorithmType algorithmType,
                                     const AlgorithmSettings &algorithmSettings)
    : dynamicGraph{readKroneckerGraph("graphs/"s + graphName)},
      graph{dynamicGraph.getDiGraph()},
      pAlgorithm{instantiate(algorithmType, algorithmSettings)} {
  std::cout << "graph has been read" << std::endl;
  pAlgorithm->setGraph(graph);
  if (algorithmType == AlgorithmType::SimpleESTreeTimeStamps) {
    auto pAlgorithmTimeStamps =
        dynamic_cast<SimpleESTreeTimeStamps<false, true> *>(pAlgorithm.get());
    pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
  } else if (algorithmType == AlgorithmType::SimpleESTreeTimeStampsFPM) {
    auto settings = dynamic_cast<const SimpleESTreeTimeStampsFPMSettings &>(
        algorithmSettings);
    if (settings.preferOlder) {
      auto pAlgorithmTimeStamps =
          dynamic_cast<SimpleESTreeTimeStampsFPM<false, true> *>(
              pAlgorithm.get());
      pAlgorithmTimeStamps->setDyDiGraph(&dynamicGraph);
    }
    else {
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