#include "pch.hpp"
#include "commands/pwd.hpp"
#include "state.hpp"

int pwd_t::execute()
{
    auto& ostream = get_ostream();

    ostream << state_t::current_path << "\n";

    ostream.flush();
    return 0;
}
