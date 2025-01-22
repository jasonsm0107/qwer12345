#pragma once
#include <string>
#include <memory>

class ResourceManager {
private:
    std::string configPath;
    bool initialized;

public:
    ResourceManager() : initialized(false) {}
    bool initialize(const std::string& path);
    void cleanup();
    const std::string& getConfigPath() const { return configPath; }
};