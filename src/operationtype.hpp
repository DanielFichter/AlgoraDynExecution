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
};

const std::map<OperationType, std::string> operationTypeNames{
    {OperationType::arcAddition, "arcAddition"},
    {OperationType::arcRemoval, "arcRemoval"},
    {OperationType::vertexAddition, "vertexAddition"},
    {OperationType::vertexRemoval, "vertexRemoval"}};

OperationType getLastOperationType(const DynamicDiGraph &dynamicGraph) {
  if (dynamicGraph.lastOpWasVertexAddition()) {
    return OperationType::vertexAddition;
  } else if (dynamicGraph.lastOpWasVertexRemoval()) {
    return OperationType::vertexRemoval;
  } else if (dynamicGraph.lastOpWasArcAddition()) {
    return OperationType::arcAddition;
  } else if (dynamicGraph.lastOpWasArcRemoval()) {
    return OperationType::arcRemoval;
  } else {
    return OperationType::noOperation;
  }
}