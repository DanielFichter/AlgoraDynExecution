#pragma once

#include <map>
#include <string>

enum class ExecutionMode {
  measurePerformance,
  testCorrectness,
  unitTest,
};

const std::map<ExecutionMode, std::string> executionModeNames{
    {ExecutionMode::measurePerformance, "measurePerformance"},
    {ExecutionMode::testCorrectness, "testCorrectness"},
    {ExecutionMode::unitTest, "unitTest"}};
