#include "analyzedynamicdigraph.hpp"
#include <graph.dyn/dynamicdigraph.h>

using namespace Algora;

json DynamicDiGraphAnalyzer::analyze(DynamicDiGraph&) {
    return {{"data", "value"}};
}

void analyzeDynamicDiGraphs(const Settings& settings) {
    json result;
    for (const auto& [graphDescription, pGraphInsantiator]: settings.graphInfos) {
        DynamicDiGraph dynamicDiGraph = pGraphInsantiator->instantiate();
        DynamicDiGraphAnalyzer analyzer;
        result[graphDescription] = analyzer.analyze(dynamicDiGraph);
    }
    std::cout << result;
}