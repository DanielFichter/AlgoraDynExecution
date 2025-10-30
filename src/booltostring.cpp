#include "booltostring.hpp"

std::string toString(bool value)
{
    return value ? "true" : "false";
}

bool toBool(const std::string& string) {
    return string == "true";
}