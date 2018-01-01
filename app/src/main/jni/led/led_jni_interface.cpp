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
#include "led_jni_interface.h"
#include "led_service_interface.h"

const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/LEDInterface";
//	+add by pengli
const char* g_pJNIREG_CLASS_INTERNAL = "com/wizarpos/internal/jniinterface/LEDInterface";
//	-add by pengli


typedef struct led_interface {
	LED_OPEN open;
	LED_CLOSE close;
	LED_ON turn_on;
	LED_OFF turn_off;
	LED_GET_STATUS get_status;
	void* pHandle;
} LED_INSTANCE;

static LED_INSTANCE* g_pLEDInstance = NULL;
static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;

int native_led_open(JNIEnv * env, jclass obj) {
	hal_sys_info("+ native_led_open");
	int nResult = ERR_HAS_OPENED;
	if (g_pLEDInstance == NULL) {
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s\n", dlerror());
			return ERR_NORMAL;
		}
		g_pLEDInstance = new LED_INSTANCE();
		g_pLEDInstance->pHandle = pHandle;

		char * methodName;
		if (NULL == (g_pLEDInstance->open = (LED_OPEN) dlsym(pHandle, methodName = "led_open"))
				|| NULL == (g_pLEDInstance->close = (LED_CLOSE) dlsym(pHandle, methodName = "led_close"))
				|| NULL == (g_pLEDInstance->turn_on = (LED_ON) dlsym(pHandle, methodName = "led_on"))
				|| NULL == (g_pLEDInstance->turn_off = (LED_OFF) dlsym(pHandle, methodName = "led_off"))
				|| NULL == (g_pLEDInstance->get_status = (LED_GET_STATUS) dlsym( pHandle, methodName = "led_get_status"))) {
			hal_sys_error("can't find %s", methodName);
			nResult = ERR_NO_IMPLEMENT;
			goto led_open_clean;
		}
		nResult = g_pLEDInstance->open();

		if (nResult < 0) {
			goto led_open_clean;
		}
	}
	hal_sys_info("- native_led_open, result = %d", nResult);
	return nResult;

	led_open_clean:
		hal_sys_info("led_open_clean");
		dlclose(g_pLEDInstance->pHandle);
		delete g_pLEDInstance;
		g_pLEDInstance = NULL;
	hal_sys_info("- native_led_open, result = %d", nResult);
	return nResult;
}

int native_led_close(JNIEnv * env, jclass obj) {
	hal_sys_info("+ native_led_close");
	int nResult = ERR_NORMAL;
	if (g_pLEDInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pLEDInstance->close();
	dlclose(g_pLEDInstance->pHandle);
	delete g_pLEDInstance;
	g_pLEDInstance = NULL;
	hal_sys_info("- native_led_close, result = %d", nResult);
	return nResult;
}

int native_led_on(JNIEnv * env, jclass obj, jint index) {
	hal_sys_info("+ native_led_on");
	int nResult = ERR_NORMAL;
	if (g_pLEDInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pLEDInstance->turn_on(index);
	hal_sys_info("- native_led_on, result = %d", nResult);
	return nResult;
}

int native_led_off(JNIEnv * env, jclass obj, jint index) {
	hal_sys_info("+ native_led_off");
	int nResult = ERR_NORMAL;
	if (g_pLEDInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pLEDInstance->turn_off(index);
	hal_sys_info("- native_led_off, result = %d", nResult);
	return nResult;
}

int native_led_get_status(JNIEnv * env, jclass obj, jint index) {
	hal_sys_info("+ native_led_get_status");
	int nResult = ERR_NORMAL;
	if (g_pLEDInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pLEDInstance->get_status(index);
	hal_sys_info("- native_led_get_status, result = %d", nResult);
	return nResult;
}

static JNINativeMethod g_Methods[] = {
		{ "open", 			"()I", 		(void*) native_led_open },
		{ "close", 			"()I",		(void*) native_led_close },
		{ "turnOn", 		"(I)I",		(void*) native_led_on },
		{ "turnOff", 		"(I)I",		(void*) native_led_off },
		{ "getStatus", 		"(I)I",		(void*) native_led_get_status }, };

const char* led_get_class_name() {
	return g_pJNIREG_CLASS;
}
const char* get_class_name_internal()
{
	return g_pJNIREG_CLASS_INTERNAL;
}

JNINativeMethod* led_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
