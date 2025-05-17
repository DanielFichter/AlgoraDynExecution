#include "operationstatistics.hpp"
#include "cli.hpp"
#include "io.hpp"
#include "measureperformance.hpp"
#include "executionmode.hpp"
#include "testcorrectness.hpp"

#include "algorithm.reachability.ss.es/simpleestree_selectrandom.h"
#include "graph.incidencelist/incidencelistgraph.h"
#include "graph.incidencelist/incidencelistvertex.h"
#include "graph.dyn/dynamicdigraph.h"
#include "algorithm.reachability.ss/staticbfsssreachalgorithm.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sys/mman.h>

using namespace Algora;
using namespace std::string_literals;

int main(int argc, char *argv[])
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

    switch (settings.executionMode)
    {
    case ExecutionMode::measurePerformance:
        measurePerformance(settings.iterationCount, graph, dynamicGraph, pAlgorithm.get());
        break;
    case ExecutionMode::testCorrectness:
        testCorrectness(graph, dynamicGraph, pAlgorithm.get());
        break;
    default:
        std::cerr << "invalid execution mode!" << std::endl;
        break;
    }

    pAlgorithm->unsetGraph();

    if (settings.preventPaging)
    {
        munlockall();
    }

    return 0;
}