
#include "algoracli.hpp"
#include "createalgorithmsettings.hpp"
#include "executionmode.hpp"
#include "measureperformance.hpp"
#include "settings.hpp"
#include "testcorrectness.hpp"
#include "unit_test.hpp"

#include <CLI/CLI.hpp>

#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <vector>

using namespace Algora;
using namespace std::string_literals;

int main(int argc, char *argv[]) {
  AlgoraCLI cli;
  CLI::App& app = cli.createApp();
  
  CLI11_PARSE(app, argc, argv);
  Settings settings = cli.parseSettings();
  
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
  default:
    std::cerr << "invalid execution mode!" << std::endl;
    break;
  }

  if (settings.preventPaging) {
    munlockall();
  }

  return 0;
}