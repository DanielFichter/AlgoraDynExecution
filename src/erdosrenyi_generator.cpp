#include "erdosrenyi_generator.hpp"
#include <cstddef>
#include <graph.dyn/dynamicdigraph.h>
#include <random>

DynamicDiGraph generateErdosRenyi(size_t n, size_t m) {

    std::random_device randomDevice;
    std::mt19937 randomEngine(randomDevice());

    DynamicDiGraph result;

    for (size_t vertexIndex = 0; vertexIndex < n; vertexIndex++) {
        result.addVertex(0);
    }

    for (size_t arcIndex = 0; arcIndex < m; arcIndex++) {
        size_t vertexIndex1 = randomEngine() % n;
        size_t vertexIndex2 = randomEngine() % n;

        result.addArc(vertexIndex1, vertexIndex2, 0);
    }
    

    return result;
}