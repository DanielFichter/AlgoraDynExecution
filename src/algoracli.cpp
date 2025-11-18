#include "algoracli.hpp"
#include "algorithmsettings.hpp"
#include "algorithmtype.hpp"
#include "createalgorithmsettings.hpp"
#include "executionmode.hpp"
#include "graphreader.hpp"
#include "randomgraphinstantiator.hpp"
#include "settings.hpp"

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

std::unique_ptr<RandomGraphInstantiator>
parseRandomGraph(const std::string &graphDescription) {
  std::string parameterString = graphDescription.substr(
      7, graphDescription.size() - 1); // remove "random(" and ")"
  const auto itRemove =
      std::remove(parameterString.begin(), parameterString.end(), ' ');
  parameterString.erase(itRemove, parameterString.end());

  size_t commaIndex = parameterString.find(',');
  std::string nString = parameterString.substr(0, commaIndex);
  parameterString.erase(0, commaIndex + 1);
  commaIndex = parameterString.find(',');
  std::string mString = parameterString.substr(0, commaIndex);
  parameterString.erase(0, commaIndex + 1);
  commaIndex = parameterString.find(',');
  std::string nOperationsString = parameterString.substr(0, commaIndex);
  parameterString.erase(0, commaIndex + 1);
  commaIndex = parameterString.find(',');
  std::string proportionInsertionsString =
      parameterString.substr(0, commaIndex);
  parameterString.erase(0, commaIndex + 1);
  commaIndex = parameterString.find(',');
  std::string proportionDeletionsString = parameterString.substr(0, commaIndex);
  parameterString.erase(0, commaIndex + 1);
  commaIndex = parameterString.find(',');
  std::string multiplierString = parameterString.substr(0, commaIndex);

  const RandomGraphInstantiator::size_type n = std::stoul(nString);
  const RandomGraphInstantiator::size_type m = std::stoul(mString);
  const RandomGraphInstantiator::size_type nOperations =
      std::stoull(nOperationsString);
  const auto proportionInsertions =
      static_cast<unsigned>(std::stoul(proportionInsertionsString));
  const auto proportionDeletions =
      static_cast<unsigned>(std::stoul(proportionDeletionsString));
  const auto batchSize = static_cast<unsigned>(std::stoul(multiplierString));

  return std::make_unique<RandomGraphInstantiator>(
      n, m, nOperations, proportionInsertions, proportionDeletions, batchSize);
}

std::unique_ptr<GraphInstantiator>
parseGraphInstantiator(const std::string &graphDescription) {
  if (graphDescription.find("random(") == 0) {
    return parseRandomGraph(graphDescription);
  }
  return std::make_unique<GraphReader>(graphDescription);
}

Settings::GraphInfo parseGraphInfo(const std::string &graphDescription) {
  return Settings::GraphInfo(
      graphDescription, std::move(parseGraphInstantiator(graphDescription)));
}

std::vector<Settings::GraphInfo>
parseGraphInfos(const std::vector<std::string> &graphDescriptions) {
  std::vector<Settings::GraphInfo> result;
  for (const auto &graphDescription : graphDescriptions) {
    result.emplace_back(parseGraphInfo(graphDescription));
  }
  return result;
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
  app.add_option_function<std::vector<std::string>>(
      "-g,--graphs",
      [this](const std::vector<std::string> &graphDescriptions) {
        settings.graphInfos = parseGraphInfos(graphDescriptions);
      },
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
  app.add_option("-q,--queryRatio", settings.queryRatio,
                 "Ratio of queries among operations");
}

Settings &&AlgoraCLI::parseSettings(int argc, char *argv[]) {
  initializeApp();
  app.parse(argc, argv);
  return std::move(settings);
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
  for (const auto &[graphDescription, graphInstantiator] :
       settings.graphInfos) {
    std::cout << graphDescription << ",";
  }
  std::cout << "] " << settings.iterationCount << " time"
            << (settings.iterationCount > 1 ? "s" : "")
            << (settings.preventPaging
                    ? ", while preventing paging to swap area"
                    : "")
            << std::endl;
}