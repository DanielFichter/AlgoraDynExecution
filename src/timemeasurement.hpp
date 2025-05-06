#include "graph.dyn/dynamicdigraph.h"

#include <map>

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