#include "state_manager.h"
#include <stdexcept>

StateManager* StateManager::instance = nullptr;

StateManager::StateManager() : logger(Logger::getInstance()) {}

StateManager& StateManager::getInstance() {
    if (!instance) {
        instance = new StateManager();
    }
    return *instance;
}

void StateManager::destroyInstance() {
    delete instance;
    instance = nullptr;
}

void StateManager::setState(const std::string& key, const StateValue& value) {
    std::lock_guard<std::mutex> lock(stateMutex);
    states[key] = value;

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

    logger.debug("State updated: {} = {}", key, valueStr);
}

StateValue StateManager::getState(const std::string& key) const {
    std::lock_guard<std::mutex> lock(stateMutex);
    auto it = states.find(key);
    if (it != states.end()) {
        return it->second;
    }
    throw std::runtime_error("State not found: " + key);
}

bool StateManager::hasState(const std::string& key) const {
    std::lock_guard<std::mutex> lock(stateMutex);
    return states.find(key) != states.end();
}

void StateManager::removeState(const std::string& key) {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (states.erase(key) > 0) {
        logger.debug("State removed: {}", key);
    }
}

void StateManager::clearStates() {
    std::lock_guard<std::mutex> lock(stateMutex);
    states.clear();
    logger.debug("All states cleared");
}