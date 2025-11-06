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

using namespace std::string_literals;
namespace {


void setOption(Settings &settings, const std::string &optionName,
               const std::string optionValueString) {
  if (optionName == "preventPaging") {
    if (optionValueString == "true") {
      settings.preventPaging = true;
    } else if (optionValueString == "false") {
      settings.preventPaging = false;
    } else {
      throw std::invalid_argument("invalid value \""s + optionValueString +
                                  "\" for option \""s + optionName + "\"");
    }
  } else if (optionName == "graph") {
    settings.graphNames.push_back(optionValueString);
  } else if (optionName == "iterations") {
    settings.iterationCount = std::stoul(optionValueString);
  } else if (optionName == "algorithm") {
    
  } else if (optionName == "executionMode") {
    bool validExecutionMode = false;
    for (const auto &[executionMode, name] : executionModeNames) {
      if (optionValueString == name) {
        settings.executionMode = executionMode;
        validExecutionMode = true;
        break;
      }
    }
    if (!validExecutionMode) {
      throw std::invalid_argument("invalid execution mode \""s +
                                  optionValueString + "\"");
    }
  } else if (optionName == "o") {
    settings.outputPath = optionValueString;
  } else {
    throw std::invalid_argument("invalid option \"" + optionName + "\"");
  }
}
} // namespace

AlgoraCLI::AlgoraCLI() { std::cout << std::boolalpha; }



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