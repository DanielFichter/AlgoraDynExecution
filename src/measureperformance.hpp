#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmtype.hpp"

class PerformanceMeasurer : public AlgorithmExecuter {
public:
  PerformanceMeasurer(const std::string &graphName, AlgorithmType,
                      unsigned iterationCount);
  void execute() override;

private:
  unsigned iterationCount;
};
