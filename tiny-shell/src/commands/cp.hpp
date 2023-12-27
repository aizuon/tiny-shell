#pragma once
#include "command.hpp"

class cp_t final : public command_t
{
public:
    explicit cp_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~cp_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::CP;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    bool _recursive = false;
    bool _overwrite = false;
    std::string _source;
    std::string _destination;
};
