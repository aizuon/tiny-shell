#include "pch.hpp"
#include <stdlib.h>
#include <iostream>
#include <string>

#include "command_parser.hpp"

void atexit_handler()
{
    std::cout << "Session ended.\n";
}

int main(int argc, char** argv)
{
    atexit(atexit_handler);

    std::string input;

    while (true)
    {
        std::cout << "tiny-shell> ";
        std::cout.flush();

        if (!std::getline(std::cin, input))
            break;

        auto parser = std::make_shared<command_parser_t>(input);
        auto command = parser->parse();
        if (command != nullptr)
        {
            command->execute();
        }
    }

    return 0;
}
