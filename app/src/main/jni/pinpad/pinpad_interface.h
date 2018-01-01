/*
 * pinpad_interface.h
 *
 *  Created on: 2012-8-2
 *      Author: yaomaobiao
 */

#ifndef PINPAD_INTERFACE_H_
#define PINPAD_INTERFACE_H_

#define KEY_TYPE_DUKPT		0
#define KEY_TYPE_TDUKPT		1
#define KEY_TYPE_MASTER		2
#define KEY_TYPE_PUBLIC		3
#define KEY_TYPE_FIX		5

#ifdef __cplusplus
extern "C" {
#endif

/*
 * open the device
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*pinpad_open)();
/*
 * close the device
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*pinpad_close)();
/*
 * show text in the first line
 * param[in] : int nLineIndex : line index, from top to down
 * param[in] : char* strText : encoded string
 * param[in] : int nLength : length of string
 * param[in] : int nFlagSound : 0 : no voice prompt, 1 : voice prompt
 * return value : < 0 : error code, maybe, your display string is too long!
 * 				  >= 0 : success
 *
 */
typedef int (*pinpad_show_text)(int nLineIndex, char* strText, int nLength,
		int nFlagSound);
/*
 * select master key and user key
 * @param[in] : int nKeyType : 0 : dukpt, 1: Tdukpt, 2 : master key, 3 : public key, 4 : fix key
 * @param[in] : int nMasterKeyID : master key ID , [0x00, ..., 0x09], make sense only when nKeyType is master-session pair,
 * @param[in] : int nUserkeyID : user key ID, [0x00, 0x01], 		  make sense only when nKeyType is master-session pair,
 * @param[in] : int nAlgorith : 1 : 3DES
 * 							   0 : DES
 * return value : < 0 : error code
 * 				  >= 0 : success
 */
typedef int (*pinpad_select_key)(int nKeyType, int nMasterKeyID, int nUserKeyID,
		int nAlgorith);

/*
 * encrypt string using user key
 * @param[in] : unsigned char* pPlainText : plain text
 * @param[in] : int nTextLength : length of plain text
 * @param[out] : unsigned char* pCipherTextBuffer : buffer for saving cipher text
 * @param[in] : int CipherTextBufferLength : length of cipher text buffer
 * return value : < 0 : error code
 * 				  >= 0 : success, length of cipher text length
 */
typedef int (*pinpad_encrypt_string)(unsigned char* pPlainText, int nTextLength,
		unsigned char* pCipherTextBuffer, int nCipherTextBufferLength);

/*
 * calculate pin block
 * @param[in] : unsigned char* pASCIICardNumber : card number in ASCII format
 * @param[in] : int nCardNumberLength : length of card number
 * @param[out] : unsigned char* pPinBlockBuffer : buffer for saving pin block
 * @param[in] : int nPinBlockBufferLength : buffer length of pin block
 * @param[in] : int nTimeout_MS : timeout waiting for user input in milliseconds, if it is less than zero, then wait forever
 * param[in]   : int nFlagSound : 0 : no voice prompt, 1 : voice prompt
 * return value : < 0 : error code
 * 			      >= 0 : success, length of pin block
 */
typedef int (*pinpad_calculate_pin_block)(unsigned char* pASCIICardNumber,
		int nCardNumberLength, unsigned char* pPinBlockBuffer,
		int nPinBlockBufferLength, int nTimeout_MS, int nFlagSound);

/*
 * calculate the MAC using current user key
 * @param[in] : unsigned char* pData : data
 * @param[in] : int nDataLength : data length
 * @param[in] : int nMACFlag : 0: X99, 1 : ECB
 * @param[out] : unsigned char* pMACOut : MAC data buffer
 * @param[in] : int nMACOutBufferLength : length of MAC data buffer
 * return value : < 0 : error code
 * 				  >= 0 : success
 *
 */
typedef int (*pinpad_calculate_mac)(unsigned char* pData, int nDataLength,
		int nMACFlag, unsigned char* pMACOutBuffer, int nMACOutBufferLength);

/*
 * update the user key
 * @param[in] : int nMasterKeyID : master key id
 * @param[in] : int nUserKeyID : user key id
 * @param[in] : unsigned char* pCipherNewUserkey : new user key in cipher text
 * @param[in] : int nCipherNewUserKeyLength : length of new user key in cipher text
 * return value : < 0 : error code
 * 				  >= 0 : success
 */
typedef int (*pinpad_update_user_key)(int nMasterKeyID, int nUserKeyID,
		unsigned char* pCipherNewUserKey, int nCipherNewUserKeyLength);

/*
 * update the user key
 * @param[in] : int nMasterKeyID : master key id
 * @param[in] : int nUserKeyID : user key id
 * @param[in] : unsigned char* pCipherNewUserkey : new user key in cipher text
 * @param[in] : int nCipherNewUserKeyLength : length of new user key in cipher text
 * @param[in] : SESSION_KEY_USAGE : nKeyUsage
 * @param[in] : unsigned char* pCheckValue : check value of user key
 * @param[in] : int nCheckValueLen : length of check value, 4 bytes in general
 * return value : < 0 : error code
 *                   >= 0 : success
 */
//typedef int (*pinpad_update_user_key_with_check)(int nMasterKeyID,
//		int nUserKeyID, unsigned char *pCipherNewUserKey,
//		int nCipherNewUserKeyLength, SESSION_KEY_USAGE nKeyUsge,
//		unsigned char *pCheckValue, int nCheckValueLen);
typedef int (*pinpad_update_user_key_with_check)(int nMasterKeyID,
		int nUserKeyID, unsigned char *pCipherUserKey,
		int nCipherNewUserKeyLenght, int nKeyUsge, unsigned char *pCheckValue,
		int nCheckValueLen);

/*
 * update the master key
 * @param[in] : int nMasterKeyID : master key ID
 * @param[in] : unsigned char* pOldKey, old key
 * @param[in] : int nOldKeyLength : length of old key, 8 or 16
 * @param[in] : unsigned char* pNewLey : new key
 * @param[in] : int nNewLeyLength : length of new key, 8 or 16
 * return value : < 0 : error code
 * 				  >= 0 : success
 */
typedef int (*pinpad_update_master_key)(int nMasterKeyID,
		unsigned char* pOldKey, int nOldKeyLength, unsigned char* pNewKey,
		int nNewKeyLength);

/*
 * set the max length of pin
 * @param[in] : int nLength : length >= 0 && length <= 0x0D
 * @param[in] : int nFlag : 1, max length
 * 							0, min length
 * return value : < 0 : error code
 * 				  >= 0 : success
 */
typedef int (*pinpad_set_pin_length)(int nLength, int nFlag);

/*
 * get serial number
 * @param[out] : unsigned char* pData : serial number buffer
 * @param[in]  : unsigned int nBufferLength : length of serial number buffer
 * return value : < 0 : error code
 * 				  >= 0 : success, length of serial number
 */
typedef int (*pinpad_get_serial_number)(unsigned char* pData,
		unsigned int nBufferLength);

// 输入字符数回调接口
typedef void (*KEYEVENT_NOTIFIER)(int nCount, int nExtra);

typedef int (*pinpad_set_pinblock_callback)(KEYEVENT_NOTIFIER);

typedef int (*pinpad_update_cipher_master_key)(int nMasterKeyID,
		unsigned char* pCipherMasterKey, int nCipherMasterKeyLen,
		unsigned char *pCheckValue, int nCheckValueLen);

typedef int (*pinpad_update_user_key_with_check_E)(int nMasterKeyID, int nUserKeyID,
		unsigned char *pCipherNewUserKey, int nCipherNewUserKeyLength,
		int nKeyUsge, unsigned char *pCheckValue, int nCheckValueLen,
		int algoCheckValue);
typedef int (*pinpad_update_cipher_master_key_E)(int nMasterKeyID,
		unsigned char *pCipherMasterKey, int nCipherMasterKeyLen,
		unsigned char *pCheckValue, int nCheckValueLen,
		int algoCheckValue);

#ifdef __cplusplus
}
#endif

#endif /* PINPAD_INTERFACE_H_ */
