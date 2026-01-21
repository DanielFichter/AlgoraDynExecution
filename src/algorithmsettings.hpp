#pragma once

#include <iostream>
#include <memory>
#include <string>

struct AlgorithmSettings {
  AlgorithmSettings() = default;

  virtual std::string print() const;
  virtual ~AlgorithmSettings() = default;
};

struct SimpleESTreeSettings : public AlgorithmSettings {
  explicit SimpleESTreeSettings(bool reverseArcDirection)
      : reverseArcDirection(reverseArcDirection) {}
  bool reverseArcDirection = false;

  std::string print() const override;
};

struct SimpleESTReeSelectRandomSettings : public AlgorithmSettings {
  explicit SimpleESTReeSelectRandomSettings(bool reverseArcDirection)
      : reverseArcDirection(reverseArcDirection) {}

  bool reverseArcDirection = false;
  std::string print() const override;
};

struct SimpleESTReeReservoirSamplingSettings : public AlgorithmSettings {
  explicit SimpleESTReeReservoirSamplingSettings(bool reverseArcDirection)
      : reverseArcDirection(reverseArcDirection) {}
  bool reverseArcDirection = false;

  std::string print() const override;
};

struct SimpleESDAGSettings : public AlgorithmSettings {
  explicit SimpleESDAGSettings(bool reverseArcDirectoin, unsigned nTreeArcs = 2)
      : reverseArcDirection(reverseArcDirectoin), nTreeArcs(nTreeArcs) {}
  bool reverseArcDirection = false;
  unsigned nTreeArcs = 2;

  std::string print() const override;
};

struct OldESTreeSettings : public AlgorithmSettings {};

struct SimpleIncrementalSettings : public AlgorithmSettings {};

struct SimpleESTreeTimeStampsFPMSettings : public AlgorithmSettings {
  SimpleESTreeTimeStampsFPMSettings(bool reverseArcDirection, bool preferOlder)
      : reverseArcDirection(reverseArcDirection), preferOlder(preferOlder) {}

  bool reverseArcDirection = false;
  bool preferOlder = true;

  std::string print() const override;
};

struct SimpleESTreeTimeStampsSettings : public AlgorithmSettings {
  SimpleESTreeTimeStampsSettings(bool reverseArcDirection, bool preferOlder)
      : reverseArcDirection(reverseArcDirection), preferOlder(preferOlder) {}

  bool reverseArcDirection = false;
  bool preferOlder = true;

  std::string print() const override;
};

struct ESTreeMLTimeStampsSettings : public AlgorithmSettings {
  ESTreeMLTimeStampsSettings(bool reverseArcDirection, bool preferOlder)
      : reverseArcDirection(reverseArcDirection), preferOlder(preferOlder) {}

  bool reverseArcDirection = false;
  bool preferOlder = true;

  std::string print() const override;
};

std::ostream &operator<<(std::ostream &o,
                         const AlgorithmSettings &algorithmSettings);
