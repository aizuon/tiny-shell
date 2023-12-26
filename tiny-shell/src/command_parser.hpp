#pragma once
#include <string>
#include <memory>

#include "command.hpp"

class command_parser_t
{
public:
    command_parser_t(const std::string& command) : _command(command) {}

    std::shared_ptr<command_t> parse();

private:
    std::string _command;
};
