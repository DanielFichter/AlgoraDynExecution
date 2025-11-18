#include "randomgraphinstantiator.hpp"

#include <graph.dyn.generator/randomdynamicdigraphgenerator.h>
#include <graph.dyn/dynamicdigraph.h>

DynamicDiGraph RandomGraphInstantiator::instantiate() {
  RandomDynamicDiGraphGenerator randomDynamicDiGraphGenerator;
  randomDynamicDiGraphGenerator.setInitialGraphSize(n);
  randomDynamicDiGraphGenerator.setInitialArcSize(m);
  randomDynamicDiGraphGenerator.setNumOperations(nOperations);
  randomDynamicDiGraphGenerator.setArcAdditionProportion(proportionInsertions);
  randomDynamicDiGraphGenerator.setArcRemovalProportion(proportionDeletions);
  randomDynamicDiGraphGenerator.setMultiplier(multiplier);

  DynamicDiGraph result;
  randomDynamicDiGraphGenerator.provideDynamicDiGraph(&result);
  return result;
}

RandomGraphInstantiator::RandomGraphInstantiator(size_type n, size_type m,
                                                 size_type nOperations,
                                                 unsigned proportionInsertions,
                                                 unsigned proportionDeletions,
                                                 unsigned batchSize)
    : n{n}, m{m}, nOperations{nOperations},
      proportionInsertions{proportionInsertions},
      proportionDeletions{proportionDeletions}, multiplier{batchSize} {}