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
#include "commands/cp.hpp"
#include "commands/mv.hpp"
#include "commands/echo.hpp"
#include "commands/export.hpp"
#include "commands/clear.hpp"

#include <string_view>
#include <magic_enum.hpp>


std::shared_ptr<command_t> command_parser_t::parse()
{
    auto command_view = std::string_view(_command);
    constexpr auto commands = magic_enum::enum_entries<command_type_t>();
    for (auto [command, command_string]: commands)
    {
        command_string = to_lowercase(std::string(command_string));
        if (_command.starts_with(command_string))
        {
            auto arguments_view = command_view.substr(command_string.size());
            arguments_view = trim(arguments_view);
            auto arguments = std::string(arguments_view);
            std::shared_ptr<command_t> command_constructed = nullptr;
            switch (command)
            {
                case command_type_t::EXIT:
                    command_constructed = std::make_shared<exit_t>(arguments);
                    break;
                case command_type_t::PWD:
                    command_constructed = std::make_shared<pwd_t>(arguments);
                    break;
                case command_type_t::LS:
                    command_constructed = std::make_shared<ls_t>(arguments);
                    break;
                case command_type_t::CD:
                    command_constructed = std::make_shared<cd_t>(arguments);
                    break;
                case command_type_t::CAT:
                    command_constructed = std::make_shared<cat_t>(arguments);
                    break;
                case command_type_t::MKDIR:
                    command_constructed = std::make_shared<mkdir_t>(arguments);
                    break;
                case command_type_t::RM:
                    command_constructed = std::make_shared<rm_t>(arguments);
                    break;
                case command_type_t::CP:
                    command_constructed = std::make_shared<cp_t>(arguments);
                    break;
                case command_type_t::MV:
                    command_constructed = std::make_shared<mv_t>(arguments);
                    break;
                case command_type_t::ECHO:
                    command_constructed = std::make_shared<echo_t>(arguments);
                    break;
                case command_type_t::EXPORT:
                    command_constructed = std::make_shared<export_t>(arguments);
                    break;
                case command_type_t::CLEAR:
                    command_constructed = std::make_shared<clear_t>(arguments);
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
