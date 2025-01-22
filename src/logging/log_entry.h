#pragma once
#include "log_level.h"
#include <string>
#include <chrono>

struct LogEntry {
    LogLevel level;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::string file;
    int line;
    std::string function;

    LogEntry(
        LogLevel lvl,
        const std::string& msg,
        const std::string& file = "",
        int line = 0,
        const std::string& func = ""
    ) : level(lvl),
        message(msg),
        timestamp(std::chrono::system_clock::now()),
        file(file),
        line(line),
        function(func) {
    }
};