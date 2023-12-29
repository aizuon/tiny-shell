#include "pch.hpp"
#include "commands/grep.hpp"

#include <iostream>
#include <regex>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void grep_t::setup_options_description()
{
    options_description->add_options()
            ("pattern", po::value<std::string>(&_pattern), "pattern to search for")
            ("input", po::value<std::string>(&_input), "input string to search");
}

void grep_t::setup_positional_options_description()
{
    positional_options_description->add("pattern", 1);
    positional_options_description->add("input", -1);
}

int grep_t::execute()
{
    auto& ostream = get_ostream();

    if (_pattern.empty() || _input.empty())
    {
        std::cerr << "Error: Pattern or input string is empty.\n";
        std::cerr.flush();
        return 1;
    }

    std::regex pattern_regex;
    try
    {
        pattern_regex = std::regex(_pattern);
    }
    catch (std::regex_error& e)
    {
        std::cerr << "Error: Invalid regular expression pattern.\n";
        std::cerr.flush();
        return 1;
    }

    std::istringstream input_stream(_input);
    std::string line;
    while (std::getline(input_stream, line))
    {
        if (std::regex_search(line, pattern_regex))
        {
            ostream << line << "\n";
        }
    }

    ostream.flush();
    return 0;
}
