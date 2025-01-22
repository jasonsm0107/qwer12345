#pragma once
#pragma once

#include <jni.h>
#include "../core/core.h"

// JNI ���� ����
#define JNI_VERSION JNI_VERSION_1_6

#ifdef __cplusplus
extern "C" {
#endif

	// �⺻ �ʱ�ȭ/���� �Լ�
	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);

	// NativeBridge �޼��� ���ε�
	JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_initialize
	(JNIEnv*, jclass);

	JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_cleanup
	(JNIEnv*, jclass);

	JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_initializeResources
	(JNIEnv*, jclass, jstring);

	JNIEXPORT void JNICALL Java_net_sloth_NativeBridge_updateSettings
	(JNIEnv*, jclass, jstring, jobject);

	JNIEXPORT jboolean JNICALL Java_net_sloth_NativeBridge_isInitialized
	(JNIEnv*, jclass);

#ifdef __cplusplus
}
#endif