#pragma once
#include "command.hpp"

class mkdir_t : public command_t
{
public:
    explicit mkdir_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~mkdir_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::MKDIR;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    std::string _directory_path;
};
