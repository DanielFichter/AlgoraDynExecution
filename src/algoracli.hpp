#pragma once

#include "settings.hpp"

class AlgoraCLI {

public:
  AlgoraCLI();
  void printStartInfo() const;

private:
  Settings settings;
};