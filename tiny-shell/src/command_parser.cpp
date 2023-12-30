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
#include "commands/grep.hpp"
#include "commands/export.hpp"
#include "commands/clear.hpp"
#include "commands/touch.hpp"
#include "commands/chmod.hpp"
#include "commands/chown.hpp"
#include "commands/find.hpp"
#include "commands/head.hpp"
#include "commands/tail.hpp"
#include "commands/uniq.hpp"
#include "commands/wc.hpp"

#include <string_view>
#include <magic_enum.hpp>

std::vector<std::shared_ptr<command_t>> command_parser_t::parse()
{
    std::vector<std::shared_ptr<command_t>> pipeline;
    std::istringstream command_stream(_command);
    std::string segment;
    std::shared_ptr<std::stringstream> previous_stream = nullptr;

    while (std::getline(command_stream, segment, '|'))
    {
        segment = trim(segment);
        _command = segment;

        char next_char = command_stream.peek();

        bool is_last_segment = next_char == std::istringstream::traits_type::eof();

        std::shared_ptr<std::stringstream> current_stream = is_last_segment
                                                                ? nullptr
                                                                : std::make_shared<std::stringstream>();

        auto cmd = parse_segment(segment, current_stream, previous_stream);
        pipeline.push_back(cmd);
        previous_stream = current_stream;
    }

    return pipeline;
}

std::shared_ptr<command_t> command_parser_t::parse_segment(const std::string& segment,
                                                           std::shared_ptr<std::stringstream> ostream,
                                                           std::shared_ptr<std::stringstream> istream)
{
    const auto command_view = std::string_view(segment);
    constexpr auto commands = magic_enum::enum_entries<command_type_t>();
    for (auto [command, command_string_view]: commands)
    {
        auto command_string = to_lowercase(std::string(command_string_view));
        if (_command.starts_with(command_string))
        {
            auto arguments_view = command_view.substr(command_string.size());
            arguments_view = trim(arguments_view);
            auto arguments = std::string(arguments_view);
            std::shared_ptr<command_t> command_constructed = nullptr;
            switch (command)
            {
                case command_type_t::EXIT:
                    command_constructed = std::make_shared<exit_t>(arguments, ostream, istream);
                    break;
                case command_type_t::CHMOD:
                    command_constructed = std::make_shared<chmod_t>(arguments, ostream, istream);
                    break;
                case command_type_t::CHOWN:
                    command_constructed = std::make_shared<chown_t>(arguments, ostream, istream);
                    break;
                case command_type_t::PWD:
                    command_constructed = std::make_shared<pwd_t>(arguments, ostream, istream);
                    break;
                case command_type_t::LS:
                    command_constructed = std::make_shared<ls_t>(arguments, ostream, istream);
                    break;
                case command_type_t::CD:
                    command_constructed = std::make_shared<cd_t>(arguments, ostream, istream);
                    break;
                case command_type_t::TOUCH:
                    command_constructed = std::make_shared<touch_t>(arguments, ostream, istream);
                    break;
                case command_type_t::FIND:
                    command_constructed = std::make_shared<find_t>(arguments, ostream, istream);
                    break;
                case command_type_t::HEAD:
                    command_constructed = std::make_shared<head_t>(arguments, ostream, istream);
                    break;
                case command_type_t::TAIL:
                    command_constructed = std::make_shared<tail_t>(arguments, ostream, istream);
                    break;
                case command_type_t::UNIQ:
                    command_constructed = std::make_shared<uniq_t>(arguments, ostream, istream);
                    break;
                case command_type_t::WC:
                    command_constructed = std::make_shared<wc_t>(arguments, ostream, istream);
                    break;
                case command_type_t::CAT:
                    command_constructed = std::make_shared<cat_t>(arguments, ostream, istream);
                    break;
                case command_type_t::MKDIR:
                    command_constructed = std::make_shared<mkdir_t>(arguments, ostream, istream);
                    break;
                case command_type_t::RM:
                    command_constructed = std::make_shared<rm_t>(arguments, ostream, istream);
                    break;
                case command_type_t::CP:
                    command_constructed = std::make_shared<cp_t>(arguments, ostream, istream);
                    break;
                case command_type_t::MV:
                    command_constructed = std::make_shared<mv_t>(arguments, ostream, istream);
                    break;
                case command_type_t::ECHO:
                    command_constructed = std::make_shared<echo_t>(arguments, ostream, istream);
                    break;
                case command_type_t::GREP:
                    command_constructed = std::make_shared<grep_t>(arguments, ostream, istream);
                    break;
                case command_type_t::EXPORT:
                    command_constructed = std::make_shared<export_t>(arguments, ostream, istream);
                    break;
                case command_type_t::CLEAR:
                    command_constructed = std::make_shared<clear_t>(arguments, ostream, istream);
                    break;
                default:
                    break;
            }
            return command_constructed;
        }
    }

    return nullptr;
}
