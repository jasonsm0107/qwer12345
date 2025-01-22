#include "input_manager.h"

void InputManager::handleKeyPress(int keyCode) {
    keyStates[keyCode] = true;
    logger.debug("Key pressed: {}", keyCode);
}

void InputManager::handleKeyRelease(int keyCode) {
    keyStates[keyCode] = false;
    logger.debug("Key released: {}", keyCode);
}

bool InputManager::isKeyPressed(int keyCode) const {
    auto it = keyStates.find(keyCode);
    return it != keyStates.end() && it->second;
}