#include "pch.hpp"
#include "commands/exit.hpp"

#include <cstdlib>

int exit_t::execute()
{
    std::exit(0);
    return 0;
}
