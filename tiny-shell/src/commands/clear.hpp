#pragma once
#include "command.hpp"

class clear_t final : public command_t
{
public:
    explicit clear_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~clear_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::CLEAR;
    }

protected:
    void setup_options_description() override
    {
    }

    void setup_positional_options_description() override
    {
    }
};
