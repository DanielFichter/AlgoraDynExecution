#include "algorithm.reachability.ss.es/simpleestree.h"
#include "graph.incidencelist/incidencelistgraph.h"
#include "graph.incidencelist/incidencelistvertex.h"
#include "io/konectnetworkreader.h"
#include "graph.dyn/dynamicdigraph.h"

#include <iostream>
#include <fstream>

int main()
{
    using namespace Algora;

    SimpleESTree<false> algorithm;

    KonectNetworkReader graphReader;
    std::ifstream graphInput{"graphs/answers"};
    graphReader.setInputStream(&graphInput);
    DynamicDiGraph dynamicGraph;
    graphReader.provideDynamicDiGraph(&dynamicGraph);
    const auto graph = dynamicGraph.getDiGraph();
    algorithm.setAutoUpdate(true);
    algorithm.setGraph(graph);
    while (graph->getSize() < 1)
    {
        dynamicGraph.applyNextOperation();
    }
    const auto source = graph->vertexAt(0);
    algorithm.setSource(source);

    std::cout << std::boolalpha;
    std::cout << "Algorithm is perpared: " << algorithm.prepare() << std::endl;

    algorithm.unsetGraph();

    return 0;
}