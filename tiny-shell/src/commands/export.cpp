#include "pch.hpp"
#include "commands/export.hpp"

#include <stdlib.h>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void export_t::setup_options_description()
{
    options_description->add_options()
    ("variable", po::value<std::vector<std::string>>(&_variables)->multitoken()->zero_tokens()->composing(),
     "environment variables to set");
}

void export_t::setup_positional_options_description()
{
    positional_options_description->add("variable", -1);
}

int export_t::execute()
{
    for (const auto& kv: _variables)
    {
        const auto pos = kv.find('=');
        if (pos == std::string::npos)
        {
            std::cerr << "Error: Invalid variable " << kv << "\n";
            std::cerr.flush();
            return 1;
        }

        auto key = kv.substr(0, pos);
        auto value = kv.substr(pos + 1);

        if (setenv(key.c_str(), value.c_str(), 1) != 0)
        {
            std::cerr << "Error: Failed to set environment variable " << key << "\n";
            std::cerr.flush();
            return 1;
        }
    }

    return 0;
}
