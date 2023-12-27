#pragma once
#include "command.hpp"
#include <vector>

class export_t final : public command_t
{
public:
    explicit export_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~export_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::EXPORT;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    std::vector<std::string> _variables;
};
