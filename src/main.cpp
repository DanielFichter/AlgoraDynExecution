#include "cli.hpp"
#include "executionmode.hpp"
#include "measureperformance.hpp"
#include "settings.hpp"
#include "testcorrectness.hpp"

#include "unit_test.hpp"

#include <algorithm.reachability.ss/dynamicsinglesourcereachabilityalgorithm.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <vector>

using namespace Algora;
using namespace std::string_literals;

void measurePerformance(const Settings &settings) {
  PerformanceMeasurer performanceMeasurer{
      settings.graphPath, settings.algorithmType, settings.iterationCount};
  performanceMeasurer.execute();
}

void testCorrectness(const Settings &settings) {
  CorrectnessTester correctnessTester{settings.graphPath,
                                      settings.algorithmType};
  correctnessTester.execute();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> input{argv + 1, argv + argc};
  CLI cli;
  const auto settings = cli.parseSettings(input);
  cli.printStartInfo();

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
    unit_test(settings.algorithmType);
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