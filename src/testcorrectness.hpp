#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmtype.hpp"

class Settings;

void testCorrectness(const Settings &settings);

class CorrectnessTester : public AlgorithmExecuter {
public:
  CorrectnessTester(const std::string &graphName, AlgorithmType);
  void execute() override;
};
