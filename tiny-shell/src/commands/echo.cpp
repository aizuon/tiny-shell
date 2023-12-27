#include "pch.hpp"
#include "commands/echo.hpp"

#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void echo_t::setup_options_description()
{
    options_description->add_options()
            ("text", po::value<std::string>(&_text), "specify the text to echo")
            ("no-newline,n",
             po::bool_switch(&_newline)->default_value(true),
             "do not output the trailing newline");
}

void echo_t::setup_positional_options_description()
{
    positional_options_description->add("text", -1);
}

int echo_t::execute()
{
    std::cout << _text;
    if (_newline)
    {
        std::cout << "\n";
    }

    std::cout.flush();
    return 0;
}
