#pragma once
#include <unordered_map>
#include "../../logging/logger.h"

class InputManager {
public:
    static InputManager& get() {
        static InputManager instance;
        return instance;
    }

    void handleKeyPress(int keyCode);
    void handleKeyRelease(int keyCode);
    bool isKeyPressed(int keyCode) const;

    // Delete copy constructor and assignment operator
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

private:
    InputManager() {} // Private constructor

    std::unordered_map<int, bool> keyStates;
    Logger& logger = Logger::get();
};