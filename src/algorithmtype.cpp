#include "algorithmtype.hpp"
#include "algorithmsettings.hpp"

#include "algorithm.reachability.ss.es/estree-bqueue.h"
#include "algorithm.reachability.ss.es/simpleesdag.h"
#include "algorithm.reachability.ss.es/simpleestree.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling_lce.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling_mte.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling_swce.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom_lce.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom_mte.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom_swce.h"
#include "algorithm.reachability.ss.es/simpleestree_timestamps.h"
#include "algorithm.reachability.ss/simpleincssreachalgorithm.h"

#include <memory>

std::unique_ptr<Algora::DynamicSingleSourceReachabilityAlgorithm>
instantiate(AlgorithmType type, const AlgorithmSettings &algorithmSettings) {
  switch (type) {
  case AlgorithmType::SimpleESTree: {
    auto settings =
        dynamic_cast<const SimpleESTreeSettings &>(algorithmSettings);
    if (settings.reverseArcDirection) {
      return std::make_unique<Algora::SimpleESTree<true>>();
    } else {
      return std::make_unique<Algora::SimpleESTree<false>>();
    }
  }
  case AlgorithmType::SimpleESTreeSelectRandomMTE: {
    auto settings = dynamic_cast<const SimpleESTReeSelectRandomSettings &>(
        algorithmSettings);
    if (settings.reverseArcDirection) {
      return std::make_unique<Algora::SimpleESTreeSelectRandomMTE<true>>();
    } else {
      return std::make_unique<Algora::SimpleESTreeSelectRandomMTE<false>>();
    }
  }
  case AlgorithmType::SimpleESTreeSelectRandomLCE: {
    auto settings = dynamic_cast<const SimpleESTReeSelectRandomSettings &>(
        algorithmSettings);
    if (settings.reverseArcDirection) {
      return std::make_unique<Algora::SimpleESTreeSelectRandomLCE<true>>();
    } else {
      return std::make_unique<Algora::SimpleESTreeSelectRandomLCE<false>>();
    }
  }
  case AlgorithmType::SimpleESTreeSelectRandomSWCE: {
    auto settings = dynamic_cast<const SimpleESTReeSelectRandomSettings &>(
        algorithmSettings);
    if (settings.reverseArcDirection) {
      return std::make_unique<Algora::SimpleESTreeSelectRandomSWCE<true>>();
    } else {
      return std::make_unique<Algora::SimpleESTreeSelectRandomSWCE<false>>();
    }
  }
  case AlgorithmType::SimpleESDAG: {
    const auto settings =
        dynamic_cast<const SimpleESDAGSettings &>(algorithmSettings);
    if (settings.reverseArcDirection) {
      if (settings.nTreeArcs == 2) {
        return std::make_unique<Algora::SimpleESDAG<true, 2>>();
      } else if (settings.nTreeArcs == 3) {
        return std::make_unique<Algora::SimpleESDAG<true, 3>>();
      } else if (settings.nTreeArcs == 4) {
        return std::make_unique<Algora::SimpleESDAG<true, 4>>();
      } else if (settings.nTreeArcs == 5) {
        return std::make_unique<Algora::SimpleESDAG<true, 5>>();
      }
    }
    if (settings.nTreeArcs == 2) {
      return std::make_unique<Algora::SimpleESDAG<false, 2>>();
    } else if (settings.nTreeArcs == 3) {
      return std::make_unique<Algora::SimpleESDAG<false, 3>>();
    } else if (settings.nTreeArcs == 4) {
      return std::make_unique<Algora::SimpleESDAG<false, 4>>();
    } else if (settings.nTreeArcs == 5) {
      return std::make_unique<Algora::SimpleESDAG<false, 5>>();
    }
  }
  case AlgorithmType::SimpleESTreeReservoirSamplingMTE: {
    auto settings = dynamic_cast<const SimpleESTReeReservoirSamplingSettings &>(
        algorithmSettings);
    if (settings.reverseArcDirection) {
      return std::make_unique<Algora::SimpleESTreeReservoirSamplingMTE<true>>();
    } else {
      return std::make_unique<
          Algora::SimpleESTreeReservoirSamplingMTE<false>>();
    }
  }
  case AlgorithmType::SimpleESTreeReservoirSamplingLCE: {
    auto settings = dynamic_cast<const SimpleESTReeReservoirSamplingSettings &>(
        algorithmSettings);
    if (settings.reverseArcDirection) {
      return std::make_unique<Algora::SimpleESTreeReservoirSamplingLCE<true>>();
    } else {
      return std::make_unique<
          Algora::SimpleESTreeReservoirSamplingLCE<false>>();
    }
  }
  case AlgorithmType::SimpleESTreeReservoirSamplingSWCE: {
    auto settings = dynamic_cast<const SimpleESTReeReservoirSamplingSettings &>(
        algorithmSettings);
    if (settings.reverseArcDirection) {
      return std::make_unique<
          Algora::SimpleESTreeReservoirSamplingSWCE<true>>();
    } else {
      return std::make_unique<
          Algora::SimpleESTreeReservoirSamplingSWCE<false>>();
    }
  }
  case AlgorithmType::ESTree:
    return std::make_unique<Algora::OldESTree>();
  case AlgorithmType::SimpleESTreeTimeStamps: {
    const auto &settings =
        dynamic_cast<const SimpleESTreeTimeStampsSettings &>(
            algorithmSettings);
    if (settings.reverseArcDirection) {
      if (settings.preferOlder) {
        return std::make_unique<
            Algora::SimpleESTreeTimeStamps<true, true>>();
      } else {
        return std::make_unique<
            Algora::SimpleESTreeTimeStamps<true, false>>();
      }
    }
    if (settings.preferOlder) {
      return std::make_unique<Algora::SimpleESTreeTimeStamps<false, true>>();
    } else {
      return std::make_unique<
          Algora::SimpleESTreeTimeStamps<false, false>>();
    }
  }

  case AlgorithmType::SimpleIncremental:
    return std::make_unique<Algora::SimpleIncSSReachAlgorithm<>>();

  default:
    throw std::invalid_argument("invalid algorithm type!");
  }
}