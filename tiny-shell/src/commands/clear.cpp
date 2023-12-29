#include "pch.hpp"
#include "commands/clear.hpp"

#include <iostream>

int clear_t::execute()
{
    auto& ostream = get_ostream();

    ostream << "\033[2J\033[H";
    ostream.flush();

    return 0;
}
