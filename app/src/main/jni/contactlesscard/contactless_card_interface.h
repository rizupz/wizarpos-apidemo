/*
 * contactless_card_interface.h
 *
 *  Created on: 2012-7-11
 *      Author: yaomaobiao
 */

#ifndef CONTACTLESS_CARD_INTERFACE_H_
#define CONTACTLESS_CARD_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define CONTACTLESS_CARD_MODE_AUTO		0
#define CONTACTLESS_CARD_MODE_TYPE_A	1
#define CONTACTLESS_CARD_MODE_TYPE_B	2
#define CONTACTLESS_CARD_MODE_TYPE_C	3

#define CONTACTLESS_CARD_EVENT_FOUND_CARD		0
#define CONTACTLESS_CARD_EVENT_TIME_OUT			1
#define CONTACTLESS_CARD_EVENT_COMM_ERROR		2
#define CONTACTLESS_CARD_EVENT_USER_CANCEL		3
/*
 * this is an inner event, user will never receive this event.
 */
#define CONTACTLESS_CARD_EVENT_NO_CARD			0xFF

/*
 * When we found a card, the event data is ATR.
 */
typedef void (*CONTACTLESS_CARD_NOTIFIER)(void* pUserData, int nEvent, unsigned char* pEventData, int nDataLength);

/*
 * Initialize the contactless card reader
 * @param[in] : CONTACTLESS_CARD_NOTIFIER fNotifier : it is called when some events happen.
 * @param[in] : void* pUserData : user data, it is the first parameter of call back function
 * @param[out] : int* pErrorCode : error code if return value is equal to zero
 * return value : == 0, error
 * 				  != 0 , correct handle
 */
typedef void* (*contactless_card_open)(CONTACTLESS_CARD_NOTIFIER fNotifier, void* pUserData, int* pErrorCode);
/*
 * Close the contactless card reader
 * @param[in] : int nHandle : handle of this card reader
 * return vlaue : >= 0, success
 * 				  < 0, error code
 */
typedef int (*contactless_card_close)(int nHandle);

/*
 * Start searching the contactless card
 * If you set the nCardMode is auto, reader will try to activate card in type A, type B and type successively;
 * If you set the nCardMode is type A, type B, or type C, reader only try to activate card in the specified way.
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : int nCardMode : possible value :
 * 								CONTACTLESS_CARD_MODE_AUTO
 *								CONTACTLESS_CARD_MODE_TYPE_A
 *								CONTACTLESS_CARD_MODE_TYPE_B
 *								CONTACTLESS_CARD_MODE_TYPE_C
 * @param[in] : int nFlagSearchAll : 0 : signal user if we find one card in the field
 * 									 1 : signal user only we find all card in the field
 * @param[in] : nTimeout_MS : time out in milliseconds.
 * 							  if nTimeout_MS is less then zero, the searching process is infinite.
 * 							  You can terminate it using the function of contactless_card_search_target_end.
 * return value : >= 0, success in starting the process.
 * 				  < 0 , error code
 */
typedef int (*contactless_card_search_target_begin)(int nHandle, int nCardMode, int nFlagSearchAll, int nTimeout_MS);
/*
 * Stop the process of searching card.
 * @param[in] : int nHandle : handle of this card reader
 */
typedef int (*contactless_card_search_target_end)(int nHandle);

/*
 * Attach the target before transmitting apdu command
 * In this process, the target(card) is activated and return ATR
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned char* pATRBuffer : ATR buffer, if you set it null, you can not get the data.
 * @param[in] : unsigned int nATRBufferLength : length of ATR buffer.
 * return value : >= 0, success, length of ATR.
 * 				  < 0 , error code
 */

typedef int (*contactless_card_attach_target)(int nHandle, unsigned char* pATRBuffer, unsigned int nATRBufferLength);
/*
 * Detach the target. If you want to send APDU again, you should attach it.
 * @param[in] : int nHandle : handle of this card reader
 * return value : >= 0, success, length of ATR.
 * 				  < 0 , error code
 */
typedef int (*contactless_card_detach_target)(int nHandle);
/*
 * Transmit APDU command and get the response
 * @param[in] : int nHandle : handle of this card reader
 * @param[in]		: unsigned char* pAPDU : command of APDU
 * @param[in]		: unsigned int nAPDULength : length of command of APDU
 * @param[out]		: unsigned char* pResponse : response of command of APDU
 * @param[in][out]	: unsigned int* pResponseLength : [in], buffer length of response
 * 													  [out], length of response
 * return value : >= 0, success
 * 				  < 0 , error code
 */

typedef int (*contactless_card_transmit)(int nHandle, unsigned char* pAPDU, unsigned int nAPDULength, unsigned char* pResponse, unsigned int *pResponseLength);

/*
 * Send control command.
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nCmdID : id of command
 * @param[in][out] : unsigned char* pCmdData : data associated with command
 * 					[in] : data associated with command, if no data, you can set it NULL
 * 					[out]: response data
 *
 * @param[in] : unsigned int nDataLength : data length of command
 * return value : >= 0, success, response data length if any.
 * 				  < 0 , error code
 */
typedef int (*contactless_card_send_control_command)(int nHandle, unsigned int nCmdID, unsigned char* pCmdData, unsigned int nDataLength);

/*
 * Verify pin
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nPinType : 0 : A type
 * 										1 : B type
 * @param[in] : unsigned char* strPin : password of this pin
 * @param[in] : unsigned int nPinLength : length of password
 * return value : >= 0 : success
 * 				  < 0 : error code
 */
typedef int (*contactless_card_mc_verify_pin)(int nHandle, unsigned int nSectorIndex, unsigned int nPinType, unsigned char* strPin, unsigned int nPinLength);

/*
 * Read data
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 * @param[out] : unsigned char* pDataBuffer : data buffer
 * @param[in] : unsigned int nDataBufferLength : buffer length
 * return value : >= 0 : data length
 * 				  < 0 : error code
 */
typedef int (*contactless_card_mc_read)(int nHandle, unsigned int nSectorIndex, unsigned int nBlockIndex, unsigned char* pDataBuffer, unsigned int nDataBufferLength);

/*
 * Write data
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 * @param[in] : unsigned char* pData : data
 * @param[in] : unsigned int nDataLength : data length
 * return value : >= 0 : success
 *                < 0 : error code
 */

typedef int (*contactless_card_mc_write)(int nHandle, unsigned int nSectorIndex, unsigned int nBlockIndex, unsigned char* pData, unsigned int nDataLength);

/*
 * @param[in] : int nHandle : handle of this card reader
 * @param[out] : unsigned int* pHasMoreCards :
 *                              0 : only one PICC in the field
 *                              0x0A : more cards in the field(type A)
 *                              0x0B : more cards in the field(type B)
 *                              0xAB : more cards in the field(type A and type B)
           @param[out] : unsigned int * pCardType :
                                        CONTACTLESS_CARD_TYPE_A_CPU             0x0000
                                        CONTACTLESS_CARD_TYPE_B_CPU             0x0100
                                        CONTACTLESS_CARD_TYPE_A_CLASSIC_MINI    0x0001
                                        CONTACTLESS_CARD_TYPE_A_CLASSIC_1K      0x0002
                                        CONTACTLESS_CARD_TYPE_A_CLASSIC_4K      0x0003
                                        CONTACTLESS_CARD_TYPE_A_UL_64           0x0004
                                        CONTACTLESS_CARD_TYPE_A_UL_192          0x0005
                                        CONTACTLESS_CARD_TYPE_A_MP_2K_SL1       0x0006
                                        CONTACTLESS_CARD_TYPE_A_MP_4K_SL1       0x0007
                                        CONTACTLESS_CARD_TYPE_A_MP_2K_SL2       0x0008
                                        CONTACTLESS_CARD_TYPE_A_MP_4K_SL2       0x0009
                                        CONTACTLESS_CARD_UNKNOWN                0x00FF
 */
typedef int (*contactless_card_query_info)(int nHandle, int* pHasMoreCards, int* pCardType);

/*
 * Read value from a block
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 * @param[out] : unsigned char* pValue : buffer for saving value. LSB, 4bytes
 * @param[in] : unsigned int nValueBufLength : must be greater than 4
 * @param[out] : unsigned char* pAddrData : one byte, for saving a user data
 *
 * return value : >= 0 : success
 *                < 0 : error code
 */
typedef int (*contactless_card_mc_read_value)(int nHandle, unsigned int nSectorIndex, unsigned int nBlockIndex, unsigned char* pValue, unsigned int nValueBufLength, unsigned char* pAddrData);
/*
 * Write value to a block
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 * @param[in] : unsigned char* pValue : data for saving value. LSB, 4bytes
 * @param[in] : unsigned int nValueBufLength : must be 4
 * @param[in] : unsigned char* pAddrData : one byte, for saving a user data
 *
 * return value : >= 0 : success
 *                < 0 : error code
 */
typedef int (*contactless_card_mc_write_value)(int nHandle, unsigned int nSectorIndex, unsigned int nBlockIndex, unsigned char* pValue, unsigned int nValueLength, unsigned char bAddrData);
/*
 * Increment value to a block, using it with the API : hal_contactless_card_mc_restore and hal_contactless_card_mc_transfer
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 * @param[in] : unsigned char* pValue : buffer for saving value. LSB, 4bytes
 * @param[in] : unsigned int nValueBufLength : must be greater than 4
 *
 * return value : >= 0 : success
 *                < 0 : error code
 */
typedef int (*contactless_card_mc_increment)(int nHandle, unsigned int nSectorIndex, unsigned int nBlockIndex, unsigned char* pValue, unsigned int nValueLength);
/*
 * Decrement value to a block, using it with the API : hal_contactless_card_mc_restore and hal_contactless_card_mc_transfer
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 * @param[in] : unsigned char* pValue : buffer for saving value. LSB, 4bytes
 * @param[in] : unsigned int nValueBufLength : must be greater than 4
 *
 * return value : >= 0 : success
 *                < 0 : error code
 */
typedef int (*contactless_card_mc_decrement)(int nHandle, unsigned int nSectorIndex, unsigned int nBlockIndex, unsigned char* pValue, unsigned int nValueLength);

/*
 * Save the value to the block from temporary buffer
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 *
 * return value : >= 0 : success
 *                < 0 : error code
 */
typedef int (*contactless_card_mc_transfer)(int nHandle, unsigned int nSectorIndex, unsigned int nBlockIndex);
/*
 * Read the value to the temporary from the block
 * @param[in] : int nHandle : handle of this card reader
 * @param[in] : unsigned int nSectorIndex : sector index
 * @param[in] : unsigned int nBlockIndex : block index
 *
 * return value : >= 0 : success
 *                < 0 : error code
 */
typedef int (*contactless_card_mc_restore)(int nHandle,unsigned int nSectorIndex, unsigned int nBlockIndex);

#ifdef __cplusplus
}
#endif

#endif /* CONTACTLESS_CARD_INTERFACE_H_ */
