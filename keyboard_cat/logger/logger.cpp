#include "logger.h"

std::mutex Logger::s_mutex;
std::ofstream Logger::s_file;

void Logger::Init()
{
    s_file.open("keyboard_cat.log", std::ios::app);
    LOG_INFO("Logger initialized");
}

std::string Logger::GetLevelString(LogLevel level)
{
    switch (level) {
        case LogLevel::TRACE:    return "TRACE";
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARN:     return "WARN";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}
