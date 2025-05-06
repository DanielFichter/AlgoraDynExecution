#pragma once

#include "settings.hpp"

#include <string>
#include <vector>

class CLI
{

public:
    CLI();
    Settings parseSettings(const std::vector<std::string>& optionInput);
    void printStartInfo() const;

private:
    Settings settings;
};