#pragma once
#include <cstdint>

enum class command_type_t : uint8_t
{
    BEGIN,
    NOP,
    EXIT,
    CHMOD,
    CHOWN,
    PS,
    KILL,
    PWD,
    LS,
    CD,
    TOUCH,
    FIND,
    HEAD,
    TAIL,
    UNIQ,
    WC,
    CAT,
    CP,
    MKDIR,
    MV,
    RM,
    ECHO,
    GREP,
    CLEAR,
    EXPORT,
    END
};
