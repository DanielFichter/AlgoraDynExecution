#pragma once

#include <memory>

namespace Algora
{
    class IncidenceListGraph;
    class DynamicDiGraph;
    class DynamicSingleSourceReachabilityAlgorithm;
}

void testCorrectness(Algora::IncidenceListGraph* const graph, Algora::DynamicDiGraph& dynamicGraph, const std::shared_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm> &pAlgorithm);