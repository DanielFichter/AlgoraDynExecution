#include "algorithmsettings.hpp"
#include "booltostring.hpp"

#include <string>

using namespace std::string_literals;

std::ostream& operator<<(std::ostream& o, const AlgorithmSettings& algorithmSettings) {
    return o << algorithmSettings.print();
}

std::string AlgorithmSettings::print() const {
    return "";
}

std::string SimpleESTreeSettings::print() const {
    return "<"s + toString(reverseArcDirection) + ">";
}

std::string SimpleESTReeSelectRandomSettings::print() const {
    return "<"s + toString(reverseArcDirection) + ">";
}

std::string SimpleESTReeReservoirSamplingSettings::print() const {
    return "<"s + toString(reverseArcDirection) + ">";
}

std::string SimpleESDAGSettings::print() const {
    return "<"s + toString(reverseArcDirection) + ", "s + std::to_string(nTreeArcs) + ">";
}

std::string SimpleESTreeTimeStampsSettings::print() const {
    return "<"s + toString(reverseArcDirection) + ", "s + toString(preferOlder) + ">";
}

std::string ESTreeMLTimeStampsSettings::print() const {
    return "<"s + toString(reverseArcDirection) + ", "s + toString(preferOlder) + ">";
}