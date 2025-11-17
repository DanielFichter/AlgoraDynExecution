#pragma once

#include <graph.dyn/dynamicdigraph.h>

using namespace Algora;

std::pair<DynamicDiGraph, std::vector<std::pair<DynamicDiGraph::VertexIdentifier, DynamicDiGraph::VertexIdentifier>>> generateErdosRenyi(size_t n, size_t m);
DynamicDiGraph generateErdosRenyi(size_t n, size_t m, size_t numOperations, size_t batchSize = 10);