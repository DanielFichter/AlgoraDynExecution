#pragma once

#include "settings.hpp"

#include <CLI/CLI.hpp>
#include <vector>

class AlgoraCLI {

public:
  AlgoraCLI();
  CLI::App& createApp();
  void printStartInfo() const;
  const Settings& parseSettings();
  std::string executionModeName;
  std::vector<std::string> algorithmInfoStrings;

private:
  Settings settings;
  CLI::App app{"Execute Algorithms from AlgoraDyn"};
};