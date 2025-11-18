#pragma once

#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "executionmode.hpp"
#include "graphinstatiator.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Settings {

  Settings operator=(const Settings &) = delete;

  Settings& operator=(Settings&& other) {
    preventPaging = other.preventPaging;
    graphInfos = std::move(other.graphInfos);
    iterationCount = other.iterationCount;
    algorithmInfos = other.algorithmInfos;
    executionMode = other.executionMode;
    outputPath = other.outputPath;
    queryRatio = other.queryRatio;

    return *this;
  }

  using AlgorithmInfo =
      std::pair<AlgorithmType, std::shared_ptr<AlgorithmSettings>>;
  using GraphInfo = std::pair<std::string, std::unique_ptr<GraphInstantiator>>;
  bool preventPaging = false;
  std::vector<GraphInfo> graphInfos = {};
  unsigned iterationCount = 1;
  std::vector<AlgorithmInfo> algorithmInfos = {};
  ExecutionMode executionMode = ExecutionMode::measurePerformance;
  std::string outputPath = "";
  double queryRatio = .1; // the ratio of queries among operations
};