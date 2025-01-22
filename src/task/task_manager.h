#pragma once
#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <functional>

class Task {
public:
    virtual ~Task() = default;
    virtual void update() = 0;
    virtual bool isCompleted() const = 0;
};

class TaskManager {
private:
    static std::unique_ptr<TaskManager> instance;
    std::vector<std::unique_ptr<Task>> tasks;
    std::mutex taskMutex;
    bool initialized;

    TaskManager();

public:
    static TaskManager& getInstance();

    void initialize();
    void update();
    void cleanup();

    void addTask(std::unique_ptr<Task> task);
    void removeCompletedTasks();

    bool isInitialized() const { return initialized; }
};