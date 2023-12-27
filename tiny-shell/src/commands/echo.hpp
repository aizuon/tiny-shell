#pragma once
#include "command.hpp"

class echo_t : public command_t
{
public:
    explicit echo_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~echo_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::ECHO;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    std::string _text;
    bool _newline = true;
};