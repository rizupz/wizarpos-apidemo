/*
 * contactless_card_jni_interface.cpp
 *
 *  Created on: 2012-7-24
 *      Author: yaomaobiao
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>

#include "hal_sys_log.h"
#include "contactless_card_interface.h"
#include "contactless_card_jni_interface.h"
#include "contactless_card_event.h"
#include "event_queue.h"

static const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/RFCardInterface";
//	+add by pengli
static const char* g_pJNIREG_CLASS_INTERNAL =
		"com/wizarpos/internal/jniinterface/RFCardInterface";
//	-add by pengli

static jclass mcls;
static jmethodID mmid;
static JavaVM *g_jvm = NULL;

static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;

typedef struct contactless_card_hal_interface {
	contactless_card_open open;
	contactless_card_close close;
	contactless_card_search_target_begin search_target_begin;
	contactless_card_search_target_end search_target_end;
	contactless_card_attach_target attach_target;
	contactless_card_detach_target detach_target;
	contactless_card_transmit transmit;
	contactless_card_send_control_command send_control_command;

	contactless_card_mc_verify_pin mc_verify_pin;
	contactless_card_mc_read mc_read;
	contactless_card_mc_write mc_write;
	contactless_card_query_info query_info;
	contactless_card_mc_read_value mc_read_value;
	contactless_card_mc_write_value mc_write_value;
	contactless_card_mc_increment mc_increment;
	contactless_card_mc_decrement mc_decrement;
	contactless_card_mc_transfer mc_transfer;
	contactless_card_mc_restore mc_restore;

	int pCardHandle;
	void* pSoHandle;
	//sem_t sem;
} CONTACTLESS_CARD_HAL_INTERFACE;

static CONTACTLESS_CARD_HAL_INTERFACE* g_pContactlessCard = NULL;

static void contactless_card_callback(void* pUserData, int nEvent,
		unsigned char* pEventData, int nEventDataLength) {
	hal_sys_error("+keyevent_notifier");
	JNIEnv *env = NULL;
	bool needDetach = false;
	if (g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
		g_jvm->AttachCurrentThread(&env, NULL);
		needDetach = true;
	} else {
		hal_sys_error("Callback is running in java thread!!!");
	}

	jbyteArray elemt = env->NewByteArray(nEventDataLength);
	jbyte* data = env->GetByteArrayElements(elemt, 0);
	memcpy(data, pEventData, nEventDataLength);

	env->CallStaticVoidMethod(mcls, mmid, nEvent, elemt);

	env->ReleaseByteArrayElements(elemt, data, 0);
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

jint JNICALL native_contactless_card_open(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_contactless_card_open");
	void* pSoHandle = NULL;
	void* pCardHandle = NULL;
	int nErrorCode = ERR_HAS_OPENED; //非常规错误...如固件版本太低,导致新的API(queryInfo\钱包等操作)加载失败
	env->GetJavaVM(&g_jvm);

	if (g_pContactlessCard == NULL) {
		pSoHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pSoHandle) {
			hal_sys_error("%s\n", dlerror());
			return ERR_NORMAL;
		}

		g_pContactlessCard = new CONTACTLESS_CARD_HAL_INTERFACE();
		g_pContactlessCard->pSoHandle = pSoHandle;

		char * methodName;
		if (NULL== (g_pContactlessCard->open = (contactless_card_open) dlsym(pSoHandle,methodName = "contactless_card_open"))
				|| NULL== (g_pContactlessCard->close = (contactless_card_close) dlsym(pSoHandle,methodName = "contactless_card_close"))
				|| NULL== (g_pContactlessCard->search_target_begin = (contactless_card_search_target_begin) dlsym(pSoHandle,methodName = "contactless_card_search_target_begin"))
				|| NULL== (g_pContactlessCard->search_target_end = (contactless_card_search_target_end) dlsym(pSoHandle,methodName = "contactless_card_search_target_end"))
				|| NULL== (g_pContactlessCard->attach_target = (contactless_card_attach_target) dlsym(pSoHandle,methodName = "contactless_card_attach_target"))
				|| NULL== (g_pContactlessCard->detach_target = (contactless_card_detach_target) dlsym(pSoHandle,methodName = "contactless_card_detach_target"))
				|| NULL== (g_pContactlessCard->send_control_command = (contactless_card_send_control_command) dlsym(pSoHandle,methodName = "contactless_card_send_control_command"))
				|| NULL== (g_pContactlessCard->mc_verify_pin = (contactless_card_mc_verify_pin) dlsym(pSoHandle,methodName = "contactless_card_mc_verify_pin"))
				|| NULL== (g_pContactlessCard->mc_read = (contactless_card_mc_read) dlsym(pSoHandle,methodName = "contactless_card_mc_read"))
				|| NULL== (g_pContactlessCard->mc_write = (contactless_card_mc_write) dlsym(pSoHandle,methodName = "contactless_card_mc_write")))
		{
			hal_sys_error("can't find %s", methodName);
			nErrorCode = ERR_NO_IMPLEMENT;
			goto card_open_clean;
		}
		if (NULL== (g_pContactlessCard->query_info = (contactless_card_query_info) dlsym(pSoHandle,methodName = "contactless_card_query_info"))
				|| NULL== (g_pContactlessCard->mc_read_value = (contactless_card_mc_read_value) dlsym(pSoHandle,methodName = "contactless_card_mc_read_value"))
				|| NULL== (g_pContactlessCard->mc_write_value = (contactless_card_mc_write_value) dlsym(pSoHandle,methodName = "contactless_card_mc_write_value"))
				|| NULL== (g_pContactlessCard->mc_increment = (contactless_card_mc_increment) dlsym(pSoHandle,methodName = "contactless_card_mc_increment"))
				|| NULL== (g_pContactlessCard->mc_decrement = (contactless_card_mc_decrement) dlsym(pSoHandle,methodName = "contactless_card_mc_decrement"))
				|| NULL== (g_pContactlessCard->mc_transfer = (contactless_card_mc_transfer) dlsym(pSoHandle,methodName = "contactless_card_mc_transfer"))
				|| NULL== (g_pContactlessCard->mc_restore = (contactless_card_mc_restore) dlsym(pSoHandle,methodName = "contactless_card_mc_restore")))
		{
			hal_sys_error("can't find %s", methodName);
		}

		hal_sys_info("+ FindClass()");
		jclass tmp = env->FindClass(g_pJNIREG_CLASS);
		//		+ add by pengli
		if (tmp == NULL) {
			env->ExceptionClear();
			tmp = env->FindClass(g_pJNIREG_CLASS_INTERNAL);
		}
		mcls = (jclass) env->NewGlobalRef(tmp);
		env->DeleteLocalRef(tmp);
		hal_sys_info("g_pContactlessCard FindClass()...%p", mcls);
		//		+ add by pengli
		mmid = env->GetStaticMethodID(mcls, "callBack", "(I[B)V");
		hal_sys_info("g_pContactlessCard GetStaticMethodID()...%p", mmid);
		if (mmid == NULL) {
			return ERR_NORMAL;
		}

		pCardHandle = g_pContactlessCard->open(contactless_card_callback, g_pContactlessCard, &nErrorCode);
		hal_sys_info("g_pContactlessCard->open ... result is %d", pCardHandle);
		if (pCardHandle == NULL || nErrorCode < 0) {
			hal_sys_error("g_pContactlessCard->open() result is %d", nErrorCode);
			goto card_open_clean;
		}
		g_pContactlessCard->pCardHandle = (int) pCardHandle;
	}
	hal_sys_info("- native_contactless_card_open, result = %d", nErrorCode);
	return nErrorCode;
	card_open_clean:
	if (g_pContactlessCard) {
		hal_sys_info("card_open_clean");
		env->DeleteGlobalRef(mcls);
		dlclose(g_pContactlessCard->pSoHandle);
		delete g_pContactlessCard;
		g_pContactlessCard = NULL;
	}
	hal_sys_info("- native_contactless_card_open, result = %d", nErrorCode);
	return nErrorCode;
}

jint JNICALL native_contactless_card_close(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_contactless_card_close()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if(g_pContactlessCard->close == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pContactlessCard->close(g_pContactlessCard->pCardHandle);

	env->DeleteGlobalRef(mcls);
	dlclose(g_pContactlessCard->pSoHandle);
	delete g_pContactlessCard;
	g_pContactlessCard = NULL;
	hal_sys_info("- native_contactless_card_close(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_search_target_begin(JNIEnv* env, jclass obj,
		jint nCardMode, jint nFlagSearchAll, jint nTimeout_MS) {
	hal_sys_info("+ native_contactless_card_search_target_begin()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if(g_pContactlessCard->search_target_begin == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pContactlessCard->search_target_begin(g_pContactlessCard->pCardHandle, nCardMode, nFlagSearchAll, nTimeout_MS);
	hal_sys_info("- native_contactless_card_search_target_begin, result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_search_target_end(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_contactless_card_search_target_end()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if(g_pContactlessCard->search_target_end == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pContactlessCard->search_target_end(g_pContactlessCard->pCardHandle);
	hal_sys_info("- native_contactless_card_search_target_end(), result = %d",nResult);
	return nResult;
}

jint JNICALL native_contactless_card_attach_target(JNIEnv* env, jclass obj, jbyteArray arryATR) {
	hal_sys_info("+ native_contactless_card_attach_target()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if(g_pContactlessCard->attach_target == NULL)
		return ERR_NO_IMPLEMENT;

	if(arryATR == NULL) {
		nResult = g_pContactlessCard->attach_target(g_pContactlessCard->pCardHandle, NULL, 0);
	} else {
		jbyte* pData = env->GetByteArrayElements(arryATR, NULL);
		jint nATRBufferLength = env->GetArrayLength(arryATR);
		nResult = g_pContactlessCard->attach_target(g_pContactlessCard->pCardHandle,
				(unsigned char*) pData, (unsigned int)nATRBufferLength);
		env->ReleaseByteArrayElements(arryATR, pData, 0);
	}
	hal_sys_info("- native_contactless_card_attach_target(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_detach_target(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_contactless_card_detach_target()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if(g_pContactlessCard->detach_target == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pContactlessCard->detach_target(g_pContactlessCard->pCardHandle);
	hal_sys_info("- native_contactless_card_detach_target(),result = %d",nResult);
	return nResult;
}

jint JNICALL native_contactless_card_transmit(JNIEnv* env, jclass obj,
		jbyteArray arryAPDU, jint nAPDULength, jbyteArray arryResponse) {
	hal_sys_info("+ native_contactless_card_transmit()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if(g_pContactlessCard->transmit == NULL)
		return ERR_NO_IMPLEMENT;
	if(arryAPDU == NULL || arryResponse == NULL)
		return ERR_INVALID_ARGUMENT;

	jbyte* pData = env->GetByteArrayElements(arryAPDU, NULL);
	jbyte* pResponse = env->GetByteArrayElements(arryResponse, NULL);
	unsigned int nResponseBufferLength = (unsigned int) (env->GetArrayLength(arryResponse));
	nResult = g_pContactlessCard->transmit(g_pContactlessCard->pCardHandle,
			(unsigned char*) pData, nAPDULength, (unsigned char*) pResponse, &nResponseBufferLength);
	env->ReleaseByteArrayElements(arryAPDU, pData, 0);
	env->ReleaseByteArrayElements(arryResponse, pResponse, 0);
	nResult = nResult >= 0 ? nResponseBufferLength : nResult;
	hal_sys_info("- native_contactless_card_transmit(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_send_control_command(JNIEnv* env, jclass obj,
		jint nCmdID, jbyteArray arryCmdData, jint nDataLength) {
	hal_sys_info("+ native_contactless_card_send_control_command()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->send_control_command == NULL)
		return ERR_NO_IMPLEMENT;
	if (arryCmdData ==  NULL)
		return ERR_INVALID_ARGUMENT;

	jbyte* pData = env->GetByteArrayElements(arryCmdData, NULL);
	nResult = g_pContactlessCard->send_control_command(g_pContactlessCard->pCardHandle,
			(unsigned int) nCmdID, (unsigned char*) pData, (unsigned int) nDataLength);
	env->ReleaseByteArrayElements(arryCmdData, pData, 0);
	hal_sys_info("- native_contactless_card_send_control_command(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_verify_pin(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nPinType, jbyteArray strPin, jint nPinLength) {
	hal_sys_info("+ native_contactless_card_mc_verify_pin()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_verify_pin == NULL)
		return ERR_NO_IMPLEMENT;
	if (strPin ==  NULL)
		return ERR_INVALID_ARGUMENT;

	jbyte* pData = env->GetByteArrayElements(strPin, NULL);
	int len = env->GetArrayLength(strPin);
	nResult = g_pContactlessCard->mc_verify_pin(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nPinType,
			(unsigned char*) pData, (unsigned int) nPinLength);
	env->ReleaseByteArrayElements(strPin, pData, 0);
	hal_sys_info("- native_contactless_card_mc_verify_pin(),result = %d",nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_read(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex, jbyteArray pDataBuffer,
		jint nDataBufferLength) {
	hal_sys_info("+ native_contactless_card_mc_read()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_read == NULL)
		return ERR_NO_IMPLEMENT;
	if (pDataBuffer ==  NULL)
		return ERR_INVALID_ARGUMENT;

	jbyte* pData = env->GetByteArrayElements(pDataBuffer, NULL);
	nResult = g_pContactlessCard->mc_read(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex,
			(unsigned char*) pData, (unsigned int) nDataBufferLength);
	env->ReleaseByteArrayElements(pDataBuffer, pData, 0);
	hal_sys_info("- native_contactless_card_mc_read(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_write(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex, jbyteArray pData, jint nDataLength) {
	hal_sys_info("+ native_contactless_card_mc_write()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_write == NULL)
		return ERR_NO_IMPLEMENT;
	if (pData ==  NULL)
		return ERR_INVALID_ARGUMENT;

	jbyte* pWriteData = env->GetByteArrayElements(pData, NULL);
	nResult = g_pContactlessCard->mc_write(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex,
			(unsigned char*) pWriteData, (unsigned int) nDataLength);
	env->ReleaseByteArrayElements(pData, pWriteData, 0);
	hal_sys_info("- native_contactless_card_mc_write(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_query_info(JNIEnv * env, jclass obj,
		jintArray pHasMoreCards, jintArray pCardType) {
	hal_sys_info("+ native_contactless_card_query_info()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->query_info == NULL)
		return ERR_NO_IMPLEMENT;
	if (pCardType ==  NULL || pHasMoreCards == NULL)
		return ERR_INVALID_ARGUMENT;

	jint* iHasMoreCards = env->GetIntArrayElements(pHasMoreCards, NULL);
	jint* iCardType = env->GetIntArrayElements(pCardType, NULL);
	nResult = g_pContactlessCard->query_info(g_pContactlessCard->pCardHandle, iHasMoreCards, iCardType);
	env->ReleaseIntArrayElements(pHasMoreCards, iHasMoreCards, 0);
	env->ReleaseIntArrayElements(pCardType, iCardType, 0);

	hal_sys_info("- native_contactless_card_query_info(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_read_value(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex, jbyteArray pValue,
		jint nValueBufLength, jbyteArray pAddrData) {
	hal_sys_info("+ native_contactless_card_mc_read_value()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_read_value == NULL)
		return ERR_NO_IMPLEMENT;
	if (pValue ==  NULL || pAddrData == NULL)
		return ERR_INVALID_ARGUMENT;

	jbyte* pData = env->GetByteArrayElements(pValue, NULL);
	jbyte* pAddr = env->GetByteArrayElements(pAddrData, NULL);
	nResult = g_pContactlessCard->mc_read_value(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex,
			(unsigned char*) pData, (unsigned int) nValueBufLength, (unsigned char*) pAddr);
	env->ReleaseByteArrayElements(pValue, pData, 0);
	env->ReleaseByteArrayElements(pAddrData, pAddr, 0);
	hal_sys_info("- native_contactless_card_mc_read_value(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_write_value(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex, jint pValue, jint nValueLength,
		jbyte pAddrData) {
	hal_sys_info("+ native_contactless_card_mc_write_value()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_write_value == NULL)
		return ERR_NO_IMPLEMENT;
	if (pValue ==  NULL || pAddrData == NULL)
		return ERR_INVALID_ARGUMENT;
	nResult = g_pContactlessCard->mc_write_value(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex,
			(unsigned char*) &(pValue),(unsigned int) nValueLength, (unsigned char) pAddrData);
	hal_sys_info("- native_contactless_card_mc_write_value(), result =%d ",
			nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_increment(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex, jint value) {
	hal_sys_info("+ native_contactless_card_mc_increment()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_increment == NULL)
		return ERR_NO_IMPLEMENT;

	int nValueLength = 4;
	nResult = g_pContactlessCard->mc_increment(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex,
			(unsigned char*) &(value), (unsigned int) nValueLength);
	hal_sys_info("- native_contactless_card_mc_increment(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_decrement(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex, jint value) {
	hal_sys_info("+ native_contactless_card_mc_decrement()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_decrement == NULL)
		return ERR_NO_IMPLEMENT;
	int nValueLength = 4;
	nResult = g_pContactlessCard->mc_decrement(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex,
			(unsigned char*) &(value), (unsigned int) nValueLength);
	hal_sys_info("- native_contactless_card_mc_decrement(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_transfer(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex) {
	hal_sys_info("+ native_contactless_card_mc_transfer()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_transfer == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pContactlessCard->mc_transfer(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex);
	hal_sys_info("- native_contactless_card_mc_transfer(), result = %d", nResult);
	return nResult;
}

jint JNICALL native_contactless_card_mc_restore(JNIEnv* env, jclass obj,
		jint nSectorIndex, jint nBlockIndex) {
	hal_sys_info("+ native_contactless_card_mc_restore()");
	int nResult = ERR_NORMAL;
	if (g_pContactlessCard == NULL)
		return ERR_NOT_OPENED;
	if (g_pContactlessCard->mc_transfer == NULL)
		return ERR_NO_IMPLEMENT;

	nResult = g_pContactlessCard->mc_restore(g_pContactlessCard->pCardHandle,
			(unsigned int) nSectorIndex, (unsigned int) nBlockIndex);
	hal_sys_info("- native_contactless_card_mc_restore(), result = %d", nResult);
	return nResult;
}
