#include "pch.hpp"
#include "command_parser.hpp"
#include "utils.hpp"
#include "commands/exit.hpp"
#include "commands/pwd.hpp"
#include "commands/ls.hpp"
#include "commands/cd.hpp"
#include "commands/cat.hpp"
#include "commands/mkdir.hpp"
#include "commands/rm.hpp"

#include <string_view>
#include <magic_enum.hpp>


std::shared_ptr<command_t> command_parser_t::parse()
{
    auto command_view = std::string_view(_command);
    constexpr auto commands = magic_enum::enum_entries<command_type_t>();
    for (auto [command, command_string]: commands)
    {
        auto command_string_view = std::string_view(command_string);
        if (_command.starts_with(command_string))
        {
            auto arguments_view = command_view.substr(command_string_view.size());
            arguments_view = trim(arguments_view);
            auto arguments = std::string(arguments_view);
            std::shared_ptr<command_t> command_constructed = nullptr;
            switch (command)
            {
                case command_type_t::exit:
                    command_constructed = std::make_shared<exit_t>(arguments);
                    break;
                case command_type_t::pwd:
                    command_constructed = std::make_shared<pwd_t>(arguments);
                    break;
                case command_type_t::ls:
                    command_constructed = std::make_shared<ls_t>(arguments);
                    break;
                case command_type_t::cd:
                    command_constructed = std::make_shared<cd_t>(arguments);
                    break;
                case command_type_t::cat:
                    command_constructed = std::make_shared<cat_t>(arguments);
                    break;
                case command_type_t::mkdir:
                    command_constructed = std::make_shared<mkdir_t>(arguments);
                    break;
                case command_type_t::rm:
                    command_constructed = std::make_shared<rm_t>(arguments);
                    break;
                default:
                    break;
            }
            if (command_constructed != nullptr)
                command_constructed->initialize();
            return command_constructed;
        }
    }

    return nullptr;
}
