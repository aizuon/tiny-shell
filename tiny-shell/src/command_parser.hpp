#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include "command.hpp"

class command_parser_t
{
public:
    explicit command_parser_t(const std::string& command) : _command(command)
    {
    }

    std::vector<std::shared_ptr<command_t>> parse();

private:
    std::string _command;

    std::shared_ptr<command_t> parse_segment(const std::string& segment, std::shared_ptr<std::stringstream> ostream,
                                             std::shared_ptr<std::stringstream> istream);
};
