#pragma once
#include "command.hpp"

class pwd_t : public command_t
{
public:
    explicit pwd_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~pwd_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::PWD;
    }
};
