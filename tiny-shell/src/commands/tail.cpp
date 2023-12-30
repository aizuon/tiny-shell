#include "pch.hpp"
#include "commands/tail.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <deque>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void tail_t::setup_options_description()
{
    options_description->add_options()
            ("path", po::value<std::string>(&_path), "specify the path to the file")
            ("lines,n", po::value<size_t>(&_num_lines)->default_value(10), "number of lines to display");
}

void tail_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
}

int tail_t::execute()
{
    auto& ostream = get_ostream();

    const auto path_to_tail = !_path.empty()
                                  ? fs::weakly_canonical(state_t::current_path / fs::path(_path))
                                  : state_t::current_path;

    if (!fs::exists(path_to_tail) || !fs::is_regular_file(path_to_tail))
    {
        std::cerr << "Error: Path does not exist or is not a file.\n";
        std::cerr.flush();
        return 1;
    }

    std::ifstream file_stream(path_to_tail, std::ios::binary);
    if (!file_stream)
    {
        std::cerr << "Error: Unable to open file.\n";
        std::cerr.flush();
        return 1;
    }

    std::deque<std::string> lines;
    std::string line;
    while (std::getline(file_stream, line))
    {
        if (lines.size() >= _num_lines)
        {
            lines.pop_front();
        }
        lines.push_back(line);
    }

    for (const auto& l: lines)
    {
        ostream << l << '\n';
    }

    ostream.flush();
    return 0;
}
