#pragma once
#include <unordered_map>
#include "../common/singleton.hpp"
#include "../logging/logger.h"

class InputManager : public Singleton<InputManager> {
public:
    void handleKeyPress(int keyCode);
    void handleKeyRelease(int keyCode);
    bool isKeyPressed(int keyCode) const;

private:
    friend class Singleton<InputManager>;
    InputManager() = default;

    std::unordered_map<int, bool> keyStates;
    Logger& logger = Logger::getInstance();
};