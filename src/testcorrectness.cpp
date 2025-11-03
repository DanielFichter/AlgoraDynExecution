#include "testcorrectness.hpp"
#include "algorithmsettings.hpp"
#include "settings.hpp"

#include "algorithm.reachability.ss.es/simpleestree.h"
#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"
#include "algorithmexecuter.hpp"
#include "algorithmtype.hpp"
#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistgraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <stdexcept>
#include <string>

using namespace Algora;
using namespace std::string_literals;

void testCorrectness(const Settings &settings) {

  for (const std::string &graphName : settings.graphNames) {
    for (const auto &[algorithmType, algorithmSettings] :
         settings.algorithmInfos) {
      std::cout << "testing correctness of algorithm \""
                << AlgorithmTypeNames.at(algorithmType) << "\" on graph \""
                << graphName << "\"" << std::endl;
      CorrectnessTester correctnessTester{graphName, algorithmType,
                                          *algorithmSettings};
      correctnessTester.execute();
    }
  }
}

CorrectnessTester::CorrectnessTester(const std::string &graphName,
                                     AlgorithmType algorithmType,
                                     const AlgorithmSettings &algorithmSettings)
    : AlgorithmExecuter(graphName, algorithmType, algorithmSettings) {}

void CorrectnessTester::execute() {
  SimpleESTree correctAlgorithm;
  correctAlgorithm.setGraph(graph);
  while (graph->getSize() < 1) {
    dynamicGraph.applyNextOperation();
  }
  auto source = graph->vertexAt(0);
  pAlgorithm->setSource(source);
  correctAlgorithm.setSource(source);

  pAlgorithm->run();

  while (dynamicGraph.applyNextOperation())
    ;

  std::cout << "algorithm executed sucessfully" << std::endl;

  for (unsigned vertexIndex = 0; vertexIndex < graph->getSize();
       vertexIndex++) {
    const auto *vertex = graph->vertexAt(vertexIndex);
    const auto truePath = correctAlgorithm.queryPath(vertex);
    const auto path = pAlgorithm->queryPath(vertex);
    if (path.size() != truePath.size()) {
      throw std::runtime_error(
          "path to for vertex "s + std::to_string(vertexIndex) +
          " should have length "s + std::to_string(truePath.size()) +
          ", but has length "s + std::to_string(path.size()));
    }
  }

  correctAlgorithm.unsetGraph();

  std::cout << "all tests succeeded!" << std::endl;

  cleanup();
}