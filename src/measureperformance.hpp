#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Settings;
class AlgorithmSettings;
class GraphInstantiator;

class PerformanceMeasurer : public AlgorithmExecuter {
public:
  PerformanceMeasurer(GraphInstantiator &, AlgorithmType,
                      const AlgorithmSettings&, unsigned iterationCount,
                      json &outerJson, double queryRatio);
  void execute() override;

private:
  unsigned iterationCount;
  size_t operationIndex = 0;
  json &outerJson;
  AlgorithmType algorithmType;
  const AlgorithmSettings& algorithmSettings;
  const double queryRatio;
  void incrementOperationIndex(size_t partOperations, size_t nOperations);
};

void measurePerformance(const Settings &settings);