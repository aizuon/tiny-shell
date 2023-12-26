#include "pch.hpp"
#include "commands/cd.hpp"
#include "state.hpp"

#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void cd_t::setup_options_description()
{
    options_description->add_options()
            ("path", po::value<std::string>(&_path), "specify the path to list");
}

void cd_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
}

int cd_t::execute()
{
    state_t::current_path = !_path.empty()
                                ? fs::weakly_canonical(state_t::current_path / fs::path(_path))
                                : state_t::current_path;

    return 0;
}
