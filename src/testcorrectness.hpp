#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"

class Settings;
class AlgorithmSettings;

void testCorrectness(const Settings &settings);

class CorrectnessTester : public AlgorithmExecuter {
public:
  CorrectnessTester(const std::string &graphName, AlgorithmType, const AlgorithmSettings&);
  void execute() override;
};
