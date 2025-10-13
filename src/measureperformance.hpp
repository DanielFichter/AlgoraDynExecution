#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmtype.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Settings;

class PerformanceMeasurer : public AlgorithmExecuter {
public:
  PerformanceMeasurer(const std::string &graphName, AlgorithmType,
                      unsigned iterationCount, json& outerJson);
  void execute() override;

private:
  unsigned iterationCount;
  size_t operationIndex = 0;
  json& outerJson;
  AlgorithmType algorithmType;
};

void measurePerformance(const Settings &settings);