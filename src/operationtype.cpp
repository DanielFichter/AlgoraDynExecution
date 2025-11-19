#include "operationtype.hpp"

OperationType getLastOperationType(const DynamicDiGraph &dynamicGraph)
{
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