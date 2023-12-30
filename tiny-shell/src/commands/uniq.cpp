#include "pch.hpp"
#include "commands/uniq.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void uniq_t::setup_options_description()
{
    options_description->add_options()
            ("data", po::value<std::string>(&_data), "the data to process or the path to the file");
}

void uniq_t::setup_positional_options_description()
{
    positional_options_description->add("data", 1);
}

int uniq_t::execute()
{
    auto& ostream = get_ostream();
    std::istringstream data_stream(_data);
    std::string line;
    std::string last_line;
    bool first_line = true;

    fs::path possible_path(_data);
    if (fs::exists(possible_path) && fs::is_regular_file(possible_path))
    {
        std::ifstream file_stream(possible_path, std::ios::binary);
        if (!file_stream)
        {
            std::cerr << "Error: Unable to open file.\n";
            std::cerr.flush();
            return 1;
        }
        data_stream.str(std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>()));
    }

    while (std::getline(data_stream, line))
    {
        if (first_line || last_line != line)
        {
            ostream << line << '\n';
            last_line = line;
            first_line = false;
        }
    }

    ostream.flush();
    return 0;
}
