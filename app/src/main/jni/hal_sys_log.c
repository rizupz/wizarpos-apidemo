/*
 * Log.cpp
 *
 *  Created on: 2012-6-20
 *      Author: yaomaobiao
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <android/log.h>
#include "hal_sys_log.h"

#define __LOG_TAG__		"HAL_JNI"

void hal_sys_info(const char* pMessage, ...)
{
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_INFO, __LOG_TAG__, pMessage, lVarArgs);
	__android_log_print(ANDROID_LOG_INFO, __LOG_TAG__, "\n");
	va_end(lVarArgs);
}

void hal_sys_error(const char* pMessage, ...)
{
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_ERROR, __LOG_TAG__, pMessage, lVarArgs);
	__android_log_print(ANDROID_LOG_ERROR, __LOG_TAG__, "\n");
	va_end(lVarArgs);

}
void hal_sys_warn(const char* pMessage, ...)
{
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_WARN, __LOG_TAG__, pMessage, lVarArgs);
	__android_log_print(ANDROID_LOG_WARN, __LOG_TAG__, "\n");
	va_end(lVarArgs);

}
void hal_sys_debug(const char* pMessage, ...)
{
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_DEBUG, __LOG_TAG__, pMessage, lVarArgs);
	__android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG__, "\n");
	va_end(lVarArgs);

}

void hal_sys_dump(const char * str, unsigned char * pBuf, unsigned int len)
{
	unsigned int i, j, nRemain;
	char tmp[17];
	char *p;
	char *add = pBuf;
	unsigned int nLine = 0;

	char strLine[32];

	if(str)
		hal_sys_info("%s: length = %d [0x%X]\r\n", str, len, len);



	if(len == 0)
		return;

	nLine = len / 8;

	for(i = 0; i < nLine; i++)
	{
		memset(strLine, 0, 32);
		for(j = 0; j < 8; j++)
			sprintf(strLine + 3 * j, "%02X ", *add++);
		__android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG__, "%s\n", strLine);
	}

	nRemain = len % 8;
	if(nRemain)
	{
		memset(strLine, 0, 32);
		for(j = 0; j < nRemain; j++)
			sprintf(strLine + 3 * j, "%02X ", *add++);
		__android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG__, "%s\n", strLine);

	}

	return;
}







