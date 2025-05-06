#include "algorithmtype.hpp"

std::unique_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm> instantiate(AlgorithmType type)
{
    switch (type)
    {
    case AlgorithmType::SimpleESTree:
       return std::make_unique<Algora::SimpleESTree<false>>();
    case AlgorithmType::SimpleESTreeSelectRandom:
        return std::make_unique<Algora::SimpleESTreeSelectRandom<false>>();
    default:
        throw std::invalid_argument("invalid algorithm type!");
    }
}