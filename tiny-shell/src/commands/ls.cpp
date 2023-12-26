#include "pch.hpp"
#include "commands/ls.hpp"
#include "state.hpp"

#include <iostream>
#include <chrono>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

void ls_t::setup_options_description()
{
    options_description->add_options()
            ("long,l", po::bool_switch(&_long_format), "use a long listing format")
            ("all,a", po::bool_switch(&_all), "include directory entries whose names begin with a dot (.)")
            ("reverse,r", po::bool_switch(&_reverse), "reverse order while sorting")
            ("time,t", po::bool_switch(&_sort_time), "sort by modification time, newest first")
            ("human-readable,h", po::bool_switch(&_human_readable),
             "with -l, print sizes in human readable format (e.g., 1K 234M 2G)")
            ("path", po::value<std::string>(&_path), "specify the path to list");
}

void ls_t::setup_positional_options_description()
{
    positional_options_description->add("path", 1);
}

int ls_t::execute()
{
    auto path_to_list = !_path.empty()
                                ? fs::weakly_canonical(state_t::current_path / fs::path(_path))
                                : state_t::current_path;

    if (!fs::exists(path_to_list) || !fs::is_directory(path_to_list))
    {
        std::cerr << "Error: Path does not exist or is not a directory.\n";
        std::cerr.flush();
        return 1;
    }

    std::vector<fs::directory_entry> entries;
    for (const auto&entry: fs::directory_iterator(path_to_list))
    {
        if (!_all && entry.path().filename().string().front() == '.')
        {
            continue;
        }
        entries.push_back(entry);
    }

    if (_sort_time)
    {
        std::sort(entries.begin(), entries.end(), [](const fs::directory_entry&a, const fs::directory_entry&b)
        {
            return fs::last_write_time(a.path()) > fs::last_write_time(b.path());
        });
    }

    if (_reverse)
    {
        std::reverse(entries.begin(), entries.end());
    }

    size_t max_nlink_width = 0;
    size_t max_fsize_width = 0;
    for (const auto&entry: entries)
    {
        if (fs::is_regular_file(entry.status()))
        {
            max_fsize_width = std::max(max_fsize_width, std::to_string(fs::file_size(entry.path())).length());
        }
        max_nlink_width = std::max(max_nlink_width, std::to_string(fs::hard_link_count(entry.path())).length());
    }

    for (const auto&entry: entries)
    {
        if (_long_format)
        {
            auto ftime = fs::last_write_time(entry.path());
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
            auto cftime = std::chrono::system_clock::to_time_t(sctp);
            auto mod_time = std::string(std::asctime(std::localtime(&cftime)));
            mod_time.pop_back();

            auto perms = get_permissions(entry.status().permissions());
            auto nlink_str = std::to_string(fs::hard_link_count(entry.path()));
            auto fsize_str = std::string("0");
            if (fs::is_regular_file(entry.status()))
            {
                fsize_str = format_size(fs::file_size(entry.path()), _human_readable);
            }

            std::cout << perms << " "
                    << std::setw(max_nlink_width) << nlink_str << " "
                    << std::setw(max_fsize_width) << fsize_str << " "
                    << mod_time << " "
                    << entry.path().filename().string();

            if (fs::is_symlink(entry.symlink_status()))
            {
                std::cout << " -> " << fs::read_symlink(entry.path()).string();
            }

            std::cout << "\n";
        }
        else
        {
            std::cout << entry.path().filename().string() << "\n";
        }
    }

    std::cout.flush();
    return 0;
}
