#include "cli.hpp"

#include <sstream>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <iostream>

using namespace std::string_literals;
namespace
{
    void setOption(Settings &settings, const std::string &optionName, const std::string optionValueString)
    {
        if (optionName == "preventPaging")
        {
            if (optionValueString == "true")
            {
                settings.preventPaging = true;
            }
            else if (optionValueString == "false")
            {
                settings.preventPaging = false;
            }
            else
            {
                throw std::invalid_argument("invalid value \""s + optionValueString + "\" for option \""s + optionName + "\"");
            }
        }
        else if (optionName == "graph")
        {
            settings.graphPath = optionValueString;
        }
        else if (optionName == "iterations")
        {
            settings.iterationCount = std::stoul(optionValueString);
        }
        else if (optionName == "algorithm")
        {
            bool validAlgorithmType = false;
            for (const auto& [algorithmType, name]: AlgorithmTypeNames)
            {
                if (optionValueString == name)
                {
                    settings.algorithmType = algorithmType;
                    validAlgorithmType = true;
                    break;
                }
            }
            if (!validAlgorithmType)
            {
                throw std::invalid_argument("invalid algorithm \""s + optionValueString + "\"");
            }
        }
        else
        {
            throw std::invalid_argument("invalid option \"" + optionName + "\"");
        }
    }
}

CLI::CLI()
{
    std::cout << std::boolalpha;
}

Settings CLI::parseSettings(const std::vector<std::string> &optionInput)
{
    Settings result;

    for (const auto& currentOptionString: optionInput)
    {
        const size_t equalsPosition = currentOptionString.find("=");
        if (equalsPosition == std::string::npos)
        {
            throw std::runtime_error("invalid syntax: missing \"=\"");
        }
        const std::string optionName = currentOptionString.substr(0, equalsPosition);
        const std::string optionValueString = currentOptionString.substr(equalsPosition + 1);
        setOption(result, optionName, optionValueString);
    }

    settings = result;

    return result;
}

void CLI::printStartInfo() const
{
    std::cout << "running " << AlgorithmTypeNames.at(settings.algorithmType) << " on graph " << settings.graphPath << " "
    << settings.iterationCount << " time" << (settings.iterationCount > 1 ? "s" : "")
    << (settings.preventPaging ? ", while preventing paging to swap area" : "")
    << std::endl;
}
