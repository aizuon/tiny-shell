#pragma once
#include "command.hpp"

class cat_t final : public command_t
{
public:
    explicit cat_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~cat_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::CAT;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    std::string _path;
};
