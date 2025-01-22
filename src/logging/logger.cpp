#include "logger.h"
#include <sstream>
#include <iomanip>
#include <ctime>

std::unique_ptr<Logger> Logger::instance = nullptr;

Logger::Logger() : minLevel(LogLevel::INFO), initialized(false) {}

Logger& Logger::getInstance() {
    if (!instance) {
        instance = std::unique_ptr<Logger>(new Logger());
    }
    return *instance;
}

bool Logger::initialize(const std::string& logPath, LogLevel minLevel) {
    std::lock_guard<std::mutex> lock(logMutex);

    if (initialized) {
        return true;
    }

    this->logFilePath = logPath;
    this->minLevel = minLevel;

    logFile.open(logPath, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        return false;
    }

    initialized = true;
    info("Logger initialized");
    return true;
}

void Logger::cleanup() {
    std::lock_guard<std::mutex> lock(logMutex);

    if (!initialized) {
        return;
    }

    if (logFile.is_open()) {
        info("Logger shutting down");
        logFile.close();
    }

    initialized = false;
}

void Logger::log(LogLevel level, const std::string& message,
    const std::string& file, int line,
    const std::string& function) {
    if (!initialized || level < minLevel) {
        return;
    }

    LogEntry entry(level, message, file, line, function);
    writeLogEntry(entry);
}

void Logger::debug(const std::string& message,
    const std::string& file, int line,
    const std::string& function) {
    log(LogLevel::DEBUG, message, file, line, function);
}

void Logger::info(const std::string& message,
    const std::string& file, int line,
    const std::string& function) {
    log(LogLevel::INFO, message, file, line, function);
}

void Logger::warn(const std::string& message,
    const std::string& file, int line,
    const std::string& function) {
    log(LogLevel::WARN, message, file, line, function);
}

void Logger::error(const std::string& message,
    const std::string& file, int line,
    const std::string& function) {
    log(LogLevel::ERROR, message, file, line, function);
}

void Logger::fatal(const std::string& message,
    const std::string& file, int line,
    const std::string& function) {
    log(LogLevel::FATAL, message, file, line, function);
}

void Logger::writeLogEntry(const LogEntry& entry) {
    std::lock_guard<std::mutex> lock(logMutex);

    if (!logFile.is_open()) {
        return;
    }

    logFile << formatLogEntry(entry) << std::endl;
    logFile.flush();
}

std::string Logger::formatLogEntry(const LogEntry& entry) {
    std::stringstream ss;

    // 시간 포맷팅
    auto timeT = std::chrono::system_clock::to_time_t(entry.timestamp);
    auto tm = std::localtime(&timeT);

    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S") << " "
        << "[" << LogLevelToString(entry.level) << "] "
        << entry.message;

    if (!entry.file.empty()) {
        ss << " (" << entry.file << ":" << entry.line;
        if (!entry.function.empty()) {
            ss << ", " << entry.function;
        }
        ss << ")";
    }

    return ss.str();
}