#include "algoracli.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "createalgorithmsettings.hpp"
#include "executionmode.hpp"

#include <CLI/CLI.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std::string_literals;
namespace {

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

} // namespace

AlgoraCLI::AlgoraCLI() { std::cout << std::boolalpha; }

void AlgoraCLI::initializeApp() {
  app.add_option_function<std::vector<std::string>>(
      "-a,--algorithms",
      [this](const std::vector<std::string> &algorithmInfoStrings) {
        settings.algorithmInfos = parseAlgorithmInfos(algorithmInfoStrings);
      },
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
  app.add_option_function<std::string>(
      "-e,--execution-mode",
      [this](const std::string &executionModeName) {
        settings.executionMode = parseExecutionMode(executionModeName);
      },
      "Tells whether the algorithm should be tested for correctness, measured "
      "by performanc or unit tested with tree dumps");
}

const Settings &AlgoraCLI::parseSettings(int argc, char *argv[]) {
  initializeApp();
  app.parse(argc, argv);
  return settings;
}

void AlgoraCLI::printStartInfo() const {
  switch (settings.executionMode) {
  case ExecutionMode::measurePerformance:
    std::cout << "measuring performance for ";
    break;
  case ExecutionMode::testCorrectness:
    std::cout << "testing ";
    break;
  case ExecutionMode::unitTest:
    std::cout << "unit testing ";
    break;
  default:
    throw std::logic_error("invalid execution mode!");
  }
  std::cout << "algorithms [";
  for (const auto &[algorithmType, algorithmSettings] :
       settings.algorithmInfos) {
    std::cout << AlgorithmTypeNames.at(algorithmType) << *algorithmSettings
              << ", ";
  }
  std::cout << "] on graphs [";
  for (const std::string &graphName : settings.graphNames) {
    std::cout << graphName << ",";
  }
  std::cout << "] " << settings.iterationCount << " time"
            << (settings.iterationCount > 1 ? "s" : "")
            << (settings.preventPaging
                    ? ", while preventing paging to swap area"
                    : "")
            << std::endl;
}