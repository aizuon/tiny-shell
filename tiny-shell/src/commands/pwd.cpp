#include "pch.hpp"
#include "commands/pwd.hpp"
#include "state.hpp"

int pwd_t::execute()
{
    std::cout << state_t::current_path << "\n";

    std::cout.flush();
    return 0;
}
