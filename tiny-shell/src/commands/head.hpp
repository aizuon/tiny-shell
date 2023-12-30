#pragma once
#include "command.hpp"

class head_t final : public command_t
{
public:
    explicit head_t(const std::string& arguments, std::shared_ptr<std::stringstream> ostream = nullptr,
                    std::shared_ptr<std::stringstream> istream = nullptr) : command_t(
        arguments, ostream, istream)
    {
    }

    ~head_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::HEAD;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    std::string _path;
    size_t _lines = 10;
};
