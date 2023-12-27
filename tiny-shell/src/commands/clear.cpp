#include "pch.hpp"
#include "commands/clear.hpp"

#include <iostream>

int clear_t::execute()
{
    std::cout << "\033[2J\033[H";
    std::cout.flush();

    return 0;
}
