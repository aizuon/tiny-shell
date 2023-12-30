#include "pch.hpp"
#include "commands/chmod.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void chmod_t::setup_options_description()
{
    options_description->add_options()
            ("mode,m", po::value<std::string>(&_mode), "mode to set")
            ("path", po::value<std::string>(&_path), "path of the file to change mode");
}

void chmod_t::setup_positional_options_description()
{
    positional_options_description->add("mode", 1);
    positional_options_description->add("path", 1);
}

int chmod_t::execute()
{
    if (_mode.empty() || _path.empty())
    {
        std::cerr << "Error: Mode and path must be specified.\n";
        std::cerr.flush();
        return 1;
    }

    const auto path_to_chmod = fs::weakly_canonical(state_t::current_path / fs::path(_path));
    if (!fs::exists(path_to_chmod))
    {
        std::cerr << "Error: Path does not exist.\n";
        std::cerr.flush();
        return 1;
    }

    const auto mode = static_cast<mode_t>(std::stoi(_mode, nullptr, 8));
    if (chmod(path_to_chmod.c_str(), mode) != 0)
    {
        std::cerr << "Error: Unable to change file mode.\n";
        std::cerr.flush();
        return 1;
    }

    return 0;
}
