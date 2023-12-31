#include "command.hpp"

#include <fmt/core.h>

void command_t::initialize()
{
    if (_istream != nullptr)
    {
        _istream->clear();
        _istream->seekg(0, std::ios::beg);
        _arguments += fmt::format(" \"{}\"", _istream->str());
    }
    setup_options_description();
    setup_positional_options_description();
    setup_variables_map(preprocess_arguments(_arguments));
}

void command_t::setup_options_description()
{
    options_description->add_options()("help,h", po::bool_switch(&help),
                                       "produce help message");
}

void command_t::setup_positional_options_description()
{
}

void command_t::setup_variables_map(const std::string& arguments)
{
    po::store(po::command_line_parser(po::split_unix(arguments))
              .options(*options_description).positional(*positional_options_description)
              .run(),
              *variables_map);
    po::notify(*variables_map);
}
