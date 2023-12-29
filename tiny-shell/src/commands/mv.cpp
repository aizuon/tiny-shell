#include "pch.hpp"
#include "commands/mv.hpp"
#include "state.hpp"

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void mv_t::setup_options_description()
{
    options_description->add_options()
            ("overwrite,o", po::bool_switch(&_overwrite), "overwrite the destination if it already exists")
            ("source", po::value<std::string>(&_source), "specify the source file/directory path")
            ("destination", po::value<std::string>(&_destination), "specify the destination file/directory path");
}

void mv_t::setup_positional_options_description()
{
    positional_options_description->add("source", 1);
    positional_options_description->add("destination", 1);
}

int mv_t::execute()
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

        if (fs::exists(destination_path) && !_overwrite)
        {
            std::cerr << "Error: Destination already exists, use -o or --overwrite to overwrite it.\n";
            std::cerr.flush();
            return 1;
        }

        fs::rename(source_path, destination_path);
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr.flush();
        return 1;
    }

    return 0;
}
