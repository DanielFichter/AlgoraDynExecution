#include "algorithmtype.hpp"

#include "algorithm.reachability.ss.es/simpleestree.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom.h"
#include "algorithm.reachability.ss.es/simpleestree_multipletreearcs.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling.h"
#include "algorithm.reachability.ss.es/estree-bqueue.h"
#include <memory>

std::unique_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm> instantiate(AlgorithmType type)
{
    switch (type)
    {
    case AlgorithmType::SimpleESTree:
        return std::make_unique<Algora::SimpleESTree<false>>();
    case AlgorithmType::SimpleESTreeSelectRandom:
        return std::make_unique<Algora::SimpleESTreeSelectRandom<false>>();
    case AlgorithmType::SimpleESTreeMultipleTreeArcs:
        return std::make_unique<Algora::SimpleESTreeMultipleTreeArcs<false, 2>>();
    case AlgorithmType::SimpleESTreeReservoirSampling:
        return std::make_unique<Algora::SimpleESTreeReservoirSampling<false>>();
    case AlgorithmType::ESTree:
        return std::make_unique<Algora::OldESTree>();
    default:
        throw std::invalid_argument("invalid algorithm type!");
    }
}