#include "measureperformance.hpp"
#include "operationtype.hpp"
#include "operationstatistics.hpp"

#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"
#include "graph.dyn/dynamicdigraph.h"
#include "graph.dyn/dynamicdigraph.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <map>
#include <iostream>

namespace
{
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

void measurePerformance(const unsigned iterationCount, const Algora::IncidenceListGraph *const graph, Algora::DynamicDiGraph &dynamicGraph, const std::shared_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm> &pAlgorithm)
{
    std::map<OperationType, OperationStatistics> operationDurations;
    for (unsigned iteration = 0; iteration < iterationCount; iteration++)
    {

        while (graph->getSize() < 1)
        {
            applyNextOperationAndMeasure(dynamicGraph, operationDurations);
        }
        auto source = graph->vertexAt(0);
        pAlgorithm->setSource(source);
        pAlgorithm->run();

        while (applyNextOperationAndMeasure(dynamicGraph, operationDurations))
            ;
        dynamicGraph.resetToBigBang();
    }

    for (const auto &[operationType, statistics] : operationDurations)
    {
        std::cout << "average duration of operation \"" << operationTypeNames.at(operationType) << "\": "
                  << statistics.getAverageDuration().count() << "ns" << std::endl;
    }
}