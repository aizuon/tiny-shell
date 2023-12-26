#pragma once
#include <cstdint>

enum class command_type_t : uint8_t
{
    BEGIN,
    NOP,
    exit,
    pwd,
    ls,
    cd,
    cat,
    cp,
    mkdir,
    mv,
    rm,
    echo,
    clear,
    END
};
