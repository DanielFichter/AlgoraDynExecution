#include "cli.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "createalgorithmsettings.hpp"
#include "executionmode.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

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
    size_t templateBegin = optionValueString.find("<");
    AlgorithmType algorithmType;
    std::shared_ptr<AlgorithmSettings> algorithmSettings;
    if (templateBegin == std::string::npos) {
      algorithmType = parseAlgorithmType(optionValueString);
      algorithmSettings = create(algorithmType, {});
    } else {
      size_t templeteEnd = optionValueString.find('>');
      if (templeteEnd == std::string::npos) {
        throw std::logic_error("missing '>'");
      }
      std::string templateArgumentString = optionValueString.substr(
          templateBegin + 1, templeteEnd - templateBegin - 1);
          templateArgumentString.erase(std::remove(templateArgumentString.begin(), templateArgumentString.end(), ' '), templateArgumentString.end());
      std::stringstream templateArgumentStream{templateArgumentString};
      std::vector<std::string> templateArguments;
      for (std::string currentTemplateArgument; std::getline(
               templateArgumentStream, currentTemplateArgument, ',');) {
        templateArguments.push_back(currentTemplateArgument);
      }
      algorithmType =
          parseAlgorithmType(optionValueString.substr(0, templateBegin));
      algorithmSettings = create(algorithmType, templateArguments);
    }
    settings.algorithmInfos.emplace_back(algorithmType, algorithmSettings);
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

CLI::CLI() { std::cout << std::boolalpha; }

Settings CLI::parseSettings(const std::vector<std::string> &optionInput) {
  Settings result;

  for (const auto &currentOptionString : optionInput) {
    const size_t equalsPosition = currentOptionString.find("=");
    if (equalsPosition == std::string::npos) {
      throw std::runtime_error("invalid syntax: missing \"=\"");
    }
    const std::string optionName =
        currentOptionString.substr(0, equalsPosition);
    const std::string optionValueString =
        currentOptionString.substr(equalsPosition + 1);
    setOption(result, optionName, optionValueString);
  }

  settings = result;

  return result;
}

void CLI::printStartInfo() const {
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
  for (const auto& [algorithmType, algorithmSettings] : settings.algorithmInfos) {
    std::cout << AlgorithmTypeNames.at(algorithmType) << *algorithmSettings << ", ";
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