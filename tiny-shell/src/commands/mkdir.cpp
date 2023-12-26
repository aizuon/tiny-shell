#include "pch.hpp"
#include "commands/mkdir.hpp"
#include "state.hpp"

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void mkdir_t::setup_options_description()
{
    options_description->add_options()
            ("path", po::value<std::string>(&_directory_path), "specify the directory path to create");
}

void mkdir_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
}

int mkdir_t::execute()
{
    if (_directory_path.empty())
    {
        std::cerr << "Error: No directory path specified.\n";
        std::cerr.flush();
        return 1;
    }

    auto dir_path = fs::weakly_canonical(state_t::current_path / fs::path(_directory_path));

    try
    {
        if (!fs::create_directories(dir_path))
        {
            std::cerr << "Error: Directory already exists or cannot be created: " << dir_path << "\n";
            std::cerr.flush();
            return 1;
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr.flush();
        return 1;
    }

    std::cout.flush();
    return 0;
}
