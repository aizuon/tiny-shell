#pragma once
#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::string_view trim(std::string_view sv);

std::string preprocess_arguments(const std::string& arguments);

std::string get_permissions(fs::perms p);

std::string get_username(uid_t uid);

std::string get_groupname(gid_t gid);

std::string format_size(uintmax_t size, bool human_readable = false);
