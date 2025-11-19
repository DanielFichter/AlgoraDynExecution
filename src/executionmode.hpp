#pragma once

#include <map>
#include <string>

enum class ExecutionMode {
  measurePerformance,
  testCorrectness,
  unitTest,
  analyzeGraphs
};

const std::map<ExecutionMode, std::string> executionModeNames{
    {ExecutionMode::measurePerformance, "measurePerformance"},
    {ExecutionMode::testCorrectness, "testCorrectness"},
    {ExecutionMode::unitTest, "unitTest"},
    {ExecutionMode::analyzeGraphs, "analyzeGraphs"}};
