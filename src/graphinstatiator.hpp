#pragma once

#include "graph.dyn/dynamicdigraph.h"

using namespace Algora;

class GraphInstantiator {
public:
  virtual DynamicDiGraph instantiate() = 0;
  virtual ~GraphInstantiator() = default;
};