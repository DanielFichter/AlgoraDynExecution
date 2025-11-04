#pragma once

#include "algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h"

#include <map>
#include <memory>
#include <string>

class AlgorithmSettings;

enum class AlgorithmType {
  ESTree,
  SimpleESTree,
  SimpleESTreeSelectRandomMTE,
  SimpleESTreeSelectRandomLCE,
  SimpleESTreeSelectRandomSWCE,
  SimpleESTreeReservoirSamplingMTE,
  SimpleESTreeReservoirSamplingLCE,
  SimpleESTreeReservoirSamplingSWCE,
  SimpleESTreeMultipleTreeArcs,
  SimpleESTreeTimeStamps,
  SimpleESTreeTimeStampsFPM,
  ESTreeMLTimeStampsFPM
};

const std::map<AlgorithmType, std::string> AlgorithmTypeNames{
    {AlgorithmType::SimpleESTree, "SimpleESTree"},
    {AlgorithmType::SimpleESTreeSelectRandomMTE, "SimpleESTreeSelectRandomMTE"},
    {AlgorithmType::SimpleESTreeSelectRandomLCE, "SimpleESTreeSelectRandomLCE"},
    {AlgorithmType::SimpleESTreeSelectRandomSWCE, "SimpleESTreeSelectRandomSWCE"},
    {AlgorithmType::SimpleESTreeReservoirSamplingMTE,
     "SimpleESTreeReservoirSamplingMTE"},
    {AlgorithmType::SimpleESTreeReservoirSamplingLCE,
     "SimpleESTreeReservoirSamplingLCE"},
    {AlgorithmType::SimpleESTreeReservoirSamplingSWCE,
     "SimpleESTreeReservoirSamplingSWCE"},
    {AlgorithmType::SimpleESTreeMultipleTreeArcs,
     "SimpleESTreeMultipleTreeArcs"},
    {AlgorithmType::ESTree, "ESTree"},
    {AlgorithmType::SimpleESTreeTimeStamps, "SimpleESTreeTimeStamps"},
    {AlgorithmType::SimpleESTreeTimeStampsFPM, "SimpleESTreeTimeStampsFPM"},
    {AlgorithmType::ESTreeMLTimeStampsFPM, "ESTreeMLTimeStampsFPM"}
  };

std::unique_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm>
instantiate(AlgorithmType type, const AlgorithmSettings&);