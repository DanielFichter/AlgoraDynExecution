#include "algorithmtype.hpp"
#include "algorithmsettings.hpp"

#include "algorithm.reachability.ss.es/estree-bqueue.h"
#include "algorithm.reachability.ss.es/estree-ml_timestamps_fpm.h"
#include "algorithm.reachability.ss.es/simpleestree.h"
#include "algorithm.reachability.ss.es/simpleestree_multipletreearcs.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling_lce.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling_mte.h"
#include "algorithm.reachability.ss.es/simpleestree_reservoirsampling_swce.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom_lce.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom_mte.h"
#include "algorithm.reachability.ss.es/simpleestree_selectrandom_swce.h"
#include "algorithm.reachability.ss.es/simpleestree_timestamps.h"
#include "algorithm.reachability.ss.es/simpleestree_timestamps_fpm.h"

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
  case AlgorithmType::SimpleESTreeMultipleTreeArcs: {
    const auto settings =
        dynamic_cast<const SimpleESTreeMultipleTreeArcsSettings &>(
            algorithmSettings);
    if (settings.reverseArcDirection) {
      if (settings.nTreeArcs == 2) {
        return std::make_unique<
            Algora::SimpleESTreeMultipleTreeArcs<true, 2>>();
      } else if (settings.nTreeArcs == 3) {
        return std::make_unique<
            Algora::SimpleESTreeMultipleTreeArcs<true, 3>>();
      } else if (settings.nTreeArcs == 4) {
        return std::make_unique<
            Algora::SimpleESTreeMultipleTreeArcs<true, 4>>();
      }
    }
    if (settings.nTreeArcs == 2) {
      return std::make_unique<Algora::SimpleESTreeMultipleTreeArcs<false, 2>>();
    } else if (settings.nTreeArcs == 3) {
      return std::make_unique<Algora::SimpleESTreeMultipleTreeArcs<false, 3>>();
    } else if (settings.nTreeArcs == 4) {
      return std::make_unique<Algora::SimpleESTreeMultipleTreeArcs<false, 4>>();
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
    auto settings =
        dynamic_cast<const SimpleESTreeTimeStampsSettings &>(algorithmSettings);
    if (settings.reverseArcDirection) {
      if (settings.preferOlder) {
        return std::make_unique<Algora::SimpleESTreeTimeStamps<true, true>>();
      } else {
        return std::make_unique<Algora::SimpleESTreeTimeStamps<true, false>>();
      }
    }
    if (settings.preferOlder) {
      return std::make_unique<Algora::SimpleESTreeTimeStamps<false, true>>();
    } else {
      return std::make_unique<Algora::SimpleESTreeTimeStamps<false, false>>();
    }
  }
  case AlgorithmType::SimpleESTreeTimeStampsFPM: {
    const auto &settings =
        dynamic_cast<const SimpleESTreeTimeStampsFPMSettings &>(
            algorithmSettings);
    if (settings.reverseArcDirection) {
      if (settings.preferOlder) {
        return std::make_unique<
            Algora::SimpleESTreeTimeStampsFPM<true, true>>();
      } else {
        return std::make_unique<
            Algora::SimpleESTreeTimeStampsFPM<true, false>>();
      }
    }
    if (settings.preferOlder) {
      return std::make_unique<Algora::SimpleESTreeTimeStampsFPM<false, true>>();
    } else {
      return std::make_unique<
          Algora::SimpleESTreeTimeStampsFPM<false, false>>();
    }
  }

  case AlgorithmType::ESTreeMLTimeStampsFPM: {
    auto settings =
        dynamic_cast<const ESTreeMLTimeStampsSettings &>(algorithmSettings);
    if (settings.reverseArcDirection) {
      if (settings.preferOlder) {
        return std::make_unique<Algora::ESTreeMLTimeStampsFPM<true, true>>();
      } else {
        return std::make_unique<Algora::ESTreeMLTimeStampsFPM<true, false>>();
      }
    }
    if (settings.preferOlder) {
      return std::make_unique<Algora::ESTreeMLTimeStampsFPM<false, true>>();
    } else {
      return std::make_unique<Algora::ESTreeMLTimeStampsFPM<false, false>>();
    }
  }
  default:
    throw std::invalid_argument("invalid algorithm type!");
  }
}