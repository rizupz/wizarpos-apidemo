/*
 * pinpad_test.cpp
 *
 *  Created on: 2012-8-8
 *      Author: yaomaobiao
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hal_sys_log.h"
#include "pinpad_utility.h"


void TEST_ansi_98_pin_block(char* strCardNumber)
{
	int nResult = -1;
	int i = 0;
	//const char* strCardNumber = "4380886582869412";
	const char* strPIN = "123456";
	unsigned char strKey[16];
	int nKeyLength = 16;
	unsigned char strResult[16];

	for(i = 0; i < 16; i++)
		strKey[i] = 0x39;

	nResult = ansi_98_pin_block((char*)strCardNumber, (char*)strPIN, strKey, 16, strResult);
	if(nResult >= 0)
	{
		hal_sys_dump("ansi_98_pin_block", strResult, 8);
	}
	return;
}

void TEST_cal_mac(unsigned char* strData, int nDataLength, int nFlag)
{
	int nResult = -1;
	int i = 0;
	unsigned char strKey[16];
	unsigned char strMAC[8];

	for(i = 0; i < 16; i++)
		strKey[i] = 0x39;

	memset(strMAC, 0, 8);

	if(0 == nFlag)
	{
		nResult = calculate_mac_x99(strData, nDataLength, strKey, 16, strMAC, 8);
		if(nResult > 0)
			hal_sys_dump("strMAC", strMAC, 8);
	}
	else
	{
		nResult = calculate_mac_ecb(strData, nDataLength, strKey, 16, strMAC, 8);
		if(nResult > 0)
			hal_sys_dump("strMAC", strMAC, 8);
	}

	return;
}
