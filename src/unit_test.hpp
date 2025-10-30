#pragma once

#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <graph.incidencelist/incidencelistgraph.h>
#include <graph/vertex.h>


#include <vector>
#include <utility>
#include <memory>

using namespace Algora;

class AlgorithmSettings;

bool unit_test(const std::vector<std::pair<AlgorithmType, std::shared_ptr<AlgorithmSettings>>>& algorithmTypes);
bool unit_test(AlgorithmType algorithmType, const AlgorithmSettings&);