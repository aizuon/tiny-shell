#include "pch.hpp"
#include "commands/find.hpp"
#include "state.hpp"

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void find_t::setup_options_description()
{
    options_description->add_options()
            ("path,p", po::value<std::string>(&_path), "specify the path to search in")
            ("name,n", po::value<std::string>(&_name_pattern), "pattern of the file names to search for");
}

void find_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
    positional_options_description->add("name", 1);
}

int find_t::execute()
{
    auto& ostream = get_ostream();

    auto search_path = !_path.empty()
                           ? fs::weakly_canonical(state_t::current_path / fs::path(_path))
                           : state_t::current_path;

    if (!fs::exists(search_path) || !fs::is_directory(search_path))
    {
        std::cerr << "Error: Path does not exist or is not a directory.\n";
        std::cerr.flush();
        return 1;
    }

    for (const auto& entry: fs::recursive_directory_iterator(search_path))
    {
        if (entry.is_regular_file() && entry.path().filename().string().find(_name_pattern) != std::string::npos)
        {
            ostream << entry.path() << "\n";
        }
    }

    ostream.flush();
    return 0;
}
