#include "graphreader.hpp"
#include "io.hpp"

GraphReader::GraphReader(const std::string &fileName) : fileName{fileName} {}

DynamicDiGraph GraphReader::instantiate() {
    return readGraph("graphs/" + fileName);
}