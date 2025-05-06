#include "algorithm.reachability.ss.es/simpleestree_selectrandom.h"
#include "graph.incidencelist/incidencelistgraph.h"
#include "graph.incidencelist/incidencelistvertex.h"
#include "io/konectnetworkreader.h"
#include "graph.dyn/dynamicdigraph.h"
#include "algorithm.reachability.ss/staticbfsssreachalgorithm.h"

#include "operationstatistics.hpp"
#include "operationtype.hpp"
#include "cli.hpp"
#include "io.hpp"
#include "timemeasurement.hpp"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sys/mman.h>

using namespace Algora;
using namespace std::string_literals;

int main(int argc, char * argv[])
{
    std::vector<std::string> input{argv + 1, argv + argc};
    CLI cli;
    const auto settings = cli.parseSettings(input);
    cli.printStartInfo();

    if (settings.preventPaging)
    {
        mlockall(MCL_CURRENT | MCL_FUTURE);
    }

    const std::string graphPath = "graphs/"s + settings.graphPath;
    DynamicDiGraph dynamicGraph = readKroneckerGraph(graphPath);
    const auto graph = dynamicGraph.getDiGraph();

    const auto pAlgorithm = instantiate(settings.algorithmType);
    pAlgorithm->setGraph(graph);

    std::map<OperationType, OperationStatistics> operationDurations;


    for (unsigned iteration = 0; iteration < settings.iterationCount; iteration++)
    {
        
        while (graph->getSize() < 1)
        {
            applyNextOperationAndMeasure(dynamicGraph, operationDurations);
        }
        const auto source = graph->vertexAt(0);
        pAlgorithm->setSource(source);
        std::cout << "Algorithm is perpared: " << pAlgorithm->prepare() << std::endl;

        while (applyNextOperationAndMeasure(dynamicGraph, operationDurations))
            ;
        dynamicGraph.resetToBigBang();
    }

    for (const auto &[operationType, statistics] : operationDurations)
    {
        std::cout << "average duration of operation \"" << operationTypeNames.at(operationType) << "\": " 
        << statistics.getAverageDuration().count() << "ns" << std::endl;
    }

    pAlgorithm->unsetGraph();

    if (settings.preventPaging)
    {
        munlockall();
    }

    return 0;
}