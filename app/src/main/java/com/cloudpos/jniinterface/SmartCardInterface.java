
package com.cloudpos.jniinterface;

public class SmartCardInterface {
    static {
        System.loadLibrary("jni_cloudpos_smartcard");
    }

    public static final int[] LOGICAL_ID = new int[] {
            0x00, 0x01, 0x02, 0x03
    };

    /**
     * The function query the max the slot in this smart card reader
     * 
     * @return value < 0 : error code, value == 0 : not defined, value > 0 ;
     *         number of slot.
     */
    public native static int queryMaxNumber();

    /**
     * The function query whether the smart card is not existent Attention : not
     * every slot can support this function
     * 
     * @param nSlotIndex : Slot index, from 0 to ( MAX_SUPPORT_SLOT - 1 )
     * @return value < 0 : error code, value == 0 : not existent, value > 0 ; be
     *         existent.
     */
    public native static int queryPresence(int nSlotIndex);

    /**
     * The function open the specified card
     * 
     * @param nSlotIndex : Slot index, from 0 to (MAX_SUPPORT_SLOT - 1).
     * @return value < 0 : error code, value >= 0: success, return value is a
     *         handle.This handle will be employed bye other API as an input
     *         parameter
     */
    public native static int open(int nSlotIndex);

    /**
     * The function initialize the smart card reader
     * 
     * @param handle : returned from method of open
     * @return value >= 0, success in starting the process; value < 0, error
     *         code
     */
    public native static int close(int handle);
    
    /**
     * set PSAM card baudrate and validate
     * nBuadrate 可以取 9600 跟38400 
     * nVoltage 可以取 1, 2 ,3 分别对应1.8v 3v 5v
     */
    public native static int  setCardInfo(int handle, int nBuadrate , int nVoltage);

    /**
     * @param Handle : returned from method of open
     * @param byteArrayATR : ATR
     * @param info : card information
     * @return value >= 0 : ATR length value < 0 : error code
     */
    public native static int powerOn(int handle, byte byteArrayATR[], SmartCardSlotInfo info);

    /**
     * The function power off the smart card
     * 
     * @param handle : return from method of open
     * @return value >= 0, success in starting the process; value < 0, error
     *         code
     */

    public native static int powerOff(int handle);

    /**
     * The function set the slot control information
     * 
     * @param Handle ：return from method of open
     * @param info ：SMART_CARD_SLOT_INFO* pSlotInfo
     * @return value >= 0, success in starting the process; value < 0, error
     *         code
     */

    public native static int setSlotInfo(int handle, SmartCardSlotInfo info);

    /**
     * @param Handle : return from method of open
     * @param byteArrayAPDU : command of APDU
     * @param nAPDULength : length of command of APDU
     * @param byteArrayResponse : response of command of APDU
     * @return value >= 0 : response data length value < 0 : error code
     */
    public native static int transmit(int handle, byte byteArrayAPDU[], byte byteArrayResponse[]);

    /**
     * This function is responsible for reading data from memory card
     * 
     * @param[in] : int Handle, return from method of open
     * @param[in] : unsigned int nAreaType, area type : 0 : main memory, 1 :
     *            protected memory 2 : security memory
     * @param[in][out] : unsigned char* pDataBuffer : data buffer
     * @param[in] : unsigned int nDataLength : data length of expecting reading
     * @param[in] : unsigned char cStartAddress : starting address
     * @return value : < 0 : error code >= 0 : data length
     */
    public native static int read(int handle, int nAreaType, byte[] byteArryData, int nStartAddress);

    /**
     * This function is responsible for writing data to memory card
     * 
     * @param[in] : int Handle, return from method of open
     * @param[in] : unsigned int nAreaType, area type : 0 : main memory, 1 :
     *            protected memory 2 : security memory
     * @param[in] : unsigned char* pData : data buffer
     * @param[in] : unsigned int nDataLength : data length of expecting reading
     * @param[in] : unsigned char cStartAddress : starting address
     * @return value : < 0 : error code >= 0 : data length
     */

    public native static int write(int handle, int nAreaType, byte[] byteArryData, int nStartAddress);

    /**
     * Verification of data
     * 
     * @param[in] : int Handle, return from method of open
     * @param[in] : unsigned char* pData : data buffer
     * @param[in] : unsigned int nDataLength : data length
     * @return value : < 0 : error code = 0 : failed in verifying data > 0 :
     *         success
     */
    public native static int verify(int handle, byte byteArrayAPDU[]);

    // 回调
    public static Object objPresent = new Object();
    public static Object objAbsent = new Object();
    public static SmartCardEvent event;

    public static void callBack(byte[] data) {
        event = new SmartCardEvent();
        event.nEventID = data[0];
        event.nSlotIndex = data[1];
        if (data[0] == SmartCardEvent.SMART_CARD_EVENT_INSERT_CARD) {
            synchronized (objPresent) {
                objPresent.notifyAll();
            }
        } else if (data[0] == SmartCardEvent.SMART_CARD_EVENT_REMOVE_CARD) {
            synchronized (objAbsent) {
                objAbsent.notifyAll();
            }
        }

    }
}
