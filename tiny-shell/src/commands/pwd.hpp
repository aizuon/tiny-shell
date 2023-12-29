#pragma once
#include "command.hpp"

class pwd_t final : public command_t
{
public:
    explicit pwd_t(const std::string& arguments, std::shared_ptr<std::stringstream> ostream = nullptr,
                   std::shared_ptr<std::stringstream> istream = nullptr) : command_t(
        arguments, ostream, istream)
    {
    }

    ~pwd_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::PWD;
    }
};
