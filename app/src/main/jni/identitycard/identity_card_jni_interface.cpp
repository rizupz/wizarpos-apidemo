#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#include <jni.h>

#include "hal_sys_log.h"
#include "identity_card_jni_interface.h"
#include "identity_card_interface.h"
//								com/wizarpos/idcardjniinterface/IDCardJniInterface
const char* g_pJNIREG_CLASS = "com/cloudpos/jniinterface/IDCardInterface";
//	+add by pengli
const char* g_pJNIREG_CLASS_INTERNAL = "com/wizarpos/internal/jniinterface/IDCardInterface";
//	-add by pengli
typedef struct identity_card_hal_interface {
	idcard_open open;
	idcard_close close;
	idcard_get_fixed_information get;
	idcard_search_target search_target;
	int portHandle;
	void* pHandle;
} IDENTITY_CARD_HAL_INSTANCE;

static IDENTITY_CARD_HAL_INSTANCE* g_pIDCardInstance = NULL;
static int ERR_NOT_OPENED = -255;
static int ERR_HAS_OPENED = -254;
static int ERR_NO_IMPLEMENT = -253;
static int ERR_INVALID_ARGUMENT = -252;
static int ERR_NORMAL = -251;

int native_identity_card_open(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_identity_card_open()");
	int nResult = ERR_HAS_OPENED;
	if (g_pIDCardInstance == NULL) {
		void* pHandle = dlopen("libUnionpayCloudPos.so", RTLD_LAZY);
		if (!pHandle) {
			hal_sys_error("%s\n", dlerror());
			return -1;
		}

		g_pIDCardInstance = new IDENTITY_CARD_HAL_INSTANCE();
		g_pIDCardInstance->pHandle = pHandle;
		char * methodName;
		if (NULL == (g_pIDCardInstance->open = (idcard_open) dlsym(pHandle, methodName = "idcard_open"))
				|| NULL == (g_pIDCardInstance->close = (idcard_close) dlsym(pHandle, methodName = "idcard_close"))
				|| NULL == (g_pIDCardInstance->get = (idcard_get_fixed_information) dlsym(pHandle, methodName = "idcard_get_fixed_information"))
				|| NULL == (g_pIDCardInstance->search_target = (idcard_search_target) dlsym(pHandle, methodName = "idcard_search_target")))
		{
			hal_sys_error("can't find %s", methodName);
			nResult = ERR_NO_IMPLEMENT;
			goto identity_card_init_clean;
		}

		nResult = g_pIDCardInstance->open();
		hal_sys_info("native_identity_card_open ! result = %d\n", nResult);
		if (nResult < 0) {
			goto identity_card_init_clean;
		} else {
			g_pIDCardInstance->portHandle = nResult;
		}
	}
	hal_sys_info("- native_identity_card_open, result = %d", nResult);
	return nResult;
	identity_card_init_clean:
		hal_sys_info("identity_card_init_clean");
		dlclose(g_pIDCardInstance->pHandle);
		delete g_pIDCardInstance;
		g_pIDCardInstance = NULL;
	hal_sys_info("- native_identity_card_open, result = %d", nResult);
	return nResult;
}

int native_identity_card_close(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_identity_card_close()");
	int nResult = ERR_NORMAL;
	if (g_pIDCardInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pIDCardInstance->close(g_pIDCardInstance->portHandle);
	dlclose(g_pIDCardInstance->pHandle);
	delete g_pIDCardInstance;
	g_pIDCardInstance = NULL;
	hal_sys_info("- native_identity_card_close(), result = %d", nResult);
	return nResult;
}

static const char* strIDCardField_name[] = { "strName", "strSex", "strNation",
		"strBorn", "strAddress", "strIDCardNo", "strGrantDept",
		"strUserLifeBegin", "strUserLifeEnd", "strReserved", "strPicture", };
static const int nIDCardField_size[] = { 30, 2, 4, 16, 70, 36, 30, 16, 16, 36,
		1024, };

int native_identity_card_get_fixed_information(JNIEnv* env, jclass obj,
		jobject objIDCardData) {
	hal_sys_info("+ native_identity_card_get_fixed_information()");
	int nResult = ERR_NORMAL;
	if (g_pIDCardInstance == NULL)
		return ERR_NOT_OPENED;
	unsigned int i;
	IDCARD_PROPERTY IDCardData;

	memset(&IDCardData, 0, sizeof(IDCARD_PROPERTY));

	nResult = g_pIDCardInstance->get(g_pIDCardInstance->portHandle,
			&IDCardData);
	if (nResult < 0) {
		hal_sys_info("- native_identity_card_get_fixed_information(), result = %d", nResult);
		return nResult;
	}
	jclass clazz = env->GetObjectClass(objIDCardData);
	if (0 == clazz) {
		hal_sys_info("clazz == 0\n");
		return ERR_INVALID_ARGUMENT;
	}

	unsigned char* pTemp = (unsigned char*) &IDCardData;
	for (i = 0; i < sizeof(strIDCardField_name) / sizeof(strIDCardField_name[0]); i++) {
		jfieldID fid = env->GetFieldID(clazz, strIDCardField_name[i], "[B");
		jbyte* byte = (jbyte*) pTemp;
		jbyteArray jarray = env->NewByteArray(nIDCardField_size[i]);
		env->SetByteArrayRegion(jarray, 0, nIDCardField_size[i], byte);
		env->SetObjectField(objIDCardData, fid, jarray);
		pTemp += nIDCardField_size[i];
	}
	hal_sys_info("- native_identity_card_get_fixed_information(), result = %d", nResult);
	return nResult;
}

int native_identity_card_search_target(JNIEnv* env, jclass obj) {
	hal_sys_info("+ native_identity_card_search_target()");
	int nResult = ERR_NORMAL;
	if (g_pIDCardInstance == NULL)
		return ERR_NOT_OPENED;
	nResult = g_pIDCardInstance->search_target(g_pIDCardInstance->portHandle);
	hal_sys_info("- native_identity_card_search_target(), result = %d",nResult);
	return nResult;
}

static JNINativeMethod g_Methods[] = {
		{ "open", 				"()I",				(void*) native_identity_card_open },
		{ "close", 				"()I",				(void*) native_identity_card_close },
		{ "getInformation",		"(Lcom/cloudpos/jniinterface/IDCardProperty;)I",	(void*) native_identity_card_get_fixed_information },
		{ "searchTarget",		"()I", 				(void*) native_identity_card_search_target }, };
static JNINativeMethod g_MethodsByInternal[] = {
		{ "open", 				"()I",				(void*) native_identity_card_open },
		{ "close", 				"()I",				(void*) native_identity_card_close },
		{ "getInformation",		"(Lcom/wizarpos/internal/jniinterface/IDCardProperty;)I",	(void*) native_identity_card_get_fixed_information },
		{ "searchTarget",		"()I", 				(void*) native_identity_card_search_target }, };

const char* identity_card_get_class_name() {
	return g_pJNIREG_CLASS;
}
const char* get_class_name_internal()
{
	return g_pJNIREG_CLASS_INTERNAL;
}

JNINativeMethod* identity_card_get_methods(int* pCount) {
	*pCount = sizeof(g_Methods) / sizeof(g_Methods[0]);
	return g_Methods;
}
JNINativeMethod* get_methods_internal(int* pCount) {
	*pCount = sizeof(g_MethodsByInternal) / sizeof(g_MethodsByInternal[0]);
	return g_MethodsByInternal;
}

