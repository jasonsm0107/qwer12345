#pragma once
#include <jni.h>
#include <memory>
#include <mutex>

class EventHandler {
private:
    static std::unique_ptr<EventHandler> instance;
    JavaVM* jvm;
    jclass eventBusClass;
    jobject forgeBus;
    jobject modBus;
    jmethodID postEventMethod;
    bool initialized;
    std::mutex eventMutex;

    EventHandler();

public:
    static EventHandler& getInstance();

    bool initialize(JNIEnv* env, jobject modBus, jobject forgeBus);
    void cleanup(JNIEnv* env);
    void handleForgeEvent(JNIEnv* env, jobject event);

    bool isInitialized() const { return initialized; }

private:
    bool initializeJNIRefs(JNIEnv* env, jobject modBus, jobject forgeBus);
    void cleanupJNIRefs(JNIEnv* env);
};