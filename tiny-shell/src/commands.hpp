#pragma once
#include <cstdint>

enum class command_type_t : uint8_t
{
    BEGIN,
    NOP,
    EXIT,
    PWD,
    LS,
    CD,
    CAT,
    CP,
    MKDIR,
    MV,
    RM,
    ECHO,
    CLEAR,
    EXPORT,
    END
};
