#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#include <jni.h>

#include "hal_sys_log.h"
#include "serial_port_jni_interface.h"
#include "serial_port_interface.h"

const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/SerialPortInterface";
//	+add by pengli
static const char* g_pJNIREG_CLASS_INTERNAL =
		"com/wizarpos/internal/jniinterface/SerialPortInterface";
//	-add by pengli

typedef struct serail_port_hal_interface {
	esp_open open;
	esp_close close;
	esp_read read;
	esp_write write;
	esp_set_baudrate set_baudrate;
	esp_flush_io flush_io;
	int portHandle;
	void* pSoHandle;
} SERIAL_PORT_HAL_INSTANCE;

SERIAL_PORT_HAL_INSTANCE* g_pSerialPortInstance = NULL;
static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;

char* jstringToChar(JNIEnv* env, jstring jstr) {
	char * rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("UTF-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte * ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1); //new char[alen+1];
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);

	return rtn;
}

int native_serial_port_open(JNIEnv* env, jclass obj, jstring device_name) {
//	const char* SERIAL_DEVICE_NAME = "/dev/s3c2410_serial2";// /dev/s3c2410_serial2
	hal_sys_info("+ native_serial_port_open()");
	int nResult = ERR_HAS_OPENED;
	char* SERIAL_DEVICE_NAME = jstringToChar(env, device_name);
	hal_sys_info("SERIAL_DEVICE_NAME = %s", SERIAL_DEVICE_NAME);
	if (g_pSerialPortInstance == NULL) {
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s\n", dlerror());
			return ERR_NORMAL;
		}

		g_pSerialPortInstance = new SERIAL_PORT_HAL_INSTANCE();
		g_pSerialPortInstance->pSoHandle = pHandle;

		char * methodName;
		if (NULL == (g_pSerialPortInstance->open = (esp_open) dlsym(pHandle, methodName = "esp_open"))
				||NULL == (g_pSerialPortInstance->close = (esp_close) dlsym(pHandle, methodName = "esp_close"))
				||NULL == (g_pSerialPortInstance->read = (esp_read) dlsym(pHandle, methodName = "esp_read"))
				||NULL == (g_pSerialPortInstance->write = (esp_write) dlsym(pHandle, methodName = "esp_write"))
				||NULL == (g_pSerialPortInstance->set_baudrate = (esp_set_baudrate) dlsym(pHandle, methodName = "esp_set_baudrate"))
				||NULL == (g_pSerialPortInstance->flush_io = (esp_flush_io) dlsym(pHandle, methodName = "esp_flush_io")))
		{
			hal_sys_error("can't find %s", methodName);
			nResult = ERR_NO_IMPLEMENT;
			goto serial_port_init_clean;
		}

		hal_sys_info("%s, open", SERIAL_DEVICE_NAME);
		nResult = g_pSerialPortInstance->open((char *) SERIAL_DEVICE_NAME);

		if (nResult < 0) {
			goto serial_port_init_clean;
		} else {
			g_pSerialPortInstance->portHandle = nResult;
		}
	}
	hal_sys_info("-native_serial_port_open(),result= %d", nResult);
	return nResult;
	serial_port_init_clean:
		hal_sys_info("serial_port_init_clean");
		dlclose(g_pSerialPortInstance->pSoHandle);
		delete g_pSerialPortInstance;
		g_pSerialPortInstance = NULL;
		hal_sys_info("-native_serial_port_open(),result= %d", nResult);
	return nResult;
}

int native_serial_port_close(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_serial_port_close()");
	int nResult = ERR_NORMAL;
	if (g_pSerialPortInstance == NULL)
		return ERR_NOT_OPENED;

	nResult = g_pSerialPortInstance->close(g_pSerialPortInstance->portHandle);
	dlclose(g_pSerialPortInstance->pSoHandle);
	delete g_pSerialPortInstance;
	g_pSerialPortInstance = NULL;
	hal_sys_info("- native_serial_port_close (), result = %d", nResult);
	return nResult;
}

int native_serial_port_read(JNIEnv* env, jclass obj, jbyteArray pDataBuffer,
		jint nExpectedDataLength, jint nTimeout_MS) {
	hal_sys_info("+ native_serial_port_read()");
	int nResult = ERR_NORMAL;
	if (g_pSerialPortInstance == NULL)
		return ERR_NOT_OPENED;
	if (pDataBuffer == NULL)
		return ERR_INVALID_ARGUMENT;
	jbyte* pData = env->GetByteArrayElements(pDataBuffer, NULL);
	nResult = g_pSerialPortInstance->read(g_pSerialPortInstance->portHandle,
			(unsigned char *) pData, nExpectedDataLength, nTimeout_MS);
	env->ReleaseByteArrayElements(pDataBuffer, pData, 0);
	hal_sys_info("- native_serial_port_read(), result = %d", nResult);
	return nResult;
}

int native_serial_port_write(JNIEnv* env, jclass obj, jbyteArray pDataBuffer,
		jint offset, jint nDataLength) {
	hal_sys_info("+ native_serial_port_write()");
	int nResult = ERR_NORMAL;
	if (g_pSerialPortInstance == NULL)
		return ERR_NOT_OPENED;
	if (pDataBuffer == NULL)
		return ERR_INVALID_ARGUMENT;
	jbyte* pData = env->GetByteArrayElements(pDataBuffer, NULL);
	nResult = g_pSerialPortInstance->write(g_pSerialPortInstance->portHandle,
			(unsigned char *) (pData + offset), nDataLength);
	env->ReleaseByteArrayElements(pDataBuffer, pData, 0);
	hal_sys_info("- native_serial_port_write(), result = %d", nResult);
	return nResult;
}

int native_serial_port_set_baudrate(JNIEnv* env, jclass obj, jint nBaudrate) {
	hal_sys_info("+ native_serial_port_set_baudrate()");
	int nResult = ERR_NORMAL;
	if (g_pSerialPortInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pSerialPortInstance->set_baudrate(g_pSerialPortInstance->portHandle, nBaudrate);
	hal_sys_info("- native_serial_port_set_baudrate(), result = %d", nResult);
	return nResult;
}

int native_serial_port_flush_io(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_serial_port_flush_io()");
	int nResult = ERR_NORMAL;
	if (g_pSerialPortInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pSerialPortInstance->flush_io(g_pSerialPortInstance->portHandle);
	hal_sys_info("- native_serial_port_flush_io(), result = %d", nResult);
	return nResult;
}

static JNINativeMethod g_Methods[] = {
		{ "open", 				"(Ljava/lang/String;)I",	(void*) native_serial_port_open },
		{ "close", 				"()I",						(void*) native_serial_port_close },
		{ "read", 				"([BII)I",					(void*) native_serial_port_read },
		{ "write", 				"([BII)I",					(void*) native_serial_port_write },
		{ "setBaudrate", 		"(I)I",						(void*) native_serial_port_set_baudrate },
		{ "flushIO", 			"()I",						(void*) native_serial_port_flush_io }, };

const char* serial_port_get_class_name() {
	return g_pJNIREG_CLASS;
}
//	+add by pengli
const char* get_class_name_internal() {
	return g_pJNIREG_CLASS_INTERNAL;
}
//	-add by pengli

JNINativeMethod* serial_port_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
