#include "core.h"
#include <stdexcept>

std::unique_ptr<PixelmonUtilsCore> PixelmonUtilsCore::instance = nullptr;

bool PixelmonUtilsCore::initialize(JNIEnv* env) {
    if (initialized) {
        return true;
    }

    try {
        // Store JavaVM
        if (env->GetJavaVM(&jvm) != JNI_OK) {
            throw std::runtime_error("Failed to get JavaVM pointer");
        }

        // Initialize components
        logger = std::make_unique<Logger>(env);
        configManager = std::make_unique<ConfigManager>();
        resourceManager = std::make_unique<ResourceManager>();

        initialized = true;
        logger->info("PixelmonUtils native library initialized successfully");
        return true;
    }
    catch (const std::exception& e) {
        if (logger) {
            logger->error("Failed to initialize PixelmonUtils: {}", e.what());
        }
        return false;
    }
}

void PixelmonUtilsCore::cleanup() {
    if (!initialized) {
        return;
    }

    try {
        if (resourceManager) resourceManager->cleanup();
        if (configManager) configManager->cleanup();
        if (logger) logger->info("PixelmonUtils native library cleaned up successfully");

        resourceManager.reset();
        configManager.reset();
        logger.reset();

        initialized = false;
    }
    catch (const std::exception& e) {
        if (logger) {
            logger->error("Error during cleanup: {}", e.what());
        }
    }
}