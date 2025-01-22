#pragma once
#include "log_level.h"
#include "log_entry.h"
#include <mutex>
#include <fstream>
#include <memory>
#include <string>

class Logger {
private:
    static std::unique_ptr<Logger> instance;
    std::mutex logMutex;
    std::ofstream logFile;
    LogLevel minLevel;
    bool initialized;
    std::string logFilePath;

    Logger();

public:
    static Logger& getInstance();

    bool initialize(const std::string& logPath = "mod.log", LogLevel minLevel = LogLevel::INFO);
    void cleanup();

    void log(LogLevel level, const std::string& message,
        const std::string& file = "", int line = 0,
        const std::string& function = "");

    void debug(const std::string& message,
        const std::string& file = "", int line = 0,
        const std::string& function = "");
    void info(const std::string& message,
        const std::string& file = "", int line = 0,
        const std::string& function = "");
    void warn(const std::string& message,
        const std::string& file = "", int line = 0,
        const std::string& function = "");
    void error(const std::string& message,
        const std::string& file = "", int line = 0,
        const std::string& function = "");
    void fatal(const std::string& message,
        const std::string& file = "", int line = 0,
        const std::string& function = "");

    bool isInitialized() const { return initialized; }

private:
    void writeLogEntry(const LogEntry& entry);
    std::string formatLogEntry(const LogEntry& entry);
};

// 매크로 정의
#define LOG_DEBUG(message) Logger::getInstance().debug(message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_INFO(message)  Logger::getInstance().info(message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_WARN(message)  Logger::getInstance().warn(message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_ERROR(message) Logger::getInstance().error(message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_FATAL(message) Logger::getInstance().fatal(message, __FILE__, __LINE__, __FUNCTION__)