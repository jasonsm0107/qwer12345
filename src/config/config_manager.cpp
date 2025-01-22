#include "config_manager.h"
#include "../core/core.h"
#include <format>

void ConfigManager::updateSetting(const std::string& key, const SettingValue& value) {
    std::lock_guard<std::mutex> guard(this->settingsMutex);
    this->settings[key] = value;
    auto* logger = PixelmonUtilsCore::getInstance().getLogger();

    std::string valueStr;
    if (std::holds_alternative<bool>(value)) {
        valueStr = std::get<bool>(value) ? "true" : "false";
    }
    else if (std::holds_alternative<int>(value)) {
        valueStr = std::to_string(std::get<int>(value));
    }
    else if (std::holds_alternative<std::string>(value)) {
        valueStr = std::get<std::string>(value);
    }

    logger->debug("Updated setting: {} = {}", key, valueStr);
}

SettingValue ConfigManager::getSetting(const std::string& key) const {
    std::lock_guard<std::mutex> guard(this->settingsMutex);
    auto it = this->settings.find(key);
    if (it != this->settings.end()) {
        return it->second;
    }
    throw std::runtime_error("Setting not found: " + key);
}

bool ConfigManager::getBoolSetting(const std::string& key) const {
    std::lock_guard<std::mutex> guard(this->settingsMutex);
    auto it = this->settings.find(key);
    if (it != this->settings.end() && std::holds_alternative<bool>(it->second)) {
        return std::get<bool>(it->second);
    }
    throw std::runtime_error("Boolean setting not found: " + key);
}

int ConfigManager::getIntSetting(const std::string& key) const {
    std::lock_guard<std::mutex> guard(this->settingsMutex);
    auto it = this->settings.find(key);
    if (it != this->settings.end() && std::holds_alternative<int>(it->second)) {
        return std::get<int>(it->second);
    }
    throw std::runtime_error("Integer setting not found: " + key);
}

std::string ConfigManager::getStringSetting(const std::string& key) const {
    std::lock_guard<std::mutex> guard(this->settingsMutex);
    auto it = this->settings.find(key);
    if (it != this->settings.end() && std::holds_alternative<std::string>(it->second)) {
        return std::get<std::string>(it->second);
    }
    throw std::runtime_error("String setting not found: " + key);
}

void ConfigManager::cleanup() {
    std::lock_guard<std::mutex> guard(this->settingsMutex);
    this->settings.clear();
}