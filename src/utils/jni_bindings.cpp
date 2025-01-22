#include "jni_bindings.h"
#include "../core/core.h"
#include "../utils/jni_utils.h"
#include <string>
#include "../logging/logger.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION) != JNI_OK) {
        return JNI_ERR;
    }

    // 여기서 필요한 전역 참조나 캐시 초기화를 수행할 수 있습니다
    return JNI_VERSION;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION) != JNI_OK) {
        return;
    }

    // 전역 참조 해제 및 정리 작업
    PixelmonUtilsCore::getInstance().cleanup();
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_initializeLogger
(JNIEnv* env, jclass, jstring logPath, jint logLevel) {
    try {
        const char* path = env->GetStringUTFChars(logPath, nullptr);
        LogLevel level = static_cast<LogLevel>(logLevel);

        if (!Logger::getInstance().initialize(path, level)) {
            throwJavaException(env, "java/lang/RuntimeException", "Failed to initialize logger");
        }

        env->ReleaseStringUTFChars(logPath, path);
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_log
(JNIEnv* env, jclass, jint level, jstring message) {
    try {
        const char* msg = env->GetStringUTFChars(message, nullptr);
        LogLevel logLevel = static_cast<LogLevel>(level);

        Logger::getInstance().log(logLevel, msg);

        env->ReleaseStringUTFChars(message, msg);
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_initialize
(JNIEnv* env, jclass) {
    try {
        PixelmonUtilsCore::getInstance().initialize(env);
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_cleanup
(JNIEnv* env, jclass) {
    try {
        PixelmonUtilsCore::getInstance().cleanup();
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_initializeResources
(JNIEnv* env, jclass, jstring configPath) {
    try {
        const char* nativeConfigPath = env->GetStringUTFChars(configPath, nullptr);
        auto* resourceManager = PixelmonUtilsCore::getInstance().getResourceManager();
        resourceManager->initialize(nativeConfigPath);
        env->ReleaseStringUTFChars(configPath, nativeConfigPath);
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_updateSettings
(JNIEnv* env, jclass, jstring key, jobject value) {
    try {
        const char* nativeKey = env->GetStringUTFChars(key, nullptr);
        auto* configManager = PixelmonUtilsCore::getInstance().getConfigManager();

        // value 객체의 타입을 확인하고 적절한 C++ 타입으로 변환
        jclass valueClass = env->GetObjectClass(value);

        if (env->IsInstanceOf(value, env->FindClass("java/lang/Boolean"))) {
            jmethodID booleanValue = env->GetMethodID(valueClass, "booleanValue", "()Z");
            jboolean bValue = env->CallBooleanMethod(value, booleanValue);
            configManager->updateSetting(nativeKey, static_cast<bool>(bValue));
        }
        else if (env->IsInstanceOf(value, env->FindClass("java/lang/Integer"))) {
            jmethodID intValue = env->GetMethodID(valueClass, "intValue", "()I");
            jint iValue = env->CallIntMethod(value, intValue);
            configManager->updateSetting(nativeKey, static_cast<int>(iValue));
        }
        else if (env->IsInstanceOf(value, env->FindClass("java/lang/String"))) {
            jstring sValue = static_cast<jstring>(value);
            const char* nativeValue = env->GetStringUTFChars(sValue, nullptr);
            configManager->updateSetting(nativeKey, std::string(nativeValue));
            env->ReleaseStringUTFChars(sValue, nativeValue);
        }

        env->ReleaseStringUTFChars(key, nativeKey);
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT jboolean JNICALL Java_net_sloth_NativeBridge_isInitialized
(JNIEnv*, jclass) {
    return static_cast<jboolean>(PixelmonUtilsCore::getInstance().isInitialized());
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_registerEventBuses
(JNIEnv* env, jclass, jobject modBus, jobject forgeBus) {
    try {
        EventHandler::getInstance().initialize(env, modBus, forgeBus);
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_initializeTasks
(JNIEnv*, jclass) {
    try {
        TaskManager::getInstance().initialize();
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_updateTasks
(JNIEnv*, jclass) {
    try {
        TaskManager::getInstance().update();
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_cleanupTasks
(JNIEnv*, jclass) {
    try {
        TaskManager::getInstance().cleanup();
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}

JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_handleForgeEvent
(JNIEnv* env, jclass, jobject event) {
    try {
        EventHandler::getInstance().handleForgeEvent(env, event);
    }
    catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
    }
}