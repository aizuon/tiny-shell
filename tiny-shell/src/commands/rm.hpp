#pragma once
#include "command.hpp"

class rm_t final : public command_t
{
public:
    explicit rm_t(const std::string& arguments, std::shared_ptr<std::stringstream> ostream = nullptr,
                  std::shared_ptr<std::stringstream> istream = nullptr) : command_t(
        arguments, ostream, istream)
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
