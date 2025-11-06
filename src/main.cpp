
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

AlgorithmType parseAlgorithmType(const std::string &algorithmTypeName) {

  for (const auto &[algorithmType, name] : AlgorithmTypeNames) {
    if (algorithmTypeName == name) {
      return algorithmType;
      break;
    }
  }
  throw std::invalid_argument("invalid algorithm type\""s + algorithmTypeName +
                              "\"");
}

Settings::AlgorithmInfo
parseAlgorithmInfo(const std::string &algorithmInfoString) {
  size_t templateBegin = algorithmInfoString.find("<");
  AlgorithmType algorithmType;
  std::shared_ptr<AlgorithmSettings> algorithmSettings;
  if (templateBegin == std::string::npos) {
    algorithmType = parseAlgorithmType(algorithmInfoString);
    algorithmSettings = create(algorithmType, {});
  } else {
    size_t templeteEnd = algorithmInfoString.find('>');
    if (templeteEnd == std::string::npos) {
      throw std::logic_error("missing '>'");
    }
    std::string templateArgumentString = algorithmInfoString.substr(
        templateBegin + 1, templeteEnd - templateBegin - 1);
    templateArgumentString.erase(std::remove(templateArgumentString.begin(),
                                             templateArgumentString.end(), ' '),
                                 templateArgumentString.end());
    std::stringstream templateArgumentStream{templateArgumentString};
    std::vector<std::string> templateArguments;
    for (std::string currentTemplateArgument;
         std::getline(templateArgumentStream, currentTemplateArgument, ',');) {
      templateArguments.push_back(currentTemplateArgument);
    }
    algorithmType =
        parseAlgorithmType(algorithmInfoString.substr(0, templateBegin));
    algorithmSettings = create(algorithmType, templateArguments);
  }
  return {algorithmType, algorithmSettings};
}

std::vector<Settings::AlgorithmInfo>
parseAlgorithmInfos(const std::vector<std::string> &algorithmInfoStrings) {
  std::vector<Settings::AlgorithmInfo> result;
  for (const auto &algorithmInfoString : algorithmInfoStrings) {
    result.push_back(parseAlgorithmInfo(algorithmInfoString));
  }

  return result;
}

ExecutionMode parseExecutionMode(const std::string &executionModeName) {
  for (const auto &[executionMode, name] : executionModeNames) {
    if (executionModeName == name) {
      return executionMode;
    }
  }
  throw std::invalid_argument("invalid execution mode \""s + executionModeName +
                              "\"");
}

int main(int argc, char *argv[]) {
  AlgoraCLI cli;
  // const auto settings = cli.parseSettings(input);
  // cli.printStartInfo();

  Settings settings;

  CLI::App app{"Execute Algorithms from AlgoraDyn"};
  std::vector<std::string> algorithmInfoStrings;
  std::string executionModeName;
  app.add_option("-a,--algorithms", algorithmInfoStrings,
                 "the algorithms that should be executed");
  app.add_flag("-p,--prevent-paging", settings.preventPaging,
                 "If true, tries to keep all the used memory in RAM");
  app.add_option("-g,--graphs", settings.graphNames,
                 "the graphs on which the algorithms should be executed");
  app.add_option(
      "-c,--iterations", settings.iterationCount,
      "Tells how often each algorithm should be executed on each graph");
  app.add_option("-o,--output-path", settings.outputPath,
                 "Tells, where to write the measurements results in JSON "
                 "format. If omitted, the output will be written into cout");
  app.add_option(
      "-e,--execution-mode", executionModeName,
      "Tells whether the algorithm should be tested for correctness, measured "
      "by performanc or unit tested with tree dumps");

  CLI11_PARSE(app, argc, argv);

  settings.algorithmInfos = parseAlgorithmInfos(algorithmInfoStrings);
  if (!executionModeName.empty()) {
    settings.executionMode = parseExecutionMode(executionModeName);
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
  default:
    std::cerr << "invalid execution mode!" << std::endl;
    break;
  }

  if (settings.preventPaging) {
    munlockall();
  }

  return 0;
}