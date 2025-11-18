#pragma once

#include "graphinstatiator.hpp"

class RandomGraphInstantiator : public GraphInstantiator {
public:
  using size_type = DynamicDiGraph::size_type;
  RandomGraphInstantiator(size_type n, size_type m, size_type nOperations,
                          unsigned proportionInsertions,
                          unsigned proportionDeletions, unsigned batchSize);
  DynamicDiGraph instantiate() override;

private:
  size_type n;
  size_type m;
  size_type nOperations;
  unsigned proportionInsertions;
  unsigned proportionDeletions;
  unsigned multiplier;
};