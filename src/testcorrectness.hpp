#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"

class Settings;
class AlgorithmSettings;
class GraphInstantiator;

void testCorrectness(const Settings &settings);

class CorrectnessTester : public AlgorithmExecuter {
public:
  CorrectnessTester(GraphInstantiator &, AlgorithmType, const AlgorithmSettings&);
  void execute() override;
};
