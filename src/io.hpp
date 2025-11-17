#pragma once

#include "graph.dyn/dynamicdigraph.h"
#include "io/konectnetworkreader.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

using namespace std::string_literals;

Algora::DynamicDiGraph readGraph(const std::string &filename);