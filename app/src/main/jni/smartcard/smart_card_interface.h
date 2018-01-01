/*
 * smart_card_interface.h
 *
 *  Created on: 2012-6-20
 *      Author: yaomaobiao
 */

#ifndef SMART_CARD_INTERFACE_H_
#define SMART_CARD_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define SMART_CART_MAX_SLOT			8

#define SMART_CARD_SLOT_INFO_FIDI		(1 << 0)
#define SMART_CARD_SLOT_INFO_EGT		(1 << 1)
#define SMART_CARD_SLOT_INFO_WI			(1 << 2)
#define SMART_CARD_SLOT_INFO_WTX		(1 << 3)
#define SMART_CARD_SLOT_INFO_EDC		(1 << 4)
#define SMART_CARD_SLOT_INFO_PROTOCOL	(1 << 5)
#define SMART_CARD_SLOT_INFO_POWER		(1 << 6)
#define SMART_CARD_SLOT_INFO_CONV		(1 << 7)
#define SMART_CARD_SLOT_INFO_IFSC		(1 << 8)
#define SMART_CARD_SLOT_INFO_CWT		(1 << 9)
#define SMART_CARD_SLOT_INFO_BWT		(1 << 10)


#define HAL_SCS_PROTOCOL_UNDEFINED	0x0000	/**< protocol not set */
#define HAL_SCS_PROTOCOL_T0			0x0001	/**< T=0 active protocol. */
#define HAL_SCS_PROTOCOL_T1			0x0002	/**< T=1 active protocol. */
#define HAL_SCS_PROTOCOL_RAW		0x0004	/**< Raw active protocol. */
#define HAL_SCS_PROTOCOL_T15		0x0008	/**< T=15 protocol. */


typedef struct smart_card_slot_info
{
    /** \showinitializer Speed communication (parameter TA1 of ISO 7816-3) */
    unsigned char FIDI;

    /** \showinitializer Extra Guard Time (parameter TC1 or N of ISO 7816-3) */
    unsigned char EGT;

    /**  \showinitializer
     * If protocol T=0 is selected, the parameter indicates the Waiting
     * Integer (parameter TC2 of ISO 7816-3 - the default value is 10)
     * If the protocol T=1 is selected, the parameter indicates the
     * Block and Character Waiting Time Integer (parameter TB3 of ISO 7816-3)
     */
    unsigned char WI;

     /** \showinitializer If the protocol T=1 is selected, the parameter indicates the
      * Waiting Time Extention (the default value is 1). */
    unsigned char WTX;

    /** \showinitializer If the protocol T=1 is selected, the parameter indicates the computing
     * mode for EDC : HAL_SCS_EDC_LRC or HAL_SCS_EDC_CRC (The default value
     * is an LRC) */
    unsigned char EDC;

    /** \showinitializer The parameter indicates the selected protocol :
     *  HAL_SCS_PROTOCOL_T0
     *  HAL_SCS_PROTOCOL_T1 */
    unsigned char protocol;

    /** \showinitializer The power supply value :
     *  HAL_SCS_POWER_1_8V
     *  HAL_SCS_POWER_3V
     *  HAL_SCS_POWER_5V
     */
    unsigned char power;

    /** \showinitializer Convention used to transfer byte :
     *  HAL_SCS_CONV_DIRECT
     *  HAL_SCS_CONV_INVERSE */
    unsigned char conv;

    /** \showinitializer If the protocol T=1 is selected, the parameter indicates the Information
     * Field Size for the Card (parameter TA3 of ISO 7816-3). */
    unsigned char IFSC;
    unsigned char reserved[3];

    /** \showinitializer Possibility to set Character Waiting Time */
    unsigned int cwt;
    /** \showinitializer Possibility to set Block Waiting Time */
    unsigned int bwt;
    /*
     * OR of these items:
     * 		SMART_CARD_SLOT_INFO_FIDI		(1 << 0)
	 *		SMART_CARD_SLOT_INFO_EGT		(1 << 1)
	 *		SMART_CARD_SLOT_INFO_WI			(1 << 2)
	 *		SMART_CARD_SLOT_INFO_WTX		(1 << 3)
	 *		SMART_CARD_SLOT_INFO_EDC		(1 << 4)
	 *		SMART_CARD_SLOT_INFO_PROTOCOL	(1 << 5)
	 *		SMART_CARD_SLOT_INFO_POWER		(1 << 6)
	 *		SMART_CARD_SLOT_INFO_CONV		(1 << 7)
	 *		SMART_CARD_SLOT_INFO_IFSC		(1 << 8)
	 * 		SMART_CARD_SLOT_INFO_CWT		(1 << 9)
	 *		SMART_CARD_SLOT_INFO_BWT		(1 << 10)
     *
     */
    unsigned int nSlotInfoItem;
}SMART_CARD_SLOT_INFO;

#define SMART_CARD_EVENT_INSERT_CARD		0
#define SMART_CARD_EVENT_REMOVE_CARD		1
#define SMART_CARD_EVENT_POWER_ON			2
#define SMART_CARD_EVENT_POWER_OFF			3


typedef void (*SMART_CARD_NOTIFIER)(void* pUserData, int nCardIndex, int nEvent);

/*
 * The function initialize the smart card reader
 * @return value	: < 0 : error
 * 					  >= 0 : success
 */
typedef int (*smart_card_init)();
/*
 * The function clean up the resource allocated by card reader.
 */
typedef int (*smart_card_terminate)();

/*
 * The function query the max the slot in this smart card reader
 *
 * @return value	: < 0 : error
 * 					  == 0 : not defined
 * 					  > 0 : number of slot
 */
typedef int (*smart_card_query_max_number)();

/*
 * The function query whether the smart card is not existent
 * Attention : not every slot can support this function
 * @param[in]	 : Slot index, from 0 to MAX_SUPPORT_SLOT - 1
 * @return value : < 0 : error
 * 				   = 0 : not existent
 * 				   > 0 : be existent
 */
typedef int (*smart_card_query_presence)(int nSlotIndex);

/*
 * The function open the specified card
 * @param[in]		: Slot index, from 0 to MAX_SUPPORT_SLOT - 1
 * @param[in]		: SMART_CARD_NOTIFIER pNotify : call back of smart card reader.
 * return value 	: < 0 : error
 * 					  >= 0 : success, return value is a handle.
 * 					  This handle will be employed bye other API as an input parameter
 *
 */
typedef int (*smart_card_open)(int nSlotIndex, SMART_CARD_NOTIFIER pNotify, void* pUserData);
/*
 * The function initialize the smart card reader
 * @param[in]		: int handle : return from method of open
 * @return value	: < 0 : error
 * 					  >= 0 : success
 */
typedef int (*smart_card_close)(int handle);

/**
 *nBuadrate 可以取 9600 跟38400
 *nVoltage	可以取 1, 2 ,3 分别对应1.8v 3v 5v
 */
typedef int (*smart_card_set_card_info)(int nHandle,int nBuadrate , int nVoltage);

/*
 * The function power on the smart card
 * @param[in]		: int Handle : return from method of open
 * @param[out]		: unsigned char* pATR : ATR
 * @param[in][out]	: unsigned int* pATRBufferLength :	[in], ATR buffer length
 * 														[out], length of ATR
 * @param[out]		: SMART_CARD_SLOT_INFO pCardInfo : card information
 * @return value	: < 0 , error
 * 						>= 0 : success
 */
typedef int (*smart_card_power_on)(int Handle, unsigned char* pATR, unsigned int* pATRBufferLength, SMART_CARD_SLOT_INFO* pSlotInfo);
/*
 * The function power off the smart card
 * @param[in]		: int Handle : return from method of open
 * @return value	: < 0, error
 * 					  >= 0 : success
 */
typedef int (*smart_card_power_off)(int Handle);

/*
 * The function set the slot control information
 * @param[in]		: int Handle : return from method of open
 * @param[in]		: SMART_CARD_SLOT_INFO* pSlotInfo
 * @return value	: < 0, error
 * 					  >= 0 : success
 */
typedef int (*smart_card_set_slot_info)(int Handle, SMART_CARD_SLOT_INFO* pSlotInfo);

/*
 * The function sends a command Application Protocol Data Unit(APDU) to a card
 * and retrieve the response APDU, plus the status words SW1 and SW2
 *
 * @param[in]		: int Handle : return from method of open
 * @param[in]		: unsigned char* pAPDU : command of APDU
 * @param[in]		: unsigned int nAPDULength : length of command of APDU
 * @param[out]		: unsigned char* pResponse : response of command of APDU
 * @param[in][out]	: unsigned int* pResponseLength : [in], buffer length of response
 * 													  [out], length of response
 * @return value	: < 0 : error
 * 					  >= 0 : success
 */
typedef int (*smart_card_transmit)(int Handle, unsigned char* pAPDU, unsigned int nAPDULength, unsigned char* pResponse, unsigned int *pResponseLength);
/*
 * This function is responsible for reading data from memory card
 * @param[in]		: int Handle, return from method of open
 * @param[in]		: unsigned int nAreaType, area type :
 * 													0 : main memory,
 * 													1 : protected memory
 * 													2 : security memory
 * @param[in][out]	: unsigned char* pDataBuffer : data buffer
 * @param[in]		: unsigned int nDataLength : data length of expecting reading
 * @param[in]		: unsigned char cStartAddress : starting address
 * @return value	: < 0 : error code
 * 					  >= 0 : data length
 */
typedef int (*smart_card_mc_read)(int Handle, unsigned int nAreaType, unsigned char* pDataBuffer, unsigned int nDataLength, unsigned char cStartAddress);

/*
 * This function is responsible for writing data to memory card
 * @param[in]		: int Handle, return from method of open
 * @param[in]		: unsigned int nAreaType, area type :
 * 													0 : main memory,
 * 													1 : protected memory
 * 													2 : security memory
 * @param[in]		: unsigned char* pData : data buffer
 * @param[in]		: unsigned int nDataLength : data length of expecting reading
 * @param[in]		: unsigned char cStartAddress : starting address
 * @return value	: < 0 : error code
 * 					  >= 0 : data length
 */
typedef int (*smart_card_mc_write)(int Handle, unsigned int nAreaType, unsigned char* pData, unsigned int nDataLength, unsigned char cStartAddress);
/*
 * Verification of data
 * @param[in]		: int Handle, return from method of open
 * @param[in]		: unsigned char* pData : data buffer
 * @param[in]		: unsigned int nDataLength : data length
 * @return value	: < 0 : error code
 * 					  = 0 : failed in verifying data
 * 					  > 0 : success
 */
typedef int (*smart_card_mc_verify_data)(int Handle, unsigned char* pData, unsigned int nDataLength);





#ifdef __cplusplus
}
#endif
#endif /* SMART_CARD_INTERFACE_H_ */
