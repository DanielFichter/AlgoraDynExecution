#pragma once

#include "graph.dyn/dynamicdigraph.h"

#include <map>
#include <string>

using namespace Algora;

enum class OperationType {
  arcAddition,
  arcRemoval,
  vertexAddition,
  vertexRemoval,
  noOperation,
  query
};

const std::map<OperationType, std::string> operationTypeNames{
    {OperationType::arcAddition, "arcAddition"},
    {OperationType::arcRemoval, "arcRemoval"},
    {OperationType::vertexAddition, "vertexAddition"},
    {OperationType::vertexRemoval, "vertexRemoval"},
    {OperationType::query, "query"},
    {OperationType::noOperation, "noOperation"}};

OperationType getLastOperationType(const DynamicDiGraph &dynamicGraph);