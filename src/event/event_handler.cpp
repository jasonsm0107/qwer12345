#include "event_handler.h"
#include "../utils/jni_utils.h"
#include <stdexcept>

std::unique_ptr<EventHandler> EventHandler::instance = nullptr;

EventHandler::EventHandler()
    : jvm(nullptr), eventBusClass(nullptr), forgeBus(nullptr), modBus(nullptr),
    postEventMethod(nullptr), initialized(false) {
}

EventHandler& EventHandler::getInstance() {
    if (!instance) {
        instance = std::unique_ptr<EventHandler>(new EventHandler());
    }
    return *instance;
}

bool EventHandler::initialize(JNIEnv* env, jobject modBus, jobject forgeBus) {
    std::lock_guard<std::mutex> lock(eventMutex);

    if (initialized) {
        return true;
    }

    if (!initializeJNIRefs(env, modBus, forgeBus)) {
        cleanup(env);
        return false;
    }

    initialized = true;
    return true;
}

bool EventHandler::initializeJNIRefs(JNIEnv* env, jobject modBus, jobject forgeBus) {
    if (env->GetJavaVM(&jvm) != JNI_OK) {
        return false;
    }

    jclass localEventBusClass = env->FindClass("net/minecraftforge/eventbus/api/IEventBus");
    if (!localEventBusClass) {
        return false;
    }

    eventBusClass = (jclass)env->NewGlobalRef(localEventBusClass);
    env->DeleteLocalRef(localEventBusClass);

    this->modBus = env->NewGlobalRef(modBus);
    this->forgeBus = env->NewGlobalRef(forgeBus);

    postEventMethod = env->GetMethodID(eventBusClass, "post", "(Ljava/lang/Object;)Z");
    if (!postEventMethod) {
        return false;
    }

    return true;
}

void EventHandler::handleForgeEvent(JNIEnv* env, jobject event) {
    std::lock_guard<std::mutex> lock(eventMutex);

    if (!initialized || !event) {
        return;
    }

    env->CallBooleanMethod(forgeBus, postEventMethod, event);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

void EventHandler::cleanup(JNIEnv* env) {
    std::lock_guard<std::mutex> lock(eventMutex);

    if (!initialized) {
        return;
    }

    cleanupJNIRefs(env);
    initialized = false;
}

void EventHandler::cleanupJNIRefs(JNIEnv* env) {
    if (eventBusClass) {
        env->DeleteGlobalRef(eventBusClass);
        eventBusClass = nullptr;
    }
    if (modBus) {
        env->DeleteGlobalRef(modBus);
        modBus = nullptr;
    }
    if (forgeBus) {
        env->DeleteGlobalRef(forgeBus);
        forgeBus = nullptr;
    }
    postEventMethod = nullptr;
    jvm = nullptr;
}