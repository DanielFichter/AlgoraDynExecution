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
#include <vector>
#include <map>
#include <sys/mman.h>

using namespace Algora;

namespace
{
    enum class OperationType
    {
        arcAddition,
        arcRemoval,
        vertexAddition,
        vertexRemoval,
        noOperation,
    };
    const std::map<OperationType, std::string> operationTypeNames{{OperationType::arcAddition, "arcAddition"},
                                                                  {OperationType::arcRemoval, "arcRemoval"},
                                                                  {OperationType::vertexAddition, "vertexAddition"},
                                                                  {OperationType::vertexRemoval, "vertexRemoval"}};

    OperationType getLastOperationType(const DynamicDiGraph &dynamicGraph)
    {
        if (dynamicGraph.lastOpWasVertexAddition())
        {
            return OperationType::vertexAddition;
        }
        else if (dynamicGraph.lastOpWasVertexRemoval())
        {
            return OperationType::vertexRemoval;
        }
        else if (dynamicGraph.lastOpWasArcAddition())
        {
            return OperationType::arcAddition;
        }
        else if (dynamicGraph.lastOpWasArcRemoval())
        {
            return OperationType::arcRemoval;
        }
        else
        {
            return OperationType::noOperation;
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

    class OperationStatistics
    {
    public:
        void addOccurnece(const std::chrono::nanoseconds &duration)
        {
            nOccurences++;
            timeSpent += duration;
        }
        std::chrono::nanoseconds getAverageDuration() const
        {
            return timeSpent / nOccurences;
        }

    private:
        std::chrono::nanoseconds timeSpent{0};
        long long unsigned nOccurences = 0;
    };

    bool applyNextOperationAndMeasure(DynamicDiGraph &dynamicGraph, std::map<OperationType, OperationStatistics> &operationDurations)
    {
        const auto timeBeforeOperation = std::chrono::high_resolution_clock::now();
        const bool operationsLeft = dynamicGraph.applyNextOperation();
        const auto timeAfterOperation = std::chrono::high_resolution_clock::now();

        const std::chrono::nanoseconds duration = timeAfterOperation - timeBeforeOperation;
        const auto lastOperationType = getLastOperationType(dynamicGraph);
        operationDurations[lastOperationType].addOccurnece(duration);

        return operationsLeft;
    }
}

int main()
{
    mlockall(MCL_CURRENT | MCL_FUTURE);

    DynamicDiGraph dynamicGraph = readKroneckerGraph("graphs/konect/out.link-dynamic-dewiki");
    const auto graph = dynamicGraph.getDiGraph();

    std::cout << std::boolalpha;

    constexpr unsigned iterationCount = 1;
    SimpleESTree<false, ParentSelectStrategy::randomOptimal> simpleES;
    simpleES.setGraph(graph);

    std::map<OperationType, OperationStatistics> operationDurations;


    for (unsigned iteration = 0; iteration < iterationCount; iteration++)
    {
        
        while (graph->getSize() < 1)
        {
            applyNextOperationAndMeasure(dynamicGraph, operationDurations);
        }
        const auto source = graph->vertexAt(0);
        simpleES.setSource(source);
        std::cout << "Algorithm is perpared: " << simpleES.prepare() << std::endl;

        while (applyNextOperationAndMeasure(dynamicGraph, operationDurations))
            ;
        dynamicGraph.resetToBigBang();
    }

    for (const auto &[operationType, statistics] : operationDurations)
    {
        std::cout << "average duration of operation \"" << operationTypeNames.at(operationType) << "\": " 
        << statistics.getAverageDuration().count() << "ns" << std::endl;
    }

    simpleES.unsetGraph();

    munlockall();

    return 0;
}