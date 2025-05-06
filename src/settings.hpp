#pragma once

#include "algorithmtype.hpp"

#include <string>

struct Settings
{
    bool preventPaging = false;
    std::string graphPath = "kronecker/csize/answers";
    unsigned iterationCount = 1;
    AlgorithmType algorithmType = AlgorithmType::SimpleESTree;
};