/*
 * smart_card_jni_interface.cpp
 *
 *  Created on: 2012-7-20
 *      Author: yaomaobiao
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <dlfcn.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#include <jni.h>
#include "hal_sys_log.h"
#include "smart_card_interface.h"
#include "event_queue.h"
#include "smart_card_event.h"

static const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/SmartCardInterface";
//	+add by pengli
static const char* g_pJNIREG_CLASS_INTERNAL = "com/wizarpos/internal/jniinterface/SmartCardInterface";
//	-add by pengli

static jclass mcls;
static jmethodID mmid;
static JavaVM *g_jvm = NULL;

typedef struct smart_card_hal_interface {
	smart_card_query_max_number query_max_number;
	smart_card_query_presence query_presence;
	smart_card_open open;
	smart_card_close close;
	smart_card_set_card_info set_card_info;
	smart_card_power_on power_on;
	smart_card_power_off power_off;
	smart_card_set_slot_info set_slot_info;
	smart_card_transmit transmit;
	smart_card_mc_read mc_read;
	smart_card_mc_write mc_write;
	smart_card_mc_verify_data mc_verify_data;
	void* pSoHandle;
} SMART_CARD_HAL_INSTANCE;

static SMART_CARD_HAL_INSTANCE* g_pSmartCardInstance = NULL;
static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;
static int COUNTS = 0;

jint JNICALL native_smart_card_query_max_number(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_smart_card_query_max_number");
	int nResult = ERR_NORMAL;
	int isCreate = 0;
	if (g_pSmartCardInstance == NULL) {
		g_pSmartCardInstance = new SMART_CARD_HAL_INSTANCE();
		isCreate = 1;
	}

	void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
	if (!pHandle) {
		hal_sys_error("%s\n", dlerror());
		if (isCreate == 1) {
			delete g_pSmartCardInstance;
			g_pSmartCardInstance = NULL;
		}
		return nResult;
	}

	g_pSmartCardInstance->query_max_number = (smart_card_query_max_number) dlsym(pHandle, "smart_card_query_max_number");
	if (g_pSmartCardInstance->query_max_number == NULL) {
		hal_sys_error("can't find smart_card_query_max_number");
		if (isCreate == 1) {
			dlclose(pHandle);
			delete g_pSmartCardInstance;
			g_pSmartCardInstance = NULL;
		}
		return ERR_NO_IMPLEMENT;
	}

	nResult = g_pSmartCardInstance->query_max_number();
	hal_sys_info("- native_smart_card_query_max_number, result = %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_query_presence(JNIEnv* env, jclass obj, jint nSlotIndex) {
	hal_sys_info("+ native_smart_card_query_presence");
	int nResult = ERR_NORMAL;
	//创建新对象
	int isCreate = 0;
	if (g_pSmartCardInstance == NULL) {
		g_pSmartCardInstance = new SMART_CARD_HAL_INSTANCE();
		isCreate = 1;
	}

	void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
	if (!pHandle) {
		hal_sys_error("%s\n", dlerror());
		if (isCreate == 1) {
			delete g_pSmartCardInstance;
			g_pSmartCardInstance = NULL;
		}
		return nResult;
	}

	g_pSmartCardInstance->query_presence = (smart_card_query_presence) dlsym(pHandle, "smart_card_query_presence");
	if (g_pSmartCardInstance->query_presence == NULL) {
		hal_sys_error("can't find smart_card_query_presence");
		if (isCreate == 1) {
			dlclose(pHandle);
			delete g_pSmartCardInstance;
			g_pSmartCardInstance = NULL;
		}
		return ERR_NO_IMPLEMENT;
	}

	nResult = g_pSmartCardInstance->query_presence(nSlotIndex);
	hal_sys_info("-native_smart_card_query_presence, result = %d", nResult);
	return nResult;
}

static void smart_card_event_notify(void* pUserData, int nSlotIndex,
		int nEvent) {
	hal_sys_error("+keyevent_notifier(%d, %d)", nSlotIndex, nEvent);
	JNIEnv *env = NULL;
	bool needDetach = false;
	if (g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
		g_jvm->AttachCurrentThread(&env, NULL);
		needDetach = true;
	} else {
		hal_sys_error("Callback is running in java thread!!!");
	}

	jbyteArray elemt = env->NewByteArray(2);
	jbyte* pelemt = env->GetByteArrayElements(elemt, 0);
	pelemt[0] = (jbyte) nEvent;
	pelemt[1] = (jbyte) nSlotIndex;

	env->CallStaticByteMethod(mcls, mmid, elemt);

	env->ReleaseByteArrayElements(elemt, pelemt, 0);
	env->DeleteLocalRef(elemt);

	if (needDetach) {
		g_jvm->DetachCurrentThread();
	}
	hal_sys_error("-keyevent_notifier()");
}

int convert_error_code(int result) {
	int error_code = -result;
	error_code = error_code & 0xFF;
	return -error_code;
}

jint JNICALL native_smart_card_open(JNIEnv* env, jclass obj, jint nSlotIndex) {
	hal_sys_info("+ native_smart_card_open, nSlotIndex = %d\n", nSlotIndex);
	int nResult = ERR_HAS_OPENED;
	int nError = -1;
	char *pError = NULL;
	env->GetJavaVM(&g_jvm);

	if (g_pSmartCardInstance == NULL) {
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s\n", dlerror());
			return ERR_NORMAL; //-257错误是没有生成对应的handle
		}

		g_pSmartCardInstance = new SMART_CARD_HAL_INSTANCE();
		g_pSmartCardInstance->pSoHandle = pHandle;

		char * methodName;
		if(NULL == (g_pSmartCardInstance->open = (smart_card_open) dlsym(pHandle, methodName = "smart_card_open"))
				|| NULL == (g_pSmartCardInstance->close = (smart_card_close) dlsym(pHandle, methodName = "smart_card_close"))
				|| NULL == (g_pSmartCardInstance->power_on = (smart_card_power_on) dlsym(pHandle, methodName = "smart_card_power_on"))
				|| NULL == (g_pSmartCardInstance->power_off = (smart_card_power_off) dlsym(pHandle, methodName = "smart_card_power_off"))
				|| NULL == (g_pSmartCardInstance->set_slot_info = (smart_card_set_slot_info) dlsym(pHandle, methodName = "smart_card_set_slot_info"))
				|| NULL == (g_pSmartCardInstance->transmit = (smart_card_transmit) dlsym(pHandle, methodName = "smart_card_transmit"))
				|| NULL == (g_pSmartCardInstance->mc_read = (smart_card_mc_read) dlsym(pHandle, methodName = "smart_card_mc_read"))
				|| NULL == (g_pSmartCardInstance->mc_write = (smart_card_mc_write) dlsym(pHandle, methodName = "smart_card_mc_write"))
				|| NULL == (g_pSmartCardInstance->mc_verify_data = (smart_card_mc_verify_data) dlsym(pHandle, methodName = "smart_card_mc_verify_data")))
		{
			hal_sys_error("can't find %s", methodName);
			nResult = ERR_NO_IMPLEMENT;
			goto smart_card_open_clean;
		}

		if(NULL == (g_pSmartCardInstance->set_card_info = (smart_card_set_card_info) dlsym(pHandle, methodName = "smart_card_set_card_info")))
		{
			hal_sys_error("can't find %s", methodName);
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
		mmid = env->GetStaticMethodID(mcls, "callBack", "([B)V");
		if (mmid == NULL) {
			return ERR_NORMAL;
		}
	}
	nResult = g_pSmartCardInstance->open(nSlotIndex, smart_card_event_notify, g_pSmartCardInstance);

	if (nResult < 0) {
		goto smart_card_open_clean;
	} else {
		COUNTS++;
	}
	hal_sys_info("- native_smart_card_open, result = %d", nResult);
	return nResult;

	smart_card_open_clean:
		hal_sys_info("smart_card_open_clean");
		env->DeleteGlobalRef(mcls);
		dlclose(g_pSmartCardInstance->pSoHandle);
		delete g_pSmartCardInstance;
		g_pSmartCardInstance = NULL;
	hal_sys_info("- native_smart_card_open, result = %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_close(JNIEnv* env, jclass obj, jint nHandle) {
	hal_sys_info("+ native_smart_card_close");
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->close == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pSmartCardInstance->close(nHandle);
	COUNTS--;
	if (COUNTS <= 0) {
		hal_sys_info("native_smart_card_close, delete");
		env->DeleteGlobalRef(mcls);
		dlclose(g_pSmartCardInstance->pSoHandle);
		delete g_pSmartCardInstance;
		g_pSmartCardInstance = NULL;
	}
	hal_sys_info("- native_smart_card_close, result= %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_set_card_info(JNIEnv* env, jclass obj,
		jint nHandle, jint nBuadrate, jint nVoltage) {
	hal_sys_info("+ native_smart_card_set_card_info");
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->set_card_info == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pSmartCardInstance->set_card_info(nHandle, nBuadrate, nVoltage);
	hal_sys_info("- native_smart_card_set_card_info, result= %d", nResult);
	return nResult;
}

/*
 *
 * return value : >= 0 : ATR length
 * 				  < 0 : error code
 */
static const char* strSlotField_Short[] = { "FIDI", "EGT", "WI", "WTX", "EDC",
		"protocol", "power", "conv", "IFSC", };
static const char* strSlotField_Long[] = { "cwt", "bwt", "nSlotInfoItem", };

jint JNICALL native_smart_card_power_on(JNIEnv* env, jclass obj, jint nHandle,
		jbyteArray byteArrayATR, jobject objSlotInfo) {
	hal_sys_info("+ native_smart_card_power_on");
	jint i = 0;
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->power_on == NULL)
		return ERR_NO_IMPLEMENT;

	SMART_CARD_SLOT_INFO slot_info;

	memset(&slot_info, 0, sizeof(SMART_CARD_SLOT_INFO));

	jbyte* pElements = env->GetByteArrayElements(byteArrayATR, 0);
	unsigned int nLength = (unsigned int) env->GetArrayLength(byteArrayATR);
	nResult = g_pSmartCardInstance->power_on(nHandle, (unsigned char*) pElements, &nLength, &slot_info);
	if (nResult >= 0) {
		jclass clazz = env->GetObjectClass(objSlotInfo);
		if (0 == clazz) {
			env->ReleaseByteArrayElements(byteArrayATR, pElements, 0);
			return -1;
		}

		unsigned char* pTemp = (unsigned char*) &slot_info;
		for (i = 0;
				i < sizeof(strSlotField_Short) / sizeof(strSlotField_Short[0]);
				i++) {
			jfieldID fid = env->GetFieldID(clazz, strSlotField_Short[i], "S");
			env->SetShortField(objSlotInfo, fid, (jshort) (*pTemp++));
		}
		unsigned int* pTempInt = (unsigned int*) &slot_info;
		pTempInt += 3;
		for (i = 0;
				i < sizeof(strSlotField_Long) / sizeof(strSlotField_Long[0]);
				i++) {
			jfieldID fid = env->GetFieldID(clazz, strSlotField_Long[i], "J");
			env->SetLongField(objSlotInfo, fid, (unsigned int) (*pTempInt++));
		}
	}
	env->ReleaseByteArrayElements(byteArrayATR, pElements, 0);
	nResult = nResult >= 0 ? nLength : nResult;
	hal_sys_info("- native_smart_card_power_on, result= %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_power_off(JNIEnv* env, jclass obj,
		jint nHandle) {
	hal_sys_info("+ native_smart_card_power_off");
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->power_off == NULL)
		return ERR_NO_IMPLEMENT;
	nResult = g_pSmartCardInstance->power_off(nHandle);
	hal_sys_info("- native_smart_card_power_off, result= %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_set_slot_info(JNIEnv* env, jclass obj,
		jint nHandle, jobject objSlotInfo) {
	hal_sys_info("+ native_smart_card_set_slot_info");
	jint i = 0;
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->set_slot_info == NULL)
		return ERR_NO_IMPLEMENT;
	SMART_CARD_SLOT_INFO slot_info;

	jclass clazz = env->GetObjectClass(objSlotInfo);
	if (0 == clazz)
		return nResult;

	unsigned char* pTemp = (unsigned char*) &slot_info;
	for (i = 0; i < sizeof(strSlotField_Short) / sizeof(strSlotField_Short[0]);
			i++) {
		jfieldID fid = env->GetFieldID(clazz, strSlotField_Short[i], "S");
		*pTemp++ = (unsigned char) env->GetShortField(objSlotInfo, fid);
		//env->SetShortField(objSlotInfo, fid, (jshort)(*pTemp++));
	}
	unsigned int* pTempInt = (unsigned int*) &slot_info;
	pTempInt += 3;
	for (i = 0; i < sizeof(strSlotField_Long) / sizeof(strSlotField_Long[0]);
			i++) {
		jfieldID fid = env->GetFieldID(clazz, strSlotField_Long[i], "J");
		*pTempInt++ = (unsigned int) env->GetLongField(objSlotInfo, fid);
		//env->SetLongField(objSlotInfo, fid, (unsigned int)(*pTempInt++));
	}
	nResult = g_pSmartCardInstance->set_slot_info(nHandle, &slot_info);
	hal_sys_info("- native_smart_card_set_slot_info, result= %d", nResult);
	return nResult;
}

/*
 * return value : >= 0 : response data length
 * 				  < 0 : error code
 */
jint JNICALL native_smart_card_transmit(JNIEnv* env, jclass obj, jint nHandle,
		jbyteArray byteArrayAPDU, jbyteArray byteArrayResponse) {
	hal_sys_info("+ native_smart_card_transmit");
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->transmit == NULL)
		return ERR_NO_IMPLEMENT;

	jbyte* strAPDUCmd = env->GetByteArrayElements(byteArrayAPDU, 0);
	unsigned int nAPDUCmdLength = (unsigned int) env->GetArrayLength(byteArrayAPDU);
	jbyte* strResponse = env->GetByteArrayElements(byteArrayResponse, 0);
	unsigned int nResponseBufferLength = (unsigned int) env->GetArrayLength(byteArrayResponse);
	nResult = g_pSmartCardInstance->transmit(nHandle, (unsigned char*) strAPDUCmd, nAPDUCmdLength,
			(unsigned char*) strResponse, &nResponseBufferLength);
	env->ReleaseByteArrayElements(byteArrayAPDU, strAPDUCmd, 0);
	env->ReleaseByteArrayElements(byteArrayResponse, strResponse, 0);
	nResult = nResult < 0 ? nResult : nResponseBufferLength;
	hal_sys_info("- native_smart_card_transmit, result= %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_mc_read(JNIEnv* env, jclass obj, jint nHandle,
		jint nAreaType, jbyteArray byteArryData, jint nStartAddress) {
	hal_sys_info("+ native_smart_card_mc_read()");
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->mc_read == NULL)
		return ERR_NO_IMPLEMENT;
	jbyte* pDataBuffer = env->GetByteArrayElements(byteArryData, 0);
	unsigned int nDataLength = (unsigned int) env->GetArrayLength(byteArryData);
	nResult = g_pSmartCardInstance->mc_read(nHandle, nAreaType,
			(unsigned char*) pDataBuffer, nDataLength, (unsigned char) nStartAddress);
	env->ReleaseByteArrayElements(byteArryData, pDataBuffer, 0);
	hal_sys_info("- native_smart_card_mc_read(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_mc_write(JNIEnv* env, jclass obj, jint nHandle,
		jint nAreaType, jbyteArray byteArryData, jint nStartAddress) {
	hal_sys_info("+ native_smart_card_mc_write()");
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->mc_write == NULL)
		return ERR_NO_IMPLEMENT;
	jbyte* pDataBuffer = env->GetByteArrayElements(byteArryData, 0);
	unsigned int nDataLength = (unsigned int) env->GetArrayLength(byteArryData);
	nResult = g_pSmartCardInstance->mc_write(nHandle, nAreaType,
			(unsigned char*) pDataBuffer, nDataLength,
			(unsigned char) nStartAddress);
	env->ReleaseByteArrayElements(byteArryData, pDataBuffer, 0);
	hal_sys_info("- native_smart_card_mc_write() ,result = %d", nResult);
	return nResult;
}

jint JNICALL native_smart_card_mc_verify_data(JNIEnv* env, jclass obj,
		jint nHandle, jbyteArray byteArryData) {
	hal_sys_info("+ native_smart_card_mc_verify_data()");
	int nResult = ERR_NORMAL;
	if (g_pSmartCardInstance == NULL)
		return ERR_NOT_OPENED;
	if(g_pSmartCardInstance->mc_verify_data == NULL)
		return ERR_NO_IMPLEMENT;
	jbyte* pDataBuffer = env->GetByteArrayElements(byteArryData, 0);
	unsigned int nDataLength = (unsigned int) env->GetArrayLength(byteArryData);
	nResult = g_pSmartCardInstance->mc_verify_data(nHandle,
			(unsigned char*) pDataBuffer, nDataLength);
	env->ReleaseByteArrayElements(byteArryData, pDataBuffer, 0);
	hal_sys_info("- native_smart_card_mc_verify_data(),result = %d", nResult);
	return nResult;

	return 0;
}

static JNINativeMethod g_Methods[] = {
		{ "queryMaxNumber", 		"()I",						(void*) native_smart_card_query_max_number },
		{ "queryPresence", 			"(I)I",						(void*) native_smart_card_query_presence },
		{ "open", 					"(I)I",						(void*) native_smart_card_open },
		{ "close", 					"(I)I",						(void*) native_smart_card_close },
		{ "setCardInfo", 			"(III)I",					(void*) native_smart_card_set_card_info },
		{ "powerOn",				"(I[BLcom/cloudpos/jniinterface/SmartCardSlotInfo;)I",(void*) native_smart_card_power_on },
		{ "powerOff", 				"(I)I",						(void*) native_smart_card_power_off },
		{ "setSlotInfo",			"(ILcom/cloudpos/jniinterface/SmartCardSlotInfo;)I",(void*) native_smart_card_set_slot_info },
		{ "transmit", 				"(I[B[B)I",					(void*) native_smart_card_transmit },
		{ "read", 					"(II[BI)I",					(void*) native_smart_card_mc_read },
		{ "write", 					"(II[BI)I",					(void*) native_smart_card_mc_write },
		{ "verify", 				"(I[B)I",					(void*) native_smart_card_mc_verify_data } };
static JNINativeMethod g_MethodsByInternal[] = {
		{ "queryMaxNumber", 		"()I",						(void*) native_smart_card_query_max_number },
		{ "queryPresence", 			"(I)I",						(void*) native_smart_card_query_presence },
		{ "open", 					"(I)I",						(void*) native_smart_card_open },
		{ "close", 					"(I)I",						(void*) native_smart_card_close },
		{ "powerOn",				"(I[BLcom/wizarpos/internal/jniinterface/SmartCardSlotInfo;)I",(void*) native_smart_card_power_on },
		{ "powerOff", 				"(I)I",						(void*) native_smart_card_power_off },
		{ "setSlotInfo",			"(ILcom/wizarpos/internal/jniinterface/SmartCardSlotInfo;)I",(void*) native_smart_card_set_slot_info },
		{ "transmit", 				"(I[B[B)I",					(void*) native_smart_card_transmit },
		{ "read", 					"(II[BI)I",					(void*) native_smart_card_mc_read },
		{ "write", 					"(II[BI)I",					(void*) native_smart_card_mc_write },
		{ "verify", 				"(I[B)I",					(void*) native_smart_card_mc_verify_data },
		{ "setCardInfo", 			"(III)I",					(void*) native_smart_card_set_card_info }};
const char* smartcard_get_class_name() {
	return g_pJNIREG_CLASS;
}
//	+add by pengli
const char* get_class_name_internal() {
	return g_pJNIREG_CLASS_INTERNAL;
}
//	-add by pengli
JNINativeMethod* smartcard_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
//	+add by pengli
JNINativeMethod* get_methods_internal(int* pCount) {
	*pCount = sizeof(g_MethodsByInternal) / sizeof(g_MethodsByInternal[0]);
	return g_MethodsByInternal;
}
//	-add by pengli
