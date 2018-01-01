/*
 * contactless_card_jni_register.cpp
 *
 *  Created on: 2012-7-24
 *      Author: yaomaobiao
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <jni.h>

#include "hal_sys_log.h"
#include "contactless_card_jni_interface.h"


#define JNIREG_CLASS "com/cloudpos/jniinterface/RFCardInterface"

//	+add by pengli
const char* g_pJNIREG_CLASS_INTERNAL = "com/wizarpos/internal/jniinterface/RFCardInterface";
//	-add by pengli
/*
 * Maybe, this table should be defined in the file contactless_card_jni_interface.cpp
 * and then, try to get the pointer by a public method!
 */
static JNINativeMethod g_Methods[] =
{
	{"open",					"()I",					(void*)native_contactless_card_open},
	{"close",					"()I",					(void*)native_contactless_card_close},
	{"searchBegin",				"(III)I",				(void*)native_contactless_card_search_target_begin},
	{"searchEnd",				"()I",					(void*)native_contactless_card_search_target_end},
	{"attach",					"([B)I",				(void*)native_contactless_card_attach_target},
	{"detach",					"()I",					(void*)native_contactless_card_detach_target},
	{"transmit",				"([BI[B)I",				(void*)native_contactless_card_transmit},
	{"sendControlCommand",		"(I[BI)I",				(void*)native_contactless_card_send_control_command},
	{"verify",					"(II[BI)I",				(void*)native_contactless_card_mc_verify_pin},
	{"read",					"(II[BI)I",				(void*)native_contactless_card_mc_read},
	{"write",					"(II[BI)I",				(void*)native_contactless_card_mc_write},
	{"queryInfo",				"([I[I)I",				(void*)native_contactless_card_query_info},
	{"readValue",				"(II[BI[B)I",			(void*)native_contactless_card_mc_read_value},
	{"writeValue",				"(IIIIB)I",				(void*)native_contactless_card_mc_write_value},
	{"increment",				"(III)I",				(void*)native_contactless_card_mc_increment},
	{"decrement",				"(III)I",				(void*)native_contactless_card_mc_decrement},
	{"transfer",				"(II)I",				(void*)native_contactless_card_mc_transfer},
	{"restore",					"(II)I",				(void*)native_contactless_card_mc_restore},
};

/*
 * Register several native methods for one class
 */
static int register_native_methods(JNIEnv* env, const char* strClassName, JNINativeMethod* pMethods, int nMethodNumber)
{
	jclass clazz;
	clazz = env->FindClass(strClassName);
	if(clazz == NULL)
		return JNI_FALSE;
	if(env->RegisterNatives(clazz, pMethods, nMethodNumber) < 0)
		return JNI_FALSE;
	return JNI_TRUE;
}

/*
 * Register native methods for all class
 *
 */
static int register_native_for_all_class(JNIEnv* env)
{
	jint nResult = register_native_methods(env, JNIREG_CLASS, g_Methods, sizeof(g_Methods) / sizeof(g_Methods[0]));
	//	+update by pengli
	if(nResult == JNI_FALSE){
			env->ExceptionClear();
			nResult = register_native_methods(env, g_pJNIREG_CLASS_INTERNAL, g_Methods, sizeof(g_Methods) / sizeof(g_Methods[0]));
	}
	//	-update by pengli
	return nResult;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint nResult = -1;

	if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		hal_sys_info("JNI_OnLoad(), failed in GetEnv()");
		return -1;
	}
	assert(env != NULL);

	if(!register_native_for_all_class(env))
		return -1;

	return JNI_VERSION_1_4;
}
