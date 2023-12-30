#include "pch.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>

#include "command_parser.hpp"

void atexit_handler()
{
    std::cout << "Session ended.\n";
}

int main(int argc, char** argv)
{
    std::atexit(atexit_handler);

    std::string input;

    while (true)
    {
        std::cout << "tiny-shell> ";
        std::cout.flush();

        if (!std::getline(std::cin, input))
            break;

        const auto parser = std::make_shared<command_parser_t>(input);
        auto commands = parser->parse();
        for (auto& command: commands)
        {
            if (command == nullptr)
            {
                std::cerr << "Command not found.\n";
                std::cerr.flush();
                break;
            }
            command->initialize();
            auto res = command->execute();
            if (res != 0)
                break;
        }
    }

    return 0;
}
