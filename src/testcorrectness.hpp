#pragma once

#include <memory>

namespace Algora
{
    class IncidenceListGraph;
    class DynamicDiGraph;
    class DynamicSingleSourceReachabilityAlgorithm;
}

void testCorrectness(Algora::IncidenceListGraph *const graph, Algora::DynamicDiGraph &dynamicGraph, Algora::DynamicSingleSourceReachabilityAlgorithm *const pAlgorithm);