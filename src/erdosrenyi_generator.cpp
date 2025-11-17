#include "erdosrenyi_generator.hpp"
#include <cstddef>
#include <graph.dyn/dynamicdigraph.h>
#include <random>
#include <utility>
#include <vector>

std::pair<DynamicDiGraph, std::vector<std::pair<DynamicDiGraph::VertexIdentifier, DynamicDiGraph::VertexIdentifier>>> generateErdosRenyi(size_t n, size_t m) {

    std::random_device randomDevice;
    std::mt19937 randomEngine(randomDevice());

    DynamicDiGraph erdosrenyiGraph;
    std::vector<std::pair<DynamicDiGraph::VertexIdentifier, DynamicDiGraph::VertexIdentifier>> arcs;
    arcs.reserve(m);

    for (size_t vertexIndex = 0; vertexIndex < n; vertexIndex++) {
        erdosrenyiGraph.addVertex(0);
    }

    for (size_t arcIndex = 0; arcIndex < m; arcIndex++) {
        size_t vertexIndex1 = randomEngine() % n;
        size_t vertexIndex2 = randomEngine() % n;

        erdosrenyiGraph.addArc(vertexIndex1, vertexIndex2, 0);
        arcs.push_back({vertexIndex1, vertexIndex2});
    }
    

    return {erdosrenyiGraph, arcs};
}

DynamicDiGraph generateErdosRenyi(size_t n, size_t m, size_t numOperations, size_t batchSize) {
    auto [graph, arcs] = generateErdosRenyi(n, m);

    std::random_device randomDevice;
    std::mt19937 randomEngine(randomDevice());

    DynamicDiGraph::DynamicTime time = 1;

    for (size_t operationIndex = 0; operationIndex < numOperations; operationIndex++) {
        for (size_t insertion_index = 0; insertion_index < batchSize && operationIndex < numOperations; insertion_index++) {
            size_t vertexIndex1 = randomEngine() % n;
            size_t vertexIndex2 = randomEngine() % n;

            graph.addArc(vertexIndex1, vertexIndex2, time);
            arcs.push_back({vertexIndex1, vertexIndex2});
        }
        time++;

        for (size_t deletionIndex = 0; deletionIndex < batchSize && operationIndex < numOperations; deletionIndex++) {
            size_t arcIndex = randomEngine() % arcs.size();
            const auto [tailId, headId] = arcs[arcIndex];
            graph.removeArc(tailId, headId, time);
            arcs.erase(arcs.begin() + arcIndex);
        }
        time++;
    }

    return graph;
}