#pragma once
#include "commands.hpp"
#include "utils.hpp"

#include <string>
#include <memory>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class command_t
{
public:
    command_t() = delete;

    explicit command_t(const std::string& arguments)
        : _arguments(arguments)
    {
        setup_options_description();
        setup_positional_options_description();
    }

    void initialize();

    virtual int execute() = 0;

    virtual command_type_t get_command_type() const = 0;

    virtual ~command_t() = default;

protected:
    std::shared_ptr<po::options_description> options_description = std::make_shared<po::options_description>(
        "Allowed options");
    std::shared_ptr<po::positional_options_description> positional_options_description = std::make_shared<
        po::positional_options_description>();
    std::shared_ptr<po::variables_map> variables_map = std::make_shared<po::variables_map>();

    virtual void setup_options_description();

    virtual void setup_positional_options_description();

    bool help = false;

private:
    std::string _arguments;

    void setup_variables_map(const std::string& arguments);
};
