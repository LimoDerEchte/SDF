//
// Created by Limo on 12/06/2026.
//

#include "util/Log.hpp"

using namespace RC;

template <class... Args>
void Log::Verbose(const std::format_string<Args...> message, Args&&... args) {
    const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
    Output::send<LogLevel::Verbose>(UtfN::StringToWString(output));
}

template <class... Args>
void Log::Normal(const std::format_string<Args...> message, Args&&... args) {
    const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
    Output::send<LogLevel::Normal>(UtfN::StringToWString(output));
}

template <class... Args>
void Log::Warning(const std::format_string<Args...> message, Args&&... args) {
    const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
    Output::send<LogLevel::Warning>(UtfN::StringToWString(output));
}

template <class... Args>
void Log::Error(const std::format_string<Args...> message, Args&&... args) {
    const std::string output = "[SDF]: " + std::format(message, std::forward<Args>(args)...) + "\n";
    Output::send<LogLevel::Error>(UtfN::StringToWString(output));
}
