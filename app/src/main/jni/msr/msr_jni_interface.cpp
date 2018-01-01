/*
 * msr_jni_interface.cpp
 *
 *  Created on: 2013-1-6
 *      Author: s990902
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>

#include <jni.h>

#include <sys/time.h>
#include <pthread.h>

#include <deque>

#include "hal_sys_log.h"

#include "msr_interface.h"
#include "hal_sys_log.h"

static const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/MSRInterface";

//	+add by pengli
static const char* g_pJNIREG_CLASS_INTERNAL =
		"com/wizarpos/internal/jniinterface/MSRInterface";
//	-add by pengli

static jclass mcls;
static jmethodID mmid;
static JavaVM *g_jvm = NULL;

typedef struct msr_hal_interface {
	msr_open open;
	msr_close close;
	msr_register_notifier register_notifier;
	msr_unregister_notifier unregister_notifier;
	msr_get_track_error get_track_error;
	msr_get_track_data_length get_track_data_length;
	msr_get_track_data get_track_data;
	void* pHandle;
	sem_t sem;
} MSR_HAL_INSTANCE;

static MSR_HAL_INSTANCE* g_pMSRInstance = NULL;
static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;
static jint nEventID = 0;

static void msr_call_back(void* pUserData) {

	hal_sys_info("+msr_call_back");

	JNIEnv *env = NULL;
	bool needDetach = false;
	if (g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
		g_jvm->AttachCurrentThread(&env, NULL);
		needDetach = true;
	} else {
		hal_sys_error("Callback is running in java thread!!!");
	}

	env->CallStaticVoidMethod(mcls, mmid, nEventID);

	if (needDetach) {
		g_jvm->DetachCurrentThread();
	}
	hal_sys_info("-msr_call_back()");

}

int native_msr_open(JNIEnv * env, jclass obj) {
	hal_sys_info("+ native_msr_open()");
	int nResult = ERR_HAS_OPENED; //最大软件错误码
	env->GetJavaVM(&g_jvm);
	if (g_pMSRInstance == NULL) {
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_info("%s\n", dlerror());
			return ERR_NORMAL;
		}
		g_pMSRInstance = new MSR_HAL_INSTANCE();
		g_pMSRInstance->pHandle = pHandle;

		char *methodName;
		if (NULL == (g_pMSRInstance->open = (msr_open) dlsym(pHandle, methodName = "msr_open"))
				|| NULL == (g_pMSRInstance->close = (msr_close) dlsym(pHandle, methodName = "msr_close"))
				|| NULL == (g_pMSRInstance->register_notifier = (msr_register_notifier) dlsym(pHandle, methodName = "msr_register_notifier"))
				|| NULL == (g_pMSRInstance->unregister_notifier = (msr_unregister_notifier) dlsym(pHandle, methodName = "msr_unregister_notifier"))
				|| NULL == (g_pMSRInstance->get_track_error = (msr_get_track_error) dlsym(pHandle, methodName = "msr_get_track_error"))
				|| NULL == (g_pMSRInstance->get_track_data_length = (msr_get_track_data_length) dlsym(pHandle, methodName = "msr_get_track_data_length"))
				|| NULL == (g_pMSRInstance->get_track_data = (msr_get_track_data) dlsym(pHandle, methodName = "msr_get_track_data")))
		{
			hal_sys_info("can't find %s", methodName);
			nResult = ERR_NO_IMPLEMENT;
			goto msr_open_clean;
		}

		nResult = g_pMSRInstance->open();
		if (nResult < 0) {
			goto msr_open_clean;
		}
		jclass tmp = env->FindClass(g_pJNIREG_CLASS);
		//		+ add by pengli
		if (tmp == NULL) {
			env->ExceptionClear();
			tmp = env->FindClass(g_pJNIREG_CLASS_INTERNAL);
		}
		mcls = (jclass) env->NewGlobalRef(tmp);
		env->DeleteLocalRef(tmp);
		//		+ add by pengli
		mmid = env->GetStaticMethodID(mcls, "callBack", "(I)V");
		if (mmid == NULL) {
			return ERR_NORMAL;
		}
		hal_sys_info("+ native_msr_register_notifier()");
		nResult = g_pMSRInstance->register_notifier(msr_call_back, g_pMSRInstance);
		hal_sys_info("- native_msr_register_notifier(), result = %d", nResult);

	}
	hal_sys_info("- native_msr_open(), result = %d", nResult);
	return nResult;
	msr_open_clean:
		if (mcls != NULL) {
			env->DeleteGlobalRef(mcls);
		}
		hal_sys_info("msr_open_clean");
		dlclose(g_pMSRInstance->pHandle);
		delete g_pMSRInstance;
		g_pMSRInstance = NULL;
	hal_sys_info("- native_msr_open(), result = %d", nResult);
	return nResult;
}

int native_msr_close(JNIEnv * env, jclass obj) {
	hal_sys_info("+native_msr_close()");
	int nResult = ERR_NORMAL;
	if (g_pMSRInstance == NULL)
		return ERR_NOT_OPENED;

	hal_sys_info("+ native_msr_unregister_notifier()");
	nResult = g_pMSRInstance->unregister_notifier();
	hal_sys_info("- native_msr_unregister_notifier(), result = %d", nResult);

	nResult = g_pMSRInstance->close();

	env->DeleteGlobalRef(mcls);
	dlclose(g_pMSRInstance->pHandle);
	delete g_pMSRInstance;
	g_pMSRInstance = NULL;
	hal_sys_info("-native_msr_close(), result = %d", nResult);
	return nResult;
}

/*
 * Class:     huiyin_hal_msr_HALMsrInterface
 * Method:    msr_get_track_error
 * Signature: (I)I
 */
int native_msr_get_track_error(JNIEnv * env, jclass obj, jint nTrackIndex) {
	hal_sys_info("+native_msr_get_track_error()");
	int nResult = ERR_NORMAL;
	if (g_pMSRInstance == NULL)
		return ERR_NOT_OPENED;

	nResult = g_pMSRInstance->get_track_error(nTrackIndex);
	hal_sys_info("-native_msr_get_track_error(), result = %d", nResult);
	return nResult;
}

int native_msr_get_track_data_length(JNIEnv * env, jclass obj,
		jint nTrackIndex) {
	hal_sys_info("+native_msr_get_track_data_length()");
	int nResult = ERR_NORMAL;
	if (g_pMSRInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pMSRInstance->get_track_data_length(nTrackIndex);
	hal_sys_info("-native_msr_get_track_data_length(), result = %d", nResult);
	return nResult;
}

int native_msr_get_track_data(JNIEnv * env, jclass obj, jint nTrackIndex,
		jbyteArray byteArray, jint nLength) {
	hal_sys_info("+native_msr_get_track_data()");
	int nResult = ERR_NORMAL;
	if (g_pMSRInstance == NULL)
		return ERR_NOT_OPENED;
	if (byteArray == NULL)
		return ERR_INVALID_ARGUMENT;
	jbyte* pData = env->GetByteArrayElements(byteArray, NULL);
	nResult = g_pMSRInstance->get_track_data(nTrackIndex, (unsigned char*) pData, nLength);
	env->ReleaseByteArrayElements(byteArray, pData, 0);
	hal_sys_info("-native_msr_get_track_data(), result = %d", nResult);
	return nResult;
}


static JNINativeMethod g_Methods[] ={
		{ "open", 					"()I", 			(void*) native_msr_open },
		{ "close", 					"()I",			(void*) native_msr_close },
		{ "getTrackError", 			"(I)I",			(void*) native_msr_get_track_error },
		{ "getTrackDataLength",		"(I)I", 		(void*) native_msr_get_track_data_length },
		{"getTrackData", 			"(I[BI)I", 		(void*) native_msr_get_track_data }, };

const char* msr_get_class_name() {
	return g_pJNIREG_CLASS;
}
const char* get_class_name_internal() {
	return g_pJNIREG_CLASS_INTERNAL;
}

JNINativeMethod* msr_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
