#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#include <jni.h>

#include "hal_sys_log.h"
#include "customer_display_jni_interface.h"
#include "customer_display_interface.h"

const char* g_pJNIREG_CLASS =
		"com/cloudpos/jniinterface/SecondaryDisplayInterface";

//	+add by pengli
static const char* g_pJNIREG_CLASS_INTERNAL =
		"com/wizarpos/internal/jniinterface/SecondaryDisplayInterface";
//	-add by pengli

typedef struct customer_display_hal_interface {
	cusdisplay_open_ex open_ex;
	cusdisplay_open open;
	cusdisplay_write_picture_data write_pic;
	cusdisplay_ctrl_devs ctrl_devs;
	cusdisplay_close close;

	int portHandle;
	void* pSoHandle;
} CUSTOMER_DISPLAY_HAL_INSTANCE;

static CUSTOMER_DISPLAY_HAL_INSTANCE* g_pCusDisplayInstance = NULL;

int native_customer_display_open(JNIEnv* env, jclass obj) {
	int nResult = -1;
	void* pCusHandle = NULL;
	int nErrorCode = -255;
	hal_sys_info("+ native_customer_display_open()");
	if (g_pCusDisplayInstance == NULL) {
		void* pHandle = dlopen("libwizarposHAL.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s\n", dlerror());
			return -1;
		}

		g_pCusDisplayInstance = new CUSTOMER_DISPLAY_HAL_INSTANCE();
		g_pCusDisplayInstance->portHandle = -1;
		g_pCusDisplayInstance->open_ex = (cusdisplay_open_ex) dlsym(pHandle,
				"customer_display_open_ex");
		if (g_pCusDisplayInstance->open_ex == NULL) {
			hal_sys_error("can't find customer_display_open_ex");
			goto customer_display_init_clean;
		}
		g_pCusDisplayInstance->open = (cusdisplay_open) dlsym(pHandle,
				"customer_display_open");
		if (g_pCusDisplayInstance->open == NULL) {
			hal_sys_error("can't find customer_display_open");
			goto customer_display_init_clean;
		}
		g_pCusDisplayInstance->close = (cusdisplay_close) dlsym(pHandle,
				"customer_display_close");
		if (g_pCusDisplayInstance->close == NULL) {
			hal_sys_error("can't find customer_display_close");
			goto customer_display_init_clean;
		}

		g_pCusDisplayInstance->write_pic =
				(cusdisplay_write_picture_data) dlsym(pHandle,
						"customer_display_write_picture_data");
		if (g_pCusDisplayInstance->write_pic == NULL) {
			hal_sys_error("can't find customer_display_write_picture_data");
			goto customer_display_init_clean;
		}

		g_pCusDisplayInstance->ctrl_devs = (cusdisplay_ctrl_devs) dlsym(pHandle,
				"customer_display_ctrl_devs");
		if (g_pCusDisplayInstance->ctrl_devs == NULL) {
			hal_sys_error("can't find customer_display_ctrl_devs");
			goto customer_display_init_clean;
		}

		g_pCusDisplayInstance->pSoHandle = pHandle;
	}
	pCusHandle = g_pCusDisplayInstance->open_ex(&nErrorCode);

	hal_sys_info("- native_customer_display_open_ex, result = %d",
			(int) pCusHandle);
	if (pCusHandle == NULL) {
		hal_sys_info("- native_customer_display_open_ex, errorCode = %d",
				nErrorCode);
		goto customer_display_init_clean;
	}
	g_pCusDisplayInstance->portHandle = (int) pCusHandle;
	return 0;

	customer_display_init_clean: if (g_pCusDisplayInstance != NULL) {
		delete g_pCusDisplayInstance;
		g_pCusDisplayInstance = NULL;
		hal_sys_info("native_customer_display_open_ex error!\n");
	}
	return nErrorCode;
}

int native_customer_display_close(JNIEnv* env, jclass obj) {
	int nResult = -1;
	hal_sys_info("+ native_customer_display_close()");
	if (g_pCusDisplayInstance == NULL) {
		hal_sys_info(
				"Leave native_customer_display_close()...g_pCusDisplayInstance == NULL!\n");
		return -1;
	}
	nResult = g_pCusDisplayInstance->close(g_pCusDisplayInstance->portHandle);
	hal_sys_info("nResult = %d\n", nResult);
	dlclose(g_pCusDisplayInstance->pSoHandle);
	delete g_pCusDisplayInstance;
	g_pCusDisplayInstance = NULL;
	hal_sys_info("- native_customer_display_close(),result = %d",
			nResult);
	return nResult;
}

int native_customer_display_write_picture_data(JNIEnv* env, jclass obj,
		jint nXcoordinate, jint nYcoordinate, jint nWidth, jint nHeight,
		jbyteArray pDataBuffer, jint nDataLength) {
	int nResult = -1;
	hal_sys_info("+ native_customer_display_write_picture_data()");
	if (g_pCusDisplayInstance == NULL)
		return -1;
	jbyte* pData = env->GetByteArrayElements(pDataBuffer, NULL);
	hal_sys_info("++native_customer_display_write_picture_data()");
	nResult = g_pCusDisplayInstance->write_pic(
			g_pCusDisplayInstance->portHandle, nXcoordinate, nYcoordinate,
			nWidth, nHeight, (unsigned char *) pData, nDataLength);
	hal_sys_info("--native_customer_display_write_picture_data()");
	env->ReleaseByteArrayElements(pDataBuffer, pData, 0);
	hal_sys_info(
			"- native_customer_display_write_picture_data(),result = %d",
			nResult);
	return nResult;
}

int native_customer_display_set_background(JNIEnv* env, jclass obj,
		jint nColor) {
	int nResult = -1;
	if (g_pCusDisplayInstance == NULL)
		return -1;
	hal_sys_info("+ native_customer_display_set_background()");
	nResult = g_pCusDisplayInstance->ctrl_devs(
			g_pCusDisplayInstance->portHandle, 0x04, nColor);
	hal_sys_info(
			"- native_customer_display_set_background(),result = %d",
			nResult);
	return nResult;
}

int native_customer_display_buzzer_beep(JNIEnv* env, jclass obj) {
	int nResult = -1;
	if (g_pCusDisplayInstance == NULL)
		return -1;
	hal_sys_info("+ native_customer_display_buzzer_beep()");
	nResult = g_pCusDisplayInstance->ctrl_devs(
			g_pCusDisplayInstance->portHandle, 0x07, 0);
	hal_sys_info(
			"- native_customer_display_buzzer_beep(),result = %d",
			nResult);
	return nResult;
}

int native_customer_display_led_power(JNIEnv* env, jclass obj, jint nValue) {
	int nResult = -1;
	if (g_pCusDisplayInstance == NULL)
		return -1;
	hal_sys_info("+ native_customer_display_led_power()");
	nResult = g_pCusDisplayInstance->ctrl_devs(
			g_pCusDisplayInstance->portHandle, 0x06, nValue);
	hal_sys_info(
			"- native_customer_display_led_power(),result = %d",
			nResult);
	return nResult;
}

int native_customer_display_display_default_screen(JNIEnv* env, jclass obj) {
	int nResult = -1;
	if (g_pCusDisplayInstance == NULL)
		return -1;
	hal_sys_info("+ native_customer_display_display_default_screen()");
	nResult = g_pCusDisplayInstance->ctrl_devs(
			g_pCusDisplayInstance->portHandle, 0x05, 0);
	hal_sys_info(
			"- native_customer_display_display_default_screen(),result = %d",
			nResult);
	return nResult;
}

//int native_customer_display_ctrl_devs(JNIEnv* env, jclass obj, jint nCmd, jint nValue)
//{
//	int nResult = -1;
//	if(g_pCusDisplayInstance == NULL)
//		return -1;
//	hal_sys_info("enter native_customer_display_ctrl_devs()...\n");
//	nResult = g_pCusDisplayInstance->ctrl_devs(g_pCusDisplayInstance->portHandle,nCmd,nValue);
//	hal_sys_info("Leave native_native_customer_display_ctrl_devs() return = %d\n", nResult);
//	return nResult;
//}

static JNINativeMethod g_Methods[] = { { "open", "()I",
		(void*) native_customer_display_open }, { "close", "()I",
		(void*) native_customer_display_close }, { "writePicture", "(IIII[BI)I",
		(void*) native_customer_display_write_picture_data }, { "setBackground",
		"(I)I", (void*) native_customer_display_set_background }, {
		"buzzerBeep", "()I", (void*) native_customer_display_buzzer_beep }, {
		"ledPower", "(I)I", (void*) native_customer_display_led_power }, {
		"displayDefaultScreen", "()I",
		(void*) native_customer_display_display_default_screen }, };

const char* customer_display_get_class_name_allinpay() {
	return g_pJNIREG_CLASS;
}
//	+add by pengli
const char* get_class_name_internal() {
	return g_pJNIREG_CLASS_INTERNAL;
}
//	-add by pengli

JNINativeMethod* customer_display_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
