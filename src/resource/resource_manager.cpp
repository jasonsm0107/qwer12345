#include "resource_manager.h"
#include "../core/core.h"
#include <filesystem>

bool ResourceManager::initialize(const std::string& path) {
    if (initialized) {
        return true;
    }

    try {
        configPath = path;
        std::filesystem::create_directories(configPath);
        initialized = true;

        auto* logger = PixelmonUtilsCore::getInstance().getLogger();
        logger->info("Resource manager initialized with path: {}", configPath);
        return true;
    }
    catch (const std::exception& e) {
        auto* logger = PixelmonUtilsCore::getInstance().getLogger();
        logger->error("Failed to initialize resource manager: {}", e.what());
        return false;
    }
}

void ResourceManager::cleanup() {
    if (!initialized) {
        return;
    }

    try {
        // Cleanup resources if needed
        initialized = false;

        auto* logger = PixelmonUtilsCore::getInstance().getLogger();
        logger->info("Resource manager cleaned up successfully");
    }
    catch (const std::exception& e) {
        auto* logger = PixelmonUtilsCore::getInstance().getLogger();
        logger->error("Error during resource manager cleanup: {}", e.what());
    }
}