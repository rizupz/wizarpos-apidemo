/*
 * pinpad_utility.h
 *
 *  Created on: 2012-8-8
 *      Author: yaomaobiao
 */

#ifndef PINPAD_UTILITY_H_
#define PINPAD_UTILITY_H_

int asscii_to_bcd(char* strAscii, int nAsciiLength, unsigned char* strBCDBuffer, int nBCDBufferLength);
int ansi_98_pin_block(char* strCardNumber, char* strPIN, unsigned char* pKey, unsigned int nKeyLength, unsigned char* pResult);
int calculate_mac_x99(unsigned char* pData, int nDataLength, unsigned char* pKey, int nKeyLength, unsigned char* pMACOutBuffer, int nMACOutBufferLength);
int calculate_mac_ecb(unsigned char* pData, int nDataLength, unsigned char* pKey, int nKeyLength, unsigned char* pMACOutBuffer, int nMACOutBufferLength);

#endif /* PINPAD_UTILITY_H_ */
