#pragma once
#include "command.hpp"

class mv_t : public command_t
{
public:
    explicit mv_t(const std::string& arguments) : command_t(arguments)
    {
    }

    ~mv_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::MV;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    bool _overwrite = false;
    std::string _source;
    std::string _destination;
};
