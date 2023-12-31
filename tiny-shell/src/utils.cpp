#include "pch.hpp"
#include "utils.hpp"

#include <cctype>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <pwd.h>
#include <grp.h>

std::string_view trim(std::string_view sv)
{
    auto is_space = [](const unsigned char ch) { return std::isspace(ch); };

    auto start = std::ranges::find_if_not(sv, is_space);
    const auto end = std::find_if_not(sv.rbegin(), sv.rend(), is_space).base();

    if (start < end)
    {
        return {start, static_cast<std::size_t>(end - start)};
    }

    return {};
}

std::string preprocess_arguments(const std::string& arguments)
{
    std::string processed_args;
    processed_args.reserve(arguments.size());
    bool in_quotes = false;
    bool expect_option = false;

    for (size_t i = 0; i < arguments.size(); ++i)
    {
        char current_char = arguments[i];

        if (current_char == '\'' || current_char == '\"')
        {
            in_quotes = !in_quotes;
            processed_args += current_char;
        }
        else if (!in_quotes && current_char == '$')
        {
            size_t j = i + 1;
            std::string env_var_name;
            bool curly_brace = false;

            if (j < arguments.size() && arguments[j] == '{')
            {
                curly_brace = true;
                ++j;
            }

            while (j < arguments.size() && (isalnum(arguments[j]) || arguments[j] == '_'))
            {
                env_var_name += arguments[j];
                ++j;
            }

            if (curly_brace && j < arguments.size() && arguments[j] == '}')
            {
                ++j;
            }
            else if (curly_brace)
            {
                processed_args += '$';
                processed_args += '{';
                processed_args += env_var_name;
                continue;
            }

            const char* env_var_value = std::getenv(env_var_name.c_str());
            if (env_var_value)
            {
                processed_args += env_var_value;
            }
            i = j - 1;
        }
        else if (!in_quotes && current_char == '-' && i + 1 < arguments.size() && isalpha(arguments[i + 1]))
        {
            if (expect_option)
            {
                processed_args += '-';
                processed_args += arguments[i + 1];
                processed_args += ' ';
                ++i;
            }
            else
            {
                processed_args += current_char;
            }
            expect_option = false;
        }
        else if (!in_quotes && current_char == '-' && i + 1 < arguments.size() && arguments[i + 1] == ' ')
        {
            expect_option = true;
            processed_args += current_char;
        }
        else
        {
            processed_args += current_char;
            expect_option = false;
        }
    }
    return processed_args;
}

std::string get_permissions(const fs::perms p)
{
    std::string perms;
    perms += (p & fs::perms::owner_read) != fs::perms::none ? "r" : "-";
    perms += (p & fs::perms::owner_write) != fs::perms::none ? "w" : "-";
    perms += (p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-";
    perms += (p & fs::perms::group_read) != fs::perms::none ? "r" : "-";
    perms += (p & fs::perms::group_write) != fs::perms::none ? "w" : "-";
    perms += (p & fs::perms::group_exec) != fs::perms::none ? "x" : "-";
    perms += (p & fs::perms::others_read) != fs::perms::none ? "r" : "-";
    perms += (p & fs::perms::others_write) != fs::perms::none ? "w" : "-";
    perms += (p & fs::perms::others_exec) != fs::perms::none ? "x" : "-";
    return perms;
}

std::string get_username(const uid_t uid)
{
    struct passwd* pw = getpwuid(uid);
    return pw ? pw->pw_name : std::to_string(uid);
}

std::string get_groupname(const gid_t gid)
{
    struct group* gr = getgrgid(gid);
    return gr ? gr->gr_name : std::to_string(gid);
}

std::string format_size(const uintmax_t size, const bool human_readable /*= false*/)
{
    std::ostringstream out;
    if (human_readable)
    {
        static const char* sizes[] = {"B", "K", "M", "G", "T"};
        int order = 0;
        double dbl_size = size;
        while (dbl_size >= 1024 && order < std::size(sizes) - 1)
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

std::string to_lowercase(const std::string& input)
{
    std::string output = input;
    std::ranges::transform(output, output.begin(),
                           [](const unsigned char c) -> unsigned char { return std::tolower(c); });
    return output;
}
