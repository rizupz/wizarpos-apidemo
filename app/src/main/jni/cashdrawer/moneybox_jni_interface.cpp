#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#include <jni.h>

#include "hal_sys_log.h"
#include "moneybox_interface.h"
#include "moneybox_jni_interface.h"

//com.cloudpos.apidemo.jniinterface
const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/CashDrawerInterface";
//	+add by pengli
const char* g_pJNIREG_CLASS_INTERNAL = "com/wizarpos/internal/jniinterface/CashDrawerInterface";
//	=add by pengli

typedef struct moneybox_hal_interface {
	moneybox_open open;
	moneybox_close close;
	moneybox_ctrl openMoneyBox;
	int portHandle;
	void* pSoHandle;
} MONEY_BOX_HAL_INSTANCE;

static MONEY_BOX_HAL_INSTANCE* g_pInstance = NULL;

int native_moneybox_open(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_moneybox_open");
	int nResult = -1;
	if (g_pInstance == NULL) {
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s\n", dlerror());
			return -1;
		}

		g_pInstance = new MONEY_BOX_HAL_INSTANCE();
		g_pInstance->portHandle = -1;
		g_pInstance->open = (moneybox_open) dlsym(pHandle, "moneybox_open");
		if (g_pInstance->open == NULL) {
			hal_sys_error("can't find moneybox open");
			goto moneybox_init_clean;
		}
		g_pInstance->close = (moneybox_close) dlsym(pHandle, "moneybox_close");
		if (g_pInstance->close == NULL) {
			hal_sys_error("can't find moneybox close");
			goto moneybox_init_clean;
		}
		g_pInstance->openMoneyBox = (moneybox_ctrl) dlsym(pHandle,
				"moneybox_ctrl");
		if (g_pInstance->close == NULL) {
			hal_sys_error("can't find moneybox ctrl");
			goto moneybox_init_clean;
		}

		g_pInstance->pSoHandle = pHandle;
		nResult = g_pInstance->open();
		if (nResult < 0) {
			goto moneybox_init_clean;
		}
		g_pInstance->portHandle = nResult;
	}
	hal_sys_info("- native_moneybox_open,result=%d", nResult);
	return nResult;
	moneybox_init_clean: if (g_pInstance != NULL) {
		delete g_pInstance;
		g_pInstance = NULL;
		hal_sys_info("native_moneybox_open error!\n");
	}
	return nResult;
}

int native_moneybox_close(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_moneybox_close");
	int nResult = -1;
	if (g_pInstance == NULL) {
		hal_sys_info(
				"Leave native_moneybox_close()...g_pIDCardInstance == NULL!\n");
		return -1;
	}
	nResult = g_pInstance->close();
	hal_sys_info("nResult = %d\n", nResult);
	dlclose(g_pInstance->pSoHandle);
	delete g_pInstance;
	g_pInstance = NULL;
	hal_sys_info("- native_moneybox_close,result =%d", nResult);
	return nResult;
}

int native_moneybox_ctrl(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_moneybox_ctrl");
	int nResult = -1;
	if (g_pInstance == NULL) {
		hal_sys_info("Leave native_moneybox_ctrl()...g_pInstance == NULL!\n");
		return -1;
	}
	hal_sys_info(
			"leave native_moneybox_ctrl()... g_pInstance->portHandle = %d\n ",
			g_pInstance->portHandle);
	nResult = g_pInstance->openMoneyBox();
	hal_sys_info("- native_moneybox_ctrl,result = %d", nResult);
	return nResult;
}

static JNINativeMethod g_Methods[] = { { "open", "()I",
		(void*) native_moneybox_open }, { "close", "()I",
		(void*) native_moneybox_close }, { "kickOut", "()I",
		(void*) native_moneybox_ctrl }, };

const char* money_box_get_class_name() {
	return g_pJNIREG_CLASS;
}
const char* get_class_name_internal()
{
	return g_pJNIREG_CLASS_INTERNAL;
}

JNINativeMethod* money_box_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
