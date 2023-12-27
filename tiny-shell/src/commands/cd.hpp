#pragma once
#include "command.hpp"

class cd_t final : public command_t
{
public:
    explicit cd_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~cd_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::CD;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    std::string _path;
};
