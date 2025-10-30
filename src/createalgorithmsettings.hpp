#pragma once

#include "algorithmtype.hpp"

#include <memory>
#include <string>
#include <vector>

struct AlgorithmSettings;

std::shared_ptr<AlgorithmSettings> create(AlgorithmType, const std::vector<std::string>& settings);