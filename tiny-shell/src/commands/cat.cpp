#include "pch.hpp"
#include "commands/cat.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void cat_t::setup_options_description()
{
    options_description->add_options()
            ("path", po::value<std::string>(&_path), "specify the path to list");
}

void cat_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
}

int cat_t::execute()
{
    auto path_to_cat = !_path.empty()
                           ? fs::weakly_canonical(state_t::current_path / fs::path(_path))
                           : state_t::current_path;

    if (!fs::exists(path_to_cat) || !fs::is_regular_file(path_to_cat))
    {
        std::cerr << "Error: Path does not exist or is not a file.\n";
        std::cerr.flush();
        return 1;
    }

    auto file_stream = std::ifstream(path_to_cat, std::ios::binary);
    if (!file_stream)
    {
        std::cerr << "Error: Unable to open file.\n";
        std::cerr.flush();
        return 1;
    }
    std::cout << file_stream.rdbuf();

    std::cout.flush();
    return 0;
}
