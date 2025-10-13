#include "cli.hpp"
#include "algorithmtype.hpp"
#include "executionmode.hpp"

#include <string>
#include <stdexcept>
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
            settings.graphNames.push_back(optionValueString);
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
                    settings.algorithmTypes.push_back({algorithmType});
                    validAlgorithmType = true;
                    break;
                }
            }
            if (!validAlgorithmType)
            {
                throw std::invalid_argument("invalid algorithm \""s + optionValueString + "\"");
            }
        }
        else if (optionName == "executionMode")
        {
            bool validExecutionMode = false;
            for (const auto& [executionMode, name]: executionModeNames)
            {
                if (optionValueString == name)
                {
                    settings.executionMode = executionMode;
                    validExecutionMode = true;
                    break;
                }
            }
            if (!validExecutionMode)
            {
                throw std::invalid_argument("invalid execution mode \""s + optionValueString + "\"");
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
    switch (settings.executionMode)
    {
    case ExecutionMode::measurePerformance:
        std::cout << "measuring performance for ";
        break;
    case ExecutionMode::testCorrectness:
        std::cout << "testing";
        break;
    case ExecutionMode::unitTest:
        std::cout << "unit testing";
        break;
    default:
        throw std::logic_error("invalid execution mode!");
    }
    std::cout << "algorithms [";
    for (const AlgorithmType algoirthmType : settings.algorithmTypes)
    {
        std::cout << AlgorithmTypeNames.at(algoirthmType) << ",";
    }
    std::cout << "] on graphs [";
    for (const std::string& graphName: settings.graphNames)
    {
        std::cout << graphName << ",";
    }
    std::cout << " " << settings.iterationCount << " time" << (settings.iterationCount > 1 ? "s" : "")
    << (settings.preventPaging ? ", while preventing paging to swap area" : "")
    << std::endl;
}
