#pragma once
#include "command.hpp"

class exit_t : public command_t
{
public:
    explicit exit_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~exit_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::EXIT;
    }
};
