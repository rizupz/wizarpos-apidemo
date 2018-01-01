#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <jni.h>

#include <time.h>

#include "hal_sys_log.h"
#include "fingerprint_jni_interface.h"
#include "fingerprint_interface.h"

const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/FingerPrintInterface";
//	+add by pengli
const char* g_pJNIREG_CLASS_INTERNAL =
		"com/wizarpos/internal/jniinterface/FingerPrintInterface";
//	-add by pengli

typedef struct fingerprint_interface {
	fp_open open;
	fp_close close;
	fp_get_fea get_fea;
	fp_getLastImage get_last_image;
	fp_match match;
	fp_cancel cancel;
	void* pHandle;
} FINGERPRINT_INSTANCE;

static FINGERPRINT_INSTANCE* g_pFingerPrintInstance = NULL;
static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;

int native_fingerprint_open(JNIEnv * env, jclass obj) {
	hal_sys_info("+ native_fingerprint_open");
	int nResult = ERR_HAS_OPENED;
	if (g_pFingerPrintInstance == NULL) {
		//										  libwizarposDriver.so
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s\n", dlerror());
			return ERR_NORMAL;
		}
		g_pFingerPrintInstance = new FINGERPRINT_INSTANCE();
		g_pFingerPrintInstance->pHandle = pHandle;
		char * methodName;
		if (NULL == (g_pFingerPrintInstance->open = (fp_open) dlsym(pHandle, methodName = "fp_open"))
				|| NULL == (g_pFingerPrintInstance->close = (fp_close) dlsym(pHandle, methodName = "fp_close"))
				|| NULL == (g_pFingerPrintInstance->get_fea = (fp_get_fea) dlsym(pHandle, methodName = "fp_get_fea"))
				|| NULL == (g_pFingerPrintInstance->get_last_image = (fp_getLastImage) dlsym(pHandle, methodName = "fp_getLastImage"))
				|| NULL == (g_pFingerPrintInstance->match = (fp_match) dlsym(pHandle, methodName = "fp_match"))
				|| NULL == (g_pFingerPrintInstance->cancel = (fp_cancel) dlsym(pHandle, methodName = "fp_cancel"))) {
			hal_sys_error("can't find %s", methodName);
			nResult = ERR_NO_IMPLEMENT;
			goto fingerprint_init_clean;
		}
		nResult = g_pFingerPrintInstance->open();

		if (nResult < 0) {
			goto fingerprint_init_clean;
		}
	}
	hal_sys_info("- native_fingerprint_open, result = %d", nResult);
	return nResult;

	fingerprint_init_clean:
		hal_sys_info("fingerprint_init_clean");
		dlclose(g_pFingerPrintInstance->pHandle);
		delete g_pFingerPrintInstance;
		g_pFingerPrintInstance = NULL;
		hal_sys_info("- native_fingerprint_open, result = %d", nResult);
	return nResult;
}

int native_fingerprint_close(JNIEnv * env, jclass obj) {
	hal_sys_info("+ native_fingerprint_close");
	int nResult = ERR_NORMAL;
	if (g_pFingerPrintInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pFingerPrintInstance->close();
	dlclose(g_pFingerPrintInstance->pHandle);
	delete g_pFingerPrintInstance;
	g_pFingerPrintInstance = NULL;
	hal_sys_info("- native_fingerprint_close, result = %d", nResult);
	return nResult;
}

int native_fingerprint_get_fea(JNIEnv * env, jclass obj, jbyteArray arryFea,
		jint nFeaLength, jintArray pRealFeaLength, jint n_TimeOut_S) {
	hal_sys_info("+ native_fingerprint_get_fea");
	int nResult = ERR_NORMAL;
	if (g_pFingerPrintInstance == NULL)
		return ERR_NOT_OPENED;

	jbyte* pFeaBuffer = env->GetByteArrayElements(arryFea, NULL);
	jint* iRealFeaLength = env->GetIntArrayElements(pRealFeaLength, NULL);
	nResult = g_pFingerPrintInstance->get_fea((unsigned char*) pFeaBuffer,
			nFeaLength, iRealFeaLength, n_TimeOut_S);
	env->ReleaseByteArrayElements(arryFea, pFeaBuffer, 0);
	env->ReleaseIntArrayElements(pRealFeaLength, iRealFeaLength, 0);
	hal_sys_info("- native_fingerprint_get_fea, result = %d", nResult);
	return nResult;
}

int native_fingerprint_get_last_image(JNIEnv * env, jclass obj,
		jbyteArray pImgBuffer, jint nImgLength, jintArray pRealImaLength,
		jintArray pImgWidth, jintArray pImgHeight) {
	hal_sys_info("+ native_fingerprint_get_last_image");
	int nResult = ERR_NORMAL;
	if (g_pFingerPrintInstance == NULL)
		return ERR_NOT_OPENED;
	jbyte* bImgBuffer = env->GetByteArrayElements(pImgBuffer, NULL);
	jint* iRealImaLength = env->GetIntArrayElements(pRealImaLength, NULL);
	jint* iImgWidth = env->GetIntArrayElements(pImgWidth, NULL);
	jint* iImgHeight = env->GetIntArrayElements(pImgHeight, NULL);
	nResult = g_pFingerPrintInstance->get_last_image(
			(unsigned char*) bImgBuffer, nImgLength, iRealImaLength, iImgWidth,
			iImgHeight);
	env->ReleaseByteArrayElements(pImgBuffer, bImgBuffer, 0);
	env->ReleaseIntArrayElements(pRealImaLength, iRealImaLength, 0);
	env->ReleaseIntArrayElements(pImgWidth, iImgWidth, 0);
	env->ReleaseIntArrayElements(pImgHeight, iImgHeight, 0);
	hal_sys_info("- native_fingerprint_get_last_image, result = %d", nResult);
	return nResult;
}

int native_fingerprint_match(JNIEnv * env, jclass obj, jbyteArray pFeaBuffer1,
		jint nFea1Length, jbyteArray pFeaBuffer2, jint nFea2Length) {
	hal_sys_info("+ native_fingerprint_match");
	int nResult = ERR_NORMAL;
	if (g_pFingerPrintInstance == NULL)
		return ERR_NOT_OPENED;
	jbyte* bFeaBuffer1 = env->GetByteArrayElements(pFeaBuffer1, NULL);
	jbyte* bFeaBuffer2 = env->GetByteArrayElements(pFeaBuffer2, NULL);
	nResult = g_pFingerPrintInstance->match((unsigned char*) bFeaBuffer1,
			nFea1Length, (unsigned char*) bFeaBuffer2, nFea2Length);
	env->ReleaseByteArrayElements(pFeaBuffer1, bFeaBuffer1, 0);
	env->ReleaseByteArrayElements(pFeaBuffer2, bFeaBuffer2, 0);
	hal_sys_info("- native_fingerprint_match, result = %d", nResult);
	return nResult;
}

int native_fingerprint_cancel(JNIEnv * env, jclass obj, jint index) {
	hal_sys_info("+ native_fingerprint_cancel");
	int nResult = ERR_NORMAL;
	if (g_pFingerPrintInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pFingerPrintInstance->cancel();
	hal_sys_info("- native_fingerprint_cancel, result = %d", nResult);
	return nResult;
}

static JNINativeMethod g_Methods[] = {
		{ "open", 				"()I",					(void*) native_fingerprint_open },
		{ "close", 				"()I",					(void*) native_fingerprint_close },
		{ "getFea", 			"([BI[II)I",			(void*) native_fingerprint_get_fea },
		{ "getLastImage", 		"([BI[I[I[I)I",			(void*) native_fingerprint_get_last_image },
		{ "match", 				"([BI[BI)I",			(void*) native_fingerprint_match },
		{ "cancel", 			"()I",					(void*) native_fingerprint_cancel }, };

const char* fingerprint_get_class_name() {
	return g_pJNIREG_CLASS;
}
const char* get_class_name_internal() {
	return g_pJNIREG_CLASS_INTERNAL;
}

JNINativeMethod* fingerprint_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
