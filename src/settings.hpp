#pragma once

#include "algorithmtype.hpp"
#include "executionmode.hpp"
#include "algorithmsettings.hpp"

#include <string>
#include <vector>
#include <memory>
#include <utility>

struct Settings {
  using AlgorithmInfo = std::pair<AlgorithmType, std::shared_ptr<AlgorithmSettings>>;
  bool preventPaging = false;
  std::vector<std::string> graphNames = {};
  unsigned iterationCount = 1;
  std::vector<AlgorithmInfo> algorithmInfos = {};
  ExecutionMode executionMode = ExecutionMode::measurePerformance;
  std::string outputPath = "";
};