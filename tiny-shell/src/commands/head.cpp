#include "pch.hpp"
#include "commands/head.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void head_t::setup_options_description()
{
    options_description->add_options()
            ("path", po::value<std::string>(&_path), "specify the path to the file")
            ("lines,n", po::value<size_t>(&_lines)->default_value(10), "number of lines to display");
}

void head_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
}

int head_t::execute()
{
    auto& ostream = get_ostream();

    const auto path_to_head = !_path.empty()
                                  ? fs::weakly_canonical(state_t::current_path / fs::path(_path))
                                  : state_t::current_path;

    if (!fs::exists(path_to_head) || !fs::is_regular_file(path_to_head))
    {
        std::cerr << "Error: Path does not exist or is not a file.\n";
        std::cerr.flush();
        return 1;
    }

    std::ifstream file_stream(path_to_head);
    if (!file_stream)
    {
        std::cerr << "Error: Unable to open file.\n";
        std::cerr.flush();
        return 1;
    }

    std::string line;
    int line_count = 0;
    while (std::getline(file_stream, line) && line_count < _lines)
    {
        ostream << line << '\n';
        ++line_count;
    }

    ostream.flush();
    return 0;
}
