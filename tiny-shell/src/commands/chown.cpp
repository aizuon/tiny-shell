#include "pch.hpp"
#include "commands/chown.hpp"
#include "state.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void chown_t::setup_options_description()
{
    options_description->add_options()
            ("path", po::value<std::string>(&_path), "specify the path to change ownership")
            ("owner", po::value<std::string>(&_owner), "specify the new owner")
            ("group", po::value<std::string>(&_group), "specify the new group");
}

void chown_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
    positional_options_description->add("owner", 1);
    positional_options_description->add("group", 1);
}

int chown_t::execute()
{
    const auto path_to_chown = !_path.empty()
                                   ? fs::weakly_canonical(state_t::current_path / fs::path(_path))
                                   : state_t::current_path;

    if (!fs::exists(path_to_chown))
    {
        std::cerr << "Error: Path does not exist.\n";
        std::cerr.flush();
        return 1;
    }

    uid_t uid = -1;
    gid_t gid = -1;

    if (!_owner.empty())
    {
        struct passwd* pwd = getpwnam(_owner.c_str());
        if (pwd)
        {
            uid = pwd->pw_uid;
        }
        else
        {
            std::cerr << "Error: Owner user not found.\n";
            std::cerr.flush();
            return 1;
        }
    }

    if (!_group.empty())
    {
        struct group* grp = getgrnam(_group.c_str());
        if (grp)
        {
            gid = grp->gr_gid;
        }
        else
        {
            std::cerr << "Error: Group not found.\n";
            std::cerr.flush();
            return 1;
        }
    }

    if (chown(path_to_chown.c_str(), uid, gid) == -1)
    {
        std::cerr << "Error: Unable to change ownership.\n";
        std::cerr.flush();
        return 1;
    }

    return 0;
}
