#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "operationtype.hpp"

#include <nlohmann/json.hpp>

#include <random>

using json = nlohmann::json;

class Settings;
class AlgorithmSettings;
class GraphInstantiator;
class OperationStatistics;

class PerformanceMeasurer : public AlgorithmExecuter {
public:
  PerformanceMeasurer(GraphInstantiator &, AlgorithmType,
                      const AlgorithmSettings &, unsigned iterationCount,
                      json &outerJson, double queryRatio);
  void execute() override;

private:
  unsigned iterationCount;
  size_t operationIndex = 0;
  json &outerJson;
  AlgorithmType algorithmType;
  const AlgorithmSettings &algorithmSettings;
  const double queryRatio;
  void incrementOperationIndex(size_t partOperations, size_t nOperations);
  bool applyNextOperationAndMeasure(
      std::map<OperationType, OperationStatistics> &operationDurations);
      void queryAndMeasure(
    std::map<OperationType, OperationStatistics> &operationDurations,
    std::mt19937 &randomEngine);
};

void measurePerformance(const Settings &settings);