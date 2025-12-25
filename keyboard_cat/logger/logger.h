#ifndef LOGGER_H
#define LOGGER_H

#include <fmt/format.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
};

class Logger
{
public:
    static void Init();

    template<typename... Args>
    static void Log(LogLevel level, fmt::format_string<Args...> fmt_str, Args&&... args) {
        std::lock_guard<std::mutex> lock(s_mutex);

        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);

        char time_buf[32];
        std::strftime(time_buf, sizeof(time_buf), "%H:%M:%S", &tm);

        std::string level_str = GetLevelString(level);
        std::string message = fmt::format(fmt_str, std::forward<Args>(args)...);

        // Console output
        std::cout << fmt::format("[{}] [{}] {}\n", time_buf, level_str, message);

        // File output
        if (s_file.is_open()) {
            char datetime_buf[64];
            std::strftime(datetime_buf, sizeof(datetime_buf), "%Y-%m-%d %H:%M:%S", &tm);
            s_file << fmt::format("[{}] [{}] {}\n", datetime_buf, level_str, message);
            if (level >= LogLevel::ERROR) {
                s_file.flush();
            }
        }
    }

private:
    static std::string GetLevelString(LogLevel level);
    static std::mutex s_mutex;
    static std::ofstream s_file;
};

// Convenience macros for logging
#define LOG_TRACE(...)    ::Logger::Log(::LogLevel::TRACE, __VA_ARGS__)
#define LOG_DEBUG(...)    ::Logger::Log(::LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...)     ::Logger::Log(::LogLevel::INFO, __VA_ARGS__)
#define LOG_WARN(...)     ::Logger::Log(::LogLevel::WARN, __VA_ARGS__)
#define LOG_ERROR(...)    ::Logger::Log(::LogLevel::ERROR, __VA_ARGS__)
#define LOG_CRITICAL(...) ::Logger::Log(::LogLevel::CRITICAL, __VA_ARGS__)

#endif // !LOGGER_H
