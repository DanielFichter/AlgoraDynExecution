#pragma once

#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"

#include <stdexcept>
#include <memory>
#include <map>
#include <string>

enum class AlgorithmType
{
    SimpleESTree,
    SimpleESTreeSelectRandom,
    SimpleESTreeReservoirSampling,
    SimpleESTreeMultipleTreeArcs
};

const std::map<AlgorithmType, std::string> AlgorithmTypeNames{{AlgorithmType::SimpleESTree, "SimpleESTree"},
                                                              {AlgorithmType::SimpleESTreeSelectRandom, "SimpleESTreeSelectRandom"},
                                                              {AlgorithmType::SimpleESTreeReservoirSampling, "SimpleESTreeReservoirSampling"},
                                                              {AlgorithmType::SimpleESTreeMultipleTreeArcs, "SimpleESTreeMultipleTreeArcs"}};

std::shared_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm> instantiate(AlgorithmType type);
