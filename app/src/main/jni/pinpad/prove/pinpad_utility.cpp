/*
 * pinpad_utility.cpp
 *
 *  Created on: 2012-8-8
 *      Author: yaomaobiao
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hal_sys_log.h"
#include "DES.h"

int asscii_to_bcd(char* strAscii, int nAsciiLength, unsigned char* strBCDBuffer, int nBCDBufferLength)
{
	int i = 0;
	int nLoop = 0;
	int nResultLength = (nAsciiLength % 2 == 1) ? (nAsciiLength / 2 + 1) : nAsciiLength / 2;

	if(nResultLength > nBCDBufferLength)
	{
		hal_sys_info("nResultLength > nBCDBufferLength! nResultLength = %d nBCDBufferLength = %d\n", nResultLength, nBCDBufferLength);
		return -1;
	}

	nLoop = nAsciiLength / 2 ;
	for(i = 0; i < nLoop; i++)
	{
		strBCDBuffer[i] = (strAscii[2 * i] - 0x30) << 4 | (strAscii[2 * i + 1] - 0x30);
	}
	if(nAsciiLength % 2 == 1)
		strBCDBuffer[i] = (strAscii[ 2 * i ] - 0x30) << 4 | 0x0F;

	return nResultLength;
}

int ansi_98_pin_block(char* strCardNumber, char* strPIN, unsigned char* pKey, unsigned int nKeyLength, unsigned char* pResult)
{
	int nCopyStart = 0;
	int nResult = 0;
	int i = 0;

	unsigned char str1[8];
	unsigned char str2[8];

	int nCardLength = strlen(strCardNumber);
	if(nCardLength < 12)
	{
		hal_sys_error("card number is too short!");
		return -1;
	}
	nCopyStart = nCardLength == 12 ? 0 : nCardLength - 12 - 1;
	hal_sys_info("nCopyStart = %d\n", nCopyStart);

	memset(str2, 0, 8);
	nResult = asscii_to_bcd(strCardNumber + nCopyStart, 12, str2 + 2, 6);
	hal_sys_dump("str2", str2, 8);

	memset(str1, 0xFF, 8);
	str1[0] = (unsigned char)strlen(strPIN);
	nResult = asscii_to_bcd(strPIN, strlen(strPIN), str1 + 1, 7);
	hal_sys_dump("str1", str1, 8);

	for(i = 0; i < 8; i++)
		str1[i] ^= str2[i];

	DES(0, str1, pKey, pResult);
	return 0;
}


int calculate_mac_x99(unsigned char* pData, int nDataLength, unsigned char* pKey, int nKeyLength, unsigned char* pMACOutBuffer, int nMACOutBufferLength)
{
	int i = 0;
	int j = 0;
	int nLoop = 0;
	unsigned char strTempDataBufferIN[8];
	unsigned char strTempDataBufferOUT[8];

	if(pData == NULL || pMACOutBuffer == NULL || pKey == NULL)
		return -1;
	if(nMACOutBufferLength < 8)
		return -1;

	memset(strTempDataBufferIN, 0, 8);
	memset(strTempDataBufferOUT, 0, 8);

	nLoop = nDataLength / 8;

	for(i = 0; i < nLoop; i++)
	{
		memcpy(strTempDataBufferIN, pData + i * 8, 8);
		for(j = 0; j < 8; j++)
			strTempDataBufferIN[j] ^= strTempDataBufferOUT[j];
		DES(0, strTempDataBufferIN, pKey, strTempDataBufferOUT);
	}

	if(nDataLength % 8)
	{
		memset(strTempDataBufferIN, 0, 8);
		memcpy(strTempDataBufferIN, pData + i * 8, nDataLength % 8);
		for(j = 0; j < 8; j++)
			strTempDataBufferIN[j] ^= strTempDataBufferOUT[j];
		DES(0, strTempDataBufferIN, pKey, strTempDataBufferOUT);
	}

	memcpy(pMACOutBuffer, strTempDataBufferOUT, 8);
	return 8;
}

int calculate_mac_ecb(unsigned char* pData, int nDataLength, unsigned char* pKey, int nKeyLength, unsigned char* pMACOutBuffer, int nMACOutBufferLength)
{
	int i = 0;
	int j = 0;
	int nLoop = 0;
	unsigned char strTempDataBufferIN[8];
	unsigned char strTempDataBufferOUT[8];

	if(pData == NULL || pMACOutBuffer == NULL || pKey == NULL)
		return -1;
	if(nMACOutBufferLength < 8)
		return -1;

	memset(strTempDataBufferIN, 0, 8);
	memset(strTempDataBufferOUT, 0, 8);

	nLoop = nDataLength / 8;

	for(i = 0; i < nLoop; i++)
	{
		memcpy(strTempDataBufferIN, pData + i * 8, 8);
		for(j = 0; j < 8; j++)
			strTempDataBufferOUT[j] ^= strTempDataBufferIN[j];
	}

	if(nDataLength % 8)
	{
		memset(strTempDataBufferIN, 0, 8);
		memcpy(strTempDataBufferIN, pData + i * 8, nDataLength % 8);
		for(j = 0; j < 8; j++)
			strTempDataBufferOUT[j] ^= strTempDataBufferIN[j];
	}

	DES(0, strTempDataBufferOUT, pKey, pMACOutBuffer);

	return 8;
}
