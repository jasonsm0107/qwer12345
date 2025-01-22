#pragma once
#include <jni.h>
#include <string>

// JNI ��ƿ��Ƽ �Լ���
void throwJavaException(JNIEnv* env, const char* exceptionClass, const char* message);
std::string jstringToString(JNIEnv* env, jstring jstr);
jstring stringToJstring(JNIEnv* env, const std::string& str);