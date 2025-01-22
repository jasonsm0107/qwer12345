#include "task_manager.h"
#include <algorithm>

std::unique_ptr<TaskManager> TaskManager::instance = nullptr;

TaskManager::TaskManager() : initialized(false) {}

TaskManager& TaskManager::getInstance() {
    if (!instance) {
        instance = std::unique_ptr<TaskManager>(new TaskManager());
    }
    return *instance;
}

void TaskManager::initialize() {
    std::lock_guard<std::mutex> lock(taskMutex);

    if (initialized) {
        return;
    }

    tasks.clear();
    initialized = true;
}

void TaskManager::update() {
    std::lock_guard<std::mutex> lock(taskMutex);

    if (!initialized) {
        return;
    }

    for (auto& task : tasks) {
        if (task && !task->isCompleted()) {
            task->update();
        }
    }

    removeCompletedTasks();
}

void TaskManager::addTask(std::unique_ptr<Task> task) {
    std::lock_guard<std::mutex> lock(taskMutex);

    if (!initialized || !task) {
        return;
    }

    tasks.push_back(std::move(task));
}

void TaskManager::removeCompletedTasks() {
    tasks.erase(
        std::remove_if(
            tasks.begin(),
            tasks.end(),
            [](const auto& task) { return task->isCompleted(); }
        ),
        tasks.end()
    );
}

void TaskManager::cleanup() {
    std::lock_guard<std::mutex> lock(taskMutex);

    if (!initialized) {
        return;
    }

    tasks.clear();
    initialized = false;
}