//
// Created by Limo on 22/05/2026.
//

#pragma once

#include <string>

#include "UtfN.hpp"
#include "DynamicOutput/Output.hpp"

namespace Log {
    template <class... Args>
    static void Verbose(const std::format_string<Args...> message, Args&&... args) {
        const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
        RC::Output::send<RC::LogLevel::Verbose>(UtfN::StringToWString(output));
    }

    template <class... Args>
    static void Normal(const std::format_string<Args...> message, Args&&... args) {
        const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
        RC::Output::send<RC::LogLevel::Normal>(UtfN::StringToWString(output));
    }

    template <class... Args>
    static void Warning(const std::format_string<Args...> message, Args&&... args) {
        const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
        RC::Output::send<RC::LogLevel::Warning>(UtfN::StringToWString(output));
    }

    template <class... Args>
    static void Error(const std::format_string<Args...> message, Args&&... args) {
        const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
        RC::Output::send<RC::LogLevel::Error>(UtfN::StringToWString(output));
    }
}
