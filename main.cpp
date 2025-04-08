#include "algorithm.reachability.ss.es/simpleestree.h"
#include "graph.incidencelist/incidencelistgraph.h"
#include "graph.incidencelist/incidencelistvertex.h"
#include "io/konectnetworkreader.h"
#include "graph.dyn/dynamicdigraph.h"
#include "algorithm.reachability.ss/staticbfsssreachalgorithm.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>

using namespace Algora;

namespace
{
    std::string getLastOperationType(const DynamicDiGraph &dynamicGraph)
    {
        if (dynamicGraph.lastOpWasVertexAddition())
        {
            return "Vertex Addition";
        }
        else if (dynamicGraph.lastOpWasVertexRemoval())
        {
            return "Vertex Removal";
        }
        else if (dynamicGraph.lastOpWasArcAddition())
        {
            return "Arc Addition";
        }
        else if (dynamicGraph.lastOpWasArcRemoval())
        {
            return "Arc Removal";
        }
        else
        {
            return "No Operation";
        }
    }

    DynamicDiGraph readKroneckerGraph(const std::string &filename)
    {
        DynamicDiGraph dynamicGraph;
        KonectNetworkReader graphReader;
        std::ifstream graphInput{filename};
        graphReader.setInputStream(&graphInput);
        graphReader.provideDynamicDiGraph(&dynamicGraph);
        return dynamicGraph;
    }
}

int main()
{
    SimpleESTree<false, ParentSelectStrategy::firstOptimal> simpleES;

    DynamicDiGraph dynamicGraph = readKroneckerGraph("graphs/gnutella-25");
    const auto graph = dynamicGraph.getDiGraph();
    simpleES.setGraph(graph);
    size_t operationIndex = 0;

    std::cout << std::boolalpha;

    constexpr unsigned iterationCount = 100;

    const auto timeBefore = std::chrono::system_clock::now();
    for (unsigned iteration = 0; iteration < iterationCount; iteration++)
    {
        while (graph->getSize() < 1)
        {
            dynamicGraph.applyNextOperation();
            operationIndex++;
        }
        const auto source = graph->vertexAt(0);
        simpleES.setSource(source);
        std::cout << "Algorithm is perpared: " << simpleES.prepare() << std::endl;

        while (dynamicGraph.applyNextOperation())
            ;
        dynamicGraph.resetToBigBang();
    }
    const auto timeAfter = std::chrono::system_clock::now();
    simpleES.unsetGraph();

    std::cout << "average duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(timeAfter - timeBefore).count() / iterationCount << "ms" << std::endl;

    return 0;
}