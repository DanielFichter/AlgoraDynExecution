#pragma once

#include <chrono>

class OperationStatistics {
public:
  void addOccurnece(const std::chrono::nanoseconds &duration) {
    nOccurences++;
    timeSpent += duration;
  }
  std::chrono::nanoseconds getAverageDuration() const {
    return timeSpent / nOccurences;
  }
  void reset()
  {
    timeSpent = std::chrono::nanoseconds{0};
    nOccurences = 0;
  }

private:
  std::chrono::nanoseconds timeSpent{0};
  long long unsigned nOccurences = 0;
};