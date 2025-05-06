#pragma once

#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"
#include "algorithm.reachability.ss.es/simpleestree.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom.h"

#include <stdexcept>
#include <memory>
#include <map>
#include <string>

enum class AlgorithmType
{
    SimpleESTree,
    SimpleESTreeSelectRandom
};

const std::map<AlgorithmType, std::string> AlgorithmTypeNames{{AlgorithmType::SimpleESTree, "SimpleESTree"},
                                                        {AlgorithmType::SimpleESTreeSelectRandom, "SimpleESTreeSelectRandom"}};

std::unique_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm> instantiate(AlgorithmType type);
