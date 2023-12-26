#include "pch.hpp"
#include "utils.hpp"

#include <cctype>
#include <sstream>
#include <algorithm>
#include <pwd.h>
#include <grp.h>

std::string_view trim(std::string_view sv)
{
    auto is_space = [](unsigned char ch) { return std::isspace(ch); };

    auto start = std::find_if_not(sv.begin(), sv.end(), is_space);
    auto end = std::find_if_not(sv.rbegin(), sv.rend(), is_space).base();

    if (start < end)
    {
        return { start, static_cast<std::size_t>(end - start) };
    }
    else
    {
        return {};
    }
}

std::string preprocess_arguments(const std::string& arguments)
{
    std::string processed_args;
    bool in_quotes = false;
    for (size_t i = 0; i < arguments.size(); ++i)
    {
        if (arguments[i] == '\'' || arguments[i] == '\"')
        {
            in_quotes = !in_quotes;
        }
        if (!in_quotes && arguments[i] == '-' && i + 1 < arguments.size() && arguments[i + 1] != ' ' && arguments[i + 1] != '-')
        {
            size_t j = i + 1;
            while (j < arguments.size() && arguments[j] != ' ' && arguments[j] != '-')
            {
                processed_args += '-';
                processed_args += arguments[j];
                processed_args += ' ';
                ++j;
            }
            i = j - 1;
        }
        else
        {
            processed_args += arguments[i];
        }
    }
    return processed_args;
}

std::string get_permissions(fs::perms p)
{
    std::string perms;
    perms += ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
    perms += ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
    perms += ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-");
    perms += ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-");
    perms += ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-");
    perms += ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-");
    perms += ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-");
    perms += ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-");
    perms += ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
    return perms;
}

std::string get_username(uid_t uid)
{
    struct passwd* pw = getpwuid(uid);
    return pw ? pw->pw_name : std::to_string(uid);
}

std::string get_groupname(gid_t gid)
{
    struct group* gr = getgrgid(gid);
    return gr ? gr->gr_name : std::to_string(gid);
}

std::string format_size(uintmax_t size, bool human_readable /*= false*/)
{
    std::ostringstream out;
    if (human_readable)
    {
        static const char* sizes[] = { "B", "K", "M", "G", "T" };
        int order = 0;
        double dbl_size = size;
        while (dbl_size >= 1024 && order < (sizeof(sizes) / sizeof(*sizes)) - 1)
        {
            order++;
            dbl_size /= 1024;
        }
        out << std::fixed << std::setprecision(2) << dbl_size << sizes[order];
    }
    else
    {
        out << size;
    }
    return out.str();
}
