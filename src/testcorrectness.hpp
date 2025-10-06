#pragma once

#include "algorithmexecuter.hpp"
#include "algorithmtype.hpp"

class CorrectnessTester : public AlgorithmExecuter {
public:
  CorrectnessTester(const std::string &graphName, AlgorithmType);
  void execute() override;
};
