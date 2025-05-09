#pragma once

#include "graph.dyn/dynamicdigraph.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

using namespace std::string_literals;

Algora::DynamicDiGraph readKroneckerGraph(const std::string &filename)
{
    if (!std::filesystem::exists(std::filesystem::path{filename}))
    {
        throw std::runtime_error("file \""s + filename + "\" does not exist!");
    }
    Algora::DynamicDiGraph dynamicGraph;
    Algora::KonectNetworkReader graphReader;
    std::ifstream graphInput{filename};
    graphReader.setInputStream(&graphInput);
    graphReader.provideDynamicDiGraph(&dynamicGraph);
    return dynamicGraph;
}