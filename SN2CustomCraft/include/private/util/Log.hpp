//
// Created by Limo on 22/05/2026.
//

#pragma once

#include <string>

#include "UtfN.hpp"
#include "DynamicOutput/Output.hpp"

class Log {
public:
    template <class... Args>
    static void Verbose(std::format_string<Args...> message, Args&&... args);

    template <class... Args>
    static void Normal(std::format_string<Args...> message, Args&&... args);

    template <class... Args>
    static void Warning(std::format_string<Args...> message, Args&&... args);

    template <class... Args>
    static void Error(std::format_string<Args...> message, Args&&... args);
};
