#include "pch.hpp"
#include "commands/cp.hpp"
#include "state.hpp"

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void cp_t::setup_options_description()
{
    options_description->add_options()
            ("recursive,r", po::bool_switch(&_recursive), "copy directories and their contents recursively")
            ("overwrite,o", po::bool_switch(&_overwrite), "overwrite files if they already exist")
            ("source", po::value<std::string>(&_source), "specify the source file/directory path")
            ("destination", po::value<std::string>(&_destination), "specify the destination file/directory path");
}

void cp_t::setup_positional_options_description()
{
    positional_options_description->add("source", 1);
    positional_options_description->add("destination", 1);
}

int cp_t::execute()
{
    if (_source.empty() || _destination.empty())
    {
        std::cerr << "Error: Source and destination must be specified.\n";
        std::cerr.flush();
        return 1;
    }

    const auto source_path = fs::weakly_canonical(state_t::current_path / fs::path(_source));
    const auto destination_path = fs::weakly_canonical(state_t::current_path / fs::path(_destination));

    try
    {
        if (!fs::exists(source_path))
        {
            std::cerr << "Error: Source does not exist: " << source_path << "\n";
            std::cerr.flush();
            return 1;
        }

        if (fs::is_directory(source_path) && !_recursive)
        {
            std::cerr << "Error: Source is a directory, use -r or --recursive to copy directories.\n";
            std::cerr.flush();
            return 1;
        }

        if (fs::exists(destination_path) && !_overwrite)
        {
            std::cerr << "Error: Destination already exists, use -o or --overwrite to overwrite files.\n";
            std::cerr.flush();
            return 1;
        }

        if (_recursive && fs::is_directory(source_path))
        {
            const auto options = _overwrite
                               ? fs::copy_options::overwrite_existing | fs::copy_options::recursive
                               : fs::copy_options::recursive;
            fs::copy(source_path, destination_path, options);
        }
        else
        {
            const auto options = _overwrite
                               ? fs::copy_options::overwrite_existing
                               : fs::copy_options::none;
            fs::copy(source_path, destination_path, options);
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
