#pragma once

#include "settings.hpp"
#include <graph.dyn/dynamicdigraph.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Algora {
    class DynamicDiGraph;
}

class DynamicDiGraphAnalyzer {
public:

json analyze(Algora::DynamicDiGraph&);
};

void analyzeDynamicDiGraphs(const Settings&);