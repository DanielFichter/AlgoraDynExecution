#pragma once

#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"

#include <map>
#include <memory>
#include <string>

class AlgorithmSettings;

enum class AlgorithmType {
  ESTree,
  SimpleESTree,
  SimpleESTreeSelectRandom,
  SimpleESTreeReservoirSampling,
  SimpleESTreeMultipleTreeArcs,
  SimpleESTreeTimeStamps,
  SimpleESTreeTimeStampsFPM,
  ESTreeMLTimeStampsFPM
};

const std::map<AlgorithmType, std::string> AlgorithmTypeNames{
    {AlgorithmType::SimpleESTree, "SimpleESTree"},
    {AlgorithmType::SimpleESTreeSelectRandom, "SimpleESTreeSelectRandom"},
    {AlgorithmType::SimpleESTreeReservoirSampling,
     "SimpleESTreeReservoirSampling"},
    {AlgorithmType::SimpleESTreeMultipleTreeArcs,
     "SimpleESTreeMultipleTreeArcs"},
    {AlgorithmType::ESTree, "ESTree"},
    {AlgorithmType::SimpleESTreeTimeStamps, "SimpleESTreeTimeStamps"},
    {AlgorithmType::SimpleESTreeTimeStampsFPM, "SimpleESTreeTimeStampsFPM"},
    {AlgorithmType::ESTreeMLTimeStampsFPM, "ESTreeMLTimeStampsFPM"}
  };

std::unique_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm>
instantiate(AlgorithmType type, const AlgorithmSettings&);