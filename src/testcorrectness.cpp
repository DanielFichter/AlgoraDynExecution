#include "testcorrectness.hpp"

#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"
#include "graph.dyn/dynamicdigraph.h"
#include "algorithm.reachability.ss.es/simpleestree.h"
#include "graph.incidencelist/incidencelistvertex.h"
#include "graph.incidencelist/incidencelistgraph.h"

#include <stdexcept>
#include <string>

using namespace Algora;
using namespace std::string_literals;

void testCorrectness(Algora::IncidenceListGraph *const graph, Algora::DynamicDiGraph &dynamicGraph, Algora::DynamicSingleSourceReachabilityAlgorithm *const pAlgorithm)
{
    SimpleESTree correctAlgorithm;
    correctAlgorithm.setGraph(graph);
    while (graph->getSize() < 1)
    {
        dynamicGraph.applyNextOperation();
    }
    auto source = graph->vertexAt(0);
    pAlgorithm->setSource(source);
    correctAlgorithm.setSource(source);

    pAlgorithm->run();

    while (dynamicGraph.applyNextOperation())
        ;

    for (unsigned vertexIndex = 0; vertexIndex < graph->getSize(); vertexIndex++)
    {
        const auto *vertex = graph->vertexAt(vertexIndex);
        const auto truePath = correctAlgorithm.queryPath(vertex);
        const auto path = pAlgorithm->queryPath(vertex);
        if (path.size() != truePath.size())
        {
            throw std::runtime_error("path to for vertex "s + std::to_string(vertexIndex) + " should have length "s + std::to_string(truePath.size()) + ", but has length "s + std::to_string(path.size()));
        }
    }

    correctAlgorithm.unsetGraph();

    std::cout << "algorithm executed without errors" << std::endl;
}