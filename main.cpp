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
    SimpleESTree<false> simpleES;

    DynamicDiGraph dynamicGraph = readKroneckerGraph("graphs/gnutella-25");
    const auto graph = dynamicGraph.getDiGraph();
    simpleES.setGraph(graph);
    size_t operationIndex = 0;
    while (graph->getSize() < 1)
    {
        dynamicGraph.applyNextOperation();
        operationIndex++;
    }
    const auto source = graph->vertexAt(0);
    simpleES.setSource(source);

    std::cout << std::boolalpha;
    std::cout << "Algorithm is perpared: " << simpleES.prepare() << std::endl;

    constexpr size_t finalOperationIndex = 223057LLU;
    const std::string targetVertexName = "117";
    const Vertex *vertex = nullptr;
    for (size_t operationCount = 0; operationCount < finalOperationIndex + 1; operationCount++)
    {
        dynamicGraph.applyNextOperation();

        if (dynamicGraph.lastOpWasArcAddition())
        {
            for (size_t vertexIndexOffset = 1; vertexIndexOffset < 3; vertexIndexOffset++)
            {
                const Vertex *const currentVertex = graph->vertexAt(graph->getSize() - vertexIndexOffset);
                if (currentVertex->getName() == targetVertexName)
                {
                    vertex = currentVertex;
                }
            }
        }
    }

    const auto path = simpleES.queryPath(vertex);

    std::cout << "after operation " << finalOperationIndex << "(" << getLastOperationType(dynamicGraph) << "): ";
    std::cout << "is vertex " << targetVertexName << "(id=" << vertex->getName() << ") reachable? " << simpleES.query(vertex) << ", path length: " << path.size() << std::endl;

    simpleES.unsetGraph();

    return 0;
}