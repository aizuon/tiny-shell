#include "pch.hpp"
#include "commands/touch.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void touch_t::setup_options_description()
{
    options_description->add_options()
            ("path", po::value<std::string>(&_path), "specify the path of the file to touch");
}

void touch_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
}

int touch_t::execute()
{
    if (_path.empty())
    {
        std::cerr << "Error: No path specified for touch command.\n";
        std::cerr.flush();
        return 1;
    }

    const auto file_path = fs::weakly_canonical(state_t::current_path / fs::path(_path));

    if (fs::exists(file_path))
    {
        std::error_code ec;
        fs::last_write_time(file_path, std::filesystem::file_time_type::clock::now(), ec);
        if (ec)
        {
            std::cerr << "Error: Unable to update file times.\n";
            std::cerr.flush();
            return 1;
        }
    }
    else
    {
        std::ofstream(file_path).close();
    }

    return 0;
}
