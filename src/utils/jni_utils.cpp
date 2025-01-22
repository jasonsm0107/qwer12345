#include "jni_utils.h"

void throwJavaException(JNIEnv* env, const char* exceptionClass, const char* message) {
    jclass cls = env->FindClass(exceptionClass);
    if (cls != nullptr) {
        env->ThrowNew(cls, message);
    }
}

std::string jstringToString(JNIEnv* env, jstring jstr) {
    if (!jstr) return "";

    const char* str = env->GetStringUTFChars(jstr, nullptr);
    if (!str) return "";

    std::string result(str);
    env->ReleaseStringUTFChars(jstr, str);

    return result;
}

jstring stringToJstring(JNIEnv* env, const std::string& str) {
    return env->NewStringUTF(str.c_str());
}