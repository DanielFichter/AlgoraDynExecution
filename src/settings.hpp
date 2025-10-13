#pragma once

#include "algorithmtype.hpp"
#include "executionmode.hpp"

#include <string>
#include <vector>

struct Settings {
  bool preventPaging = false;
  std::string graphPath = "kronecker/csize/answers";
  unsigned iterationCount = 1;
  std::vector<AlgorithmType> algorithmTypes = {};
  ExecutionMode executionMode = ExecutionMode::measurePerformance;
};