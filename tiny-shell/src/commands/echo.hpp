#pragma once
#include "command.hpp"

class echo_t final : public command_t
{
public:
    explicit echo_t(const std::string& arguments, std::shared_ptr<std::stringstream> ostream = nullptr,
                    std::shared_ptr<std::stringstream> istream = nullptr) : command_t(
        arguments, ostream, istream)
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
