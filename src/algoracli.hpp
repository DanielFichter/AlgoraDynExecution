#pragma once

#include "settings.hpp"

#include <CLI/CLI.hpp>
#include <vector>

class AlgoraCLI {

public:
  AlgoraCLI();
  void printStartInfo() const;
  Settings&& parseSettings(int argc, char* argv[]);
  
  private:
  Settings settings;
  void initializeApp();
  CLI::App app{"Execute Algorithms from AlgoraDyn"};
};