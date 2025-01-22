#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <mutex>
#include <memory>
#include <vector>
#include "../logging/logger.h"

using StateValue = std::variant<bool, int, std::string>;

class StateManager {
private:
    static StateManager* instance;
    std::unordered_map<std::string, StateValue> states;
    std::vector<std::string> stateHistory;  // ���� ���� �����丮 �߰�
    mutable std::mutex stateMutex;
    Logger& logger;
    size_t maxHistorySize = 100;  // �����丮 �ִ� ũ��

    StateManager();

public:
    static StateManager& getInstance();
    static void destroyInstance();

    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;

    void setState(const std::string& key, const StateValue& value);
    StateValue getState(const std::string& key) const;
    bool hasState(const std::string& key) const;
    void removeState(const std::string& key);
    void clearStates();

    // �߰��� �޼����
    std::vector<std::string> getStateHistory() const;
    void clearHistory();
    void setMaxHistorySize(size_t size);
    bool tryGetState(const std::string& key, StateValue& outValue) const;  // ���� ���� ����
};