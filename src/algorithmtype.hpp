#pragma once

#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"

#include <map>
#include <memory>
#include <string>

enum class AlgorithmType {
  ESTree,
  SimpleESTree,
  SimpleESTreeSelectRandom,
  SimpleESTreeReservoirSampling,
  SimpleESTreeMultipleTreeArcs,
  SimpleESTreeTimeStamps
};

const std::map<AlgorithmType, std::string> AlgorithmTypeNames{
    {AlgorithmType::SimpleESTree, "SimpleESTree"},
    {AlgorithmType::SimpleESTreeSelectRandom, "SimpleESTreeSelectRandom"},
    {AlgorithmType::SimpleESTreeReservoirSampling,
     "SimpleESTreeReservoirSampling"},
    {AlgorithmType::SimpleESTreeMultipleTreeArcs,
     "SimpleESTreeMultipleTreeArcs"},
    {AlgorithmType::ESTree, "ESTree"},
    {AlgorithmType::SimpleESTreeTimeStamps, "SimpleESTreeTimeStamps"}
  };

std::unique_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm>
instantiate(AlgorithmType type);