#include "pch.hpp"
#include "commands/rm.hpp"
#include "state.hpp"

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void rm_t::setup_options_description()
{
    options_description->add_options()
            ("recursive,r", po::bool_switch(&_recursive), "remove directories and their contents recursively")
            ("force,f", po::bool_switch(&_force), "ignore nonexistent files and arguments, never prompt")
            ("target", po::value<std::vector<std::string>>(&_targets), "specify the file/directory paths to remove");
}

void rm_t::setup_positional_options_description()
{
    positional_options_description->add("target", -1);
}

int rm_t::execute()
{
    if (_targets.empty())
    {
        std::cerr << "Error: No target specified for removal.\n";
        std::cerr.flush();
        return 1;
    }

    for (const auto& target_path_str: _targets)
    {
        auto target_path = fs::weakly_canonical(state_t::current_path / fs::path(target_path_str));

        try
        {
            if (fs::exists(target_path) || _force)
            {
                if (_recursive && fs::is_directory(target_path))
                {
                    fs::remove_all(target_path);
                }
                else
                {
                    fs::remove(target_path);
                }
            }
            else
            {
                std::cerr << "Error: Target does not exist: " << target_path << "\n";
                std::cerr.flush();
                return 1;
            }
        }
        catch (const fs::filesystem_error& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            std::cerr.flush();
            return 1;
        }
    }

    return 0;
}
