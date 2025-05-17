#include "algorithmtype.hpp"

#include "algorithm.reachability.ss.es/simpleestree.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom.h"
#include "algorithm.reachability.ss.es/simpleestree_multipletreearcs.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling.h"

std::shared_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm> instantiate(AlgorithmType type)
{
    switch (type)
    {
    case AlgorithmType::SimpleESTree:
        return std::make_shared<Algora::SimpleESTree<false>>();
    case AlgorithmType::SimpleESTreeSelectRandom:
        return std::make_shared<Algora::SimpleESTreeSelectRandom<false>>();
    case AlgorithmType::SimpleESTreeMultipleTreeArcs:
        return std::make_shared<Algora::SimpleESTreeMultipleTreeArcs<false, 2>>();
    case AlgorithmType::SimpleESTreeReservoirSampling:
        return std::make_shared<Algora::SimpleESTreeReservoirSampling<false>>();
    default:
        throw std::invalid_argument("invalid algorithm type!");
    }
}