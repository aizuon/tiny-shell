#pragma once
#include "command.hpp"

class ls_t final : public command_t
{
public:
    explicit ls_t(const std::string& arguments, std::shared_ptr<std::stringstream> ostream = nullptr,
                  std::shared_ptr<std::stringstream> istream = nullptr) : command_t(
        arguments, ostream, istream)
    {
    }

    ~ls_t() override = default;

    int execute() override;

    command_type_t get_command_type() const override
    {
        return command_type_t::LS;
    }

protected:
    void setup_options_description() override;

    void setup_positional_options_description() override;

private:
    bool _long_format = false;
    bool _all = false;
    bool _reverse = false;
    bool _sort_time = false;
    bool _human_readable = false;
    std::string _path;
};
