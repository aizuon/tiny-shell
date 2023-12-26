#include "pch.hpp"
#include "commands/exit.hpp"

#include <stdlib.h>

int exit_t::execute()
{
    exit(0);
    return 0;
}
