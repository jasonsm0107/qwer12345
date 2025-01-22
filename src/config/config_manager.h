#pragma once

#include <unordered_map>
#include <variant>
#include <string>
#include <mutex>
#include <stdexcept>

// 전방 선언
class ConfigManager;

using SettingValue = std::variant<bool, int, std::string>;

class ConfigManager {
public:
    ConfigManager() = default;
    ~ConfigManager() = default;

    void updateSetting(const std::string& key, const SettingValue& value);
    SettingValue getSetting(const std::string& key) const;
    bool getBoolSetting(const std::string& key) const;
    int getIntSetting(const std::string& key) const;
    std::string getStringSetting(const std::string& key) const;
    void cleanup();

private:
    std::unordered_map<std::string, SettingValue> settings;
    mutable std::mutex settingsMutex;
};