
#include "algoracli.hpp"
#include "analyzedynamicdigraph.hpp"
#include "createalgorithmsettings.hpp"
#include "executionmode.hpp"
#include "measureperformance.hpp"
#include "settings.hpp"
#include "testcorrectness.hpp"
#include "unit_test.hpp"

#include <CLI/CLI.hpp>

#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <vector>

using namespace Algora;
using namespace std::string_literals;

int main(int argc, char *argv[]) {
  AlgoraCLI cli;

  Settings settings;
  try {
    settings = std::move(cli.parseSettings(argc, argv));
  } catch (const std::exception &parseError) {
    std::cerr << "error occured while parsing user input: " << parseError.what()
              << std::endl;
    return EXIT_FAILURE;
  }

  if (settings.preventPaging) {
    mlockall(MCL_CURRENT | MCL_FUTURE);
  }

  switch (settings.executionMode) {
  case ExecutionMode::measurePerformance:
    measurePerformance(settings);
    break;
  case ExecutionMode::testCorrectness:
    testCorrectness(settings);
    break;
  case ExecutionMode::unitTest:
    unit_test(settings.algorithmInfos);
    break;
  case ExecutionMode::analyzeGraphs:
    analyzeDynamicDiGraphs(settings);
    break;
  default:
    std::cerr << "invalid execution mode!" << std::endl;
    break;
  }

  if (settings.preventPaging) {
    munlockall();
  }

  return 0;
}