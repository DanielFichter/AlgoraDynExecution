#include "createalgorithmsettings.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "booltostring.hpp"
#include <ios>
#include <memory>
#include <stdexcept>
#include <vector>

std::shared_ptr<AlgorithmSettings>
create(AlgorithmType algorithmType, const std::vector<std::string> &settings) {

  const bool reverseArcDirection =
      settings.size() >= 1 ? toBool(settings[0]) : false;

  switch (algorithmType) {
  case AlgorithmType::SimpleESTreeMultipleTreeArcs: {

    const unsigned nTreeArcs =
        settings.size() >= 2 ? (unsigned)std::stoi(settings[1]) : 2;
    if (nTreeArcs > 4 || nTreeArcs == 1) {
      throw std::runtime_error("invalid number of tree arcs");
    }
    return std::make_shared<SimpleESTreeMultipleTreeArcsSettings>(
        reverseArcDirection, nTreeArcs);
  }
  case AlgorithmType::SimpleESTreeTimeStampsFPM: {
    const bool preferOlder = settings.size() >= 2 ? toBool(settings[1]) : true;
    return std::make_shared<SimpleESTreeTimeStampsFPMSettings>(
        reverseArcDirection, preferOlder);
  }
  case AlgorithmType::ESTree:
    return std::make_shared<OldESTreeSettings>();
  case AlgorithmType::SimpleESTree:
    return std::make_shared<SimpleESTreeSettings>(reverseArcDirection);
  case AlgorithmType::SimpleESTreeSelectRandomMTE:
    return std::make_shared<SimpleESTReeSelectRandomSettings>(
        reverseArcDirection);
  case AlgorithmType::SimpleESTreeSelectRandomLCE:
    return std::make_shared<SimpleESTReeSelectRandomSettings>(
        reverseArcDirection);
  case AlgorithmType::SimpleESTreeSelectRandomSWCE:
    return std::make_shared<SimpleESTReeSelectRandomSettings>(
        reverseArcDirection);
  case AlgorithmType::SimpleESTreeReservoirSamplingMTE:
    return std::make_shared<SimpleESTReeReservoirSamplingSettings>(
        reverseArcDirection);
  case AlgorithmType::SimpleESTreeReservoirSamplingLCE:
    return std::make_shared<SimpleESTReeReservoirSamplingSettings>(
        reverseArcDirection);
  case AlgorithmType::SimpleESTreeReservoirSamplingSWCE:
    return std::make_shared<SimpleESTReeReservoirSamplingSettings>(
        reverseArcDirection);
  case AlgorithmType::SimpleESTreeTimeStamps: {
    bool preferOlder = settings.size() >= 2 ? toBool(settings[1]) : true;
    return std::make_shared<SimpleESTreeTimeStampsSettings>(reverseArcDirection,
                                                            preferOlder);
  }
  case AlgorithmType::ESTreeMLTimeStampsFPM:
    bool preferOlder = settings.size() >= 2 ? toBool(settings[1]) : true;
    return std::make_shared<ESTreeMLTimeStampsSettings>(reverseArcDirection,
                                                        preferOlder);
  }
  throw std::runtime_error("unknown algorithm type!");
}