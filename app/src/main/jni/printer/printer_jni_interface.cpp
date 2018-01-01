/*
 * printer_jni_interface.cpp
 *
 *  Created on: 2012-8-3
 *      Author: yaomaobiao
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#include <jni.h>

#include "hal_sys_log.h"
#include "printer_jni_interface.h"
#include "printer_interface.h"

static const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/PrinterInterface";
//	+add by pengli
static const char* g_pJNIREG_CLASS_INTERNAL = "com/wizarpos/internal/jniinterface/PrinterInterface";
//	-add by pengli

typedef struct printer_hal_interface {
	printer_open open;
	printer_close close;
	printer_begin begin;
	printer_end end;
	printer_write write;
	printer_query_status query;
	printer_query_voltage query_voltage;
	printer_sync sync;
	void* pSoHandle;
} PRINTER_HAL_INSTANCE;

static PRINTER_HAL_INSTANCE* g_pPrinterInstance = NULL;
static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;

int native_printer_query_voltage(JNIEnv* env, jclass obj,
		jintArray capacity, jintArray voltage) {
	hal_sys_info("+ native_printer_query_voltage");
	int nResult = ERR_NORMAL;
	int isCreate = 0;
	if (g_pPrinterInstance == NULL) {
		isCreate = 1;
		g_pPrinterInstance = new PRINTER_HAL_INSTANCE();
	}

	void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
	if (!pHandle) {
		hal_sys_error("%s\n", dlerror());
		if (isCreate == 1) {
			delete g_pPrinterInstance;
			g_pPrinterInstance = NULL;
		}
		return nResult;
	}

	g_pPrinterInstance->query_voltage = (printer_query_voltage) dlsym(pHandle, "printer_query_voltage");
	if (g_pPrinterInstance->query_voltage == NULL) {
		hal_sys_error("can't find printer_query_voltage");
		if (isCreate == 1) {
			dlclose(pHandle);
			delete g_pPrinterInstance;
			g_pPrinterInstance = NULL;
		}
		return ERR_NO_IMPLEMENT;
	}

	if(capacity == NULL || voltage == NULL)
		return ERR_INVALID_ARGUMENT;

	jint* nCapacity = env->GetIntArrayElements(capacity, NULL);
	jint* nVoltage = env->GetIntArrayElements(voltage, NULL);
	nResult = g_pPrinterInstance->query_voltage(nCapacity, nVoltage);
	env->ReleaseIntArrayElements(capacity, nCapacity, 0);
	env->ReleaseIntArrayElements(voltage, nVoltage, 0);

	if (isCreate == 1) {
		dlclose(pHandle);
		delete g_pPrinterInstance;
		g_pPrinterInstance = NULL;
	}
	hal_sys_info("-native_printer_query_voltage, result = %d", nResult);
	return nResult;
}

int native_printer_open(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_printer_open()");
	int nResult = ERR_HAS_OPENED;

	if (g_pPrinterInstance == NULL) {
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s", dlerror());
			return ERR_NORMAL;
		}
		g_pPrinterInstance = new PRINTER_HAL_INSTANCE();
		g_pPrinterInstance->pSoHandle = pHandle;

		char * methodName;
		if(NULL == (g_pPrinterInstance->open = (printer_open) dlsym(pHandle, methodName = "printer_open"))
				|| NULL == (g_pPrinterInstance->close = (printer_close) dlsym(pHandle, methodName = "printer_close"))
				|| NULL == (g_pPrinterInstance->begin = (printer_begin) dlsym(pHandle, methodName = "printer_begin"))
				|| NULL == (g_pPrinterInstance->end = (printer_end) dlsym(pHandle, methodName = "printer_end"))
				|| NULL == (g_pPrinterInstance->write = (printer_write) dlsym(pHandle, methodName = "printer_write")))
		{
			hal_sys_error("can't find %s", methodName);
			nResult = ERR_NO_IMPLEMENT;
			goto printer_init_clean;
		}

		if(NULL == (g_pPrinterInstance->query = (printer_query_status) dlsym(pHandle, methodName = "printer_query_status"))
				||NULL == (g_pPrinterInstance->sync = ( printer_sync) dlsym(pHandle, methodName = "printer_sync")))
		{
			hal_sys_error("can't find %s", methodName);
		}

		nResult = g_pPrinterInstance->open();
		if (nResult < 0) {
			goto printer_init_clean;
		}
	}
	hal_sys_info("- native_printer_open(),result = %d", nResult);
	return nResult;
	printer_init_clean:
	if (g_pPrinterInstance != NULL) {
		hal_sys_info("printer_init_clean");
		dlclose(g_pPrinterInstance->pSoHandle);
		delete g_pPrinterInstance;
		g_pPrinterInstance = NULL;
	}
	hal_sys_info("- native_printer_open(),result = %d", nResult);
	return nResult; //需要返回数据。
}

/*
 * query the status of printer
 * return value : < 0 : error code
 *                = 0 : no paper
 *                = 1 : has paper
 *                other value : RFU
 */
int native_printer_query_status(JNIEnv * env, jclass obj) {
	hal_sys_info("+ native_printer_query_status()");
	int nResult = ERR_NORMAL;
	if (g_pPrinterInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pPrinterInstance->query == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pPrinterInstance->query();
	hal_sys_info("- native_printer_query_status(), result = %d", nResult);
	return nResult;

}

int native_printer_close(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_printer_close()");
	int nResult = ERR_NORMAL;
	if (g_pPrinterInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pPrinterInstance->close == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pPrinterInstance->close();

	dlclose(g_pPrinterInstance->pSoHandle);
	delete g_pPrinterInstance;
	g_pPrinterInstance = NULL;
	hal_sys_info("- native_printer_close(), result = %d", nResult);
	return nResult;
}

int native_printer_begin(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_printer_begin()");
	int nResult = ERR_NORMAL;
	if (g_pPrinterInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pPrinterInstance->begin == NULL)
		return ERR_NO_IMPLEMENT;
	nResult = g_pPrinterInstance->begin();
	hal_sys_info("- native_printer_begin(), result = %d", nResult);
	return nResult;
}

int native_printer_end(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_printer_end()");
	int nResult = ERR_NORMAL;
	if (g_pPrinterInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pPrinterInstance->end == NULL)
		return ERR_NO_IMPLEMENT;
	nResult = g_pPrinterInstance->end();
	hal_sys_info("- native_printer_end(), result = %d", nResult);
	return  nResult;
}

int native_printer_write(JNIEnv* env, jclass obj, jbyteArray arryData,
		jint nDataLength) {
	hal_sys_info("+ native_printer_write()");
	int nResult = ERR_NORMAL;
	if (g_pPrinterInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pPrinterInstance->write == NULL)
		return ERR_NO_IMPLEMENT;

	jbyte* pData = env->GetByteArrayElements(arryData, NULL);
	nResult = g_pPrinterInstance->write((unsigned char*) pData, nDataLength);
	env->ReleaseByteArrayElements(arryData, pData, 0);
	hal_sys_info("- native_printer_write(),result = %d", nResult);
	return nResult;
}

int native_printer_write_offset(JNIEnv* env, jclass obj, jbyteArray arryData,
		jint nOffset, jint nDataLength) {
	hal_sys_info("+ native_printer_write_offset()");
	int nResult = ERR_NORMAL;
	if (g_pPrinterInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pPrinterInstance->write == NULL)
		return ERR_NO_IMPLEMENT;

	if( nOffset < 0 ){
		hal_sys_info("invalid offset %d", nOffset);
		return ERR_INVALID_ARGUMENT;
	}
	jbyte* pData = env->GetByteArrayElements(arryData, NULL);
	nResult = g_pPrinterInstance->write((unsigned char*) (pData + nOffset), nDataLength);
	env->ReleaseByteArrayElements(arryData, pData, 0);
	hal_sys_info("- native_printer_write_offset(),result = %d", nResult);
	return nResult;
}

int native_printer_sync(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_printer_syn()");
	int nResult = ERR_NORMAL;
	if (g_pPrinterInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pPrinterInstance->sync == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pPrinterInstance->sync();
	hal_sys_info("- native_printer_syn(),result = %d", nResult);
	return nResult;
}

static JNINativeMethod g_Methods[] = {
		{ "open", 				"()I",				(void*) native_printer_open },
		{ "close", 				"()I",				(void*) native_printer_close },
		{ "begin", 				"()I",				(void*) native_printer_begin },
		{ "end", 				"()I",				(void*) native_printer_end },
		{ "write", 				"([BI)I",			(void*) native_printer_write },
		{ "queryStatus", 		"()I",				(void*) native_printer_query_status },
		{ "queryVoltage", 		"([I[I)I",			(void*) native_printer_query_voltage },
		{ "write", 				"([BII)I", 			(void*) native_printer_write_offset },
		{ "sync", 				"()I", 				(void*) native_printer_sync }};

const char* printer_get_class_name() {
	return g_pJNIREG_CLASS;
}
//	+add by pengli
const char* get_class_name_internal() {
	return g_pJNIREG_CLASS_INTERNAL;
}
//	-add by pengli
JNINativeMethod* printer_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
