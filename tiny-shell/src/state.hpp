#pragma once
#include <filesystem>

namespace fs = std::filesystem;

class state_t
{
public:
    static fs::path current_path;
};
