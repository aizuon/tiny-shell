#pragma once
#include "command.hpp"

class rm_t : public command_t
{
public:
    explicit rm_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~rm_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::RM;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    bool _recursive = false;
    bool _force = false;
    std::vector<std::string> _targets;
};
