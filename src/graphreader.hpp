#pragma once

#include "graphinstatiator.hpp"

class GraphReader : public GraphInstantiator {
public:
  explicit GraphReader(const std::string &fileName);
  DynamicDiGraph instantiate() override;

private:
  std::string fileName;
};