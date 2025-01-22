#pragma once
#include <jni.h>
#include <memory>
#include <string>
#include "../config/config_manager.h"
#include "../resource/resource_manager.h"
#include "../logging/logger.h"

class PixelmonUtilsCore {
private:
    static std::unique_ptr<PixelmonUtilsCore> instance;
    JavaVM* jvm;
    bool initialized;

    std::unique_ptr<ConfigManager> configManager;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<Logger> logger;

    PixelmonUtilsCore() : jvm(nullptr), initialized(false) {}

public:
    static PixelmonUtilsCore& getInstance() {
        if (!instance) {
            instance = std::unique_ptr<PixelmonUtilsCore>(new PixelmonUtilsCore());
        }
        return *instance;
    }

    bool initialize(JNIEnv* env);
    void cleanup();
    bool isInitialized() const { return initialized; }

    ConfigManager* getConfigManager() { return configManager.get(); }
    ResourceManager* getResourceManager() { return resourceManager.get(); }
    Logger* getLogger() { return logger.get(); }
};