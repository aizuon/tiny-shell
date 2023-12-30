#include "pch.hpp"
#include "commands/wc.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void wc_t::setup_options_description()
{
    options_description->add_options()
            ("data", po::value<std::string>(&_data), "the data to process or the path to the file");
}

void wc_t::setup_positional_options_description()
{
    positional_options_description->add("data", 1);
}

int wc_t::execute()
{
    auto& ostream = get_ostream();
    std::istringstream data_stream(_data);
    std::string content;
    std::string line;
    size_t line_count = 0;
    size_t word_count = 0;
    size_t byte_count = 0;

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
        content.assign(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
        data_stream.str(content);
    }
    else
    {
        content = _data;
    }

    while (std::getline(data_stream, line))
    {
        line_count++;
        word_count += std::distance(std::istream_iterator<std::string>(std::istringstream(line) >> std::ws),
                                    std::istream_iterator<std::string>());
    }
    byte_count = content.size();

    ostream << line_count << " " << word_count << " " << byte_count << std::endl;

    ostream.flush();
    return 0;
}
