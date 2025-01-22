#pragma once
#include <jni.h>
#include <memory>
#include "../logging/logger.h"
#include "state/state_manager.h"
#include "input/input_manager.h"
#include "../config/config_manager.h"
#include "../resource/resource_manager.h"

class PixelmonUtilsCore {
private:
    static std::unique_ptr<PixelmonUtilsCore> instance;
    bool initialized = false;
    JavaVM* jvm = nullptr;

    // Core components
    std::unique_ptr<Logger> logger;
    std::unique_ptr<ConfigManager> configManager;
    std::unique_ptr<ResourceManager> resourceManager;

public:
    PixelmonUtilsCore() = default;
    ~PixelmonUtilsCore() = default;

    static PixelmonUtilsCore& getInstance() {
        if (!instance) {
            instance = std::unique_ptr<PixelmonUtilsCore>(new PixelmonUtilsCore());
        }
        return *instance;
    }

    bool initialize(JNIEnv* env);
    void cleanup();

    // Core getters
    Logger* getLogger() { return logger.get(); }
    ConfigManager* getConfigManager() { return configManager.get(); }
    ResourceManager* getResourceManager() { return resourceManager.get(); }
    StateManager& getStateManager() { return StateManager::getInstance(); }
    InputManager& getInputManager() { return InputManager::getInstance(); }

    // JNI related
    JavaVM* getJavaVM() const { return jvm; }
    JNIEnv* getJNIEnv() const;

    // State checks
    bool isInitialized() const { return initialized; }

    // Delete copy constructor and assignment operator
    PixelmonUtilsCore(const PixelmonUtilsCore&) = delete;
    PixelmonUtilsCore& operator=(const PixelmonUtilsCore&) = delete;

private:
    bool initializeJNI(JNIEnv* env);
    bool initializeComponents(JNIEnv* env);
    void cleanupComponents();
};