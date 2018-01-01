
package com.cloudpos.jniinterface;

public class RFCardInterface {
    // load *.so library for use
    static {
        System.loadLibrary("jni_cloudpos_rfcard");
    }
    public static int CONTACTLESS_CARD_MODE_AUTO = 0;
    public static int CONTACTLESS_CARD_MODE_TYPE_A = 1;
    public static int CONTACTLESS_CARD_MODE_TYPE_B = 2;
    public static int CONTACTLESS_CARD_MODE_TYPE_C = 3;

    public static int RC500_COMMON_CMD_GET_READER_VERSION = 0x40;
    public static int RC500_COMMON_CMD_RF_CONTROL = 0x38; // command data :
                                                          // 0x01(turn on),
                                                          // 0x00(turn off)

    public static final int CONTACTLESS_CARD_EVENT_FOUND_CARD = 0;
    public static final int CONTACTLESS_CARD_EVENT_TIME_OUT = 1;
    public static final int CONTACTLESS_CARD_EVENT_COMM_ERROR = 2;
    public static final int CONTACTLESS_CARD_EVENT_USER_CANCEL = 3;

    /**
     * 语义代表对非接触设备进行占用并且初始化非接触式设备。 Initialize the contactless1 card reader
     * 
     * @return value == 0, error ; value != 0 , correct handle
     */
    public native static int open();

    /**
     * 语义代表对非接触设备解除占用，在调用完毕非接触式卡后，需要调用，否则其他人无法使用。 Close the contactless1 card
     * reader
     * 
     * @return value >= 0, success ; value < 0, error code
     */
    public native static int close();

    /**
     * 语义代表通知非接设备开始寻卡， Start searching the contactless1 card If you set the
     * nCardMode is auto, reader will try to activate card in type A, type B and
     * type successively; If you set the nCardMode is type A, type B, or type C,
     * reader only try to activate card in the specified way.
     * 
     * @param : nCardMode : handle of this card reader
     *        ，寻找那种类型的非接卡，一般使用CONTACTLESS_CARD_MODE_AUTO来自动匹配卡片
     *        ，或者根据卡片文档来选择卡片类型来适配搜寻范围。 possible value :
     *        CONTACTLESS_CARD_MODE_AUTO = 0 CONTACTLESS_CARD_MODE_TYPE_A = 1;
     *        CONTACTLESS_CARD_MODE_TYPE_B = 2; CONTACTLESS_CARD_MODE_TYPE_C =
     *        3;
     * @param nFlagSearchAll : 0 : signal user if we find one card in the field
     *            1 : signal user only we find all card in the field 找到范围内的所有卡片。
     * @param nTimeout_MS : time out in milliseconds. if nTimeout_MS is less
     *            then zero, the searching process is infinite. 超时
     * @return value >= 0, success in starting the process; value < 0, error
     *         code
     */
    public native static int searchBegin(int nCardMode, int nFlagSearchAll, int nTimeout_MS);

    /**
     * 语义代表通知非接设备停止寻卡。 Stop the process of searching card.
     * 
     * @return :value >= 0, success in starting the process; value < 0, error
     *         code
     */

    public native static int searchEnd();

    /**
     * 语义代表通知非接设备对卡片上电。 Attach the target before transmitting apdu command In
     * this process, the target(card) is activated and return ATR
     * 
     * @param byteArrayATR ATR buffer, if you set it null, you can not get the
     *            data.
     * @return :value >= 0, success in starting the process and return length of
     *         ATR; value < 0, error code
     */
    public native static int attach(byte byteArrayATR[]);

    /**
     * 语义代表通知非接设备对卡片下电。 Detach the target. If you want to send APDU again, you
     * should attach it.
     * 
     * @return :value >= 0, success in starting the process; value < 0, error
     *         code
     */
    public native static int detach();

    /**
     * 语义代表通知非接设备对卡片发送通讯命令，来执行对卡的操作。 Transmit APDU command and get the response
     * 
     * @param byteArrayAPDU : command of APDU ：根据文档来发送命令的byte数组。
     * @param nAPDULength : length of command of APDU ：命令的长度
     * @param byteArrayResponse : response of command of APDU，返回数据
     * @return value >= 0, success in starting the process; value < 0, error
     *         code
     */
    public native static int transmit(byte byteArrayAPDU[], int nAPDULength,
            byte byteArrayResponse[]);

    /**
     * 语义代表对非接设备的细化处理,用于完成特殊的非接卡，功能有打开射频关闭射频，以及获得非接设备的版本号等信息。 Send control
     * command.
     * 
     * @param nCmdID : id of command
     * @param byteArrayCmdData : data associated with command, if no data, you
     *            can set it NULL
     * @param nDataLength : data length of command
     * @return value >= 0, success in starting the process; value < 0, error
     *         code
     */

    public native static int sendControlCommand(int nCmdID, byte byteArrayCmdData[], int nDataLength);

    /**
     * 语义代表验证MiFareone卡的密码。 Verify pin only for MiFare one card
     * 
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nPinType : 0 : A type 1 : B type
     * @param[in] : unsigned char* strPin : password of this pin
     * @param[in] : unsigned int nPinLength : length of password return value :
     *            >= 0 : success
     */
    public native static int verify(int nSectorIndex, int nPinType, byte[] strPin, int nPinLength);

    /**
     * 语义代表在验证MiFareone卡后读取数据. Read data only for MiFare one card
     * 
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index
     * @param[out] : unsigned char* pDataBuffer : data buffer
     * @param[in] : unsigned int nDataBufferLength : buffer length return value
     *            : >= 0 : data length < 0 : error code
     */
    public native static int read(int nSectorIndex, int nBlockIndex, byte[] pDataBuffer,
            int nDataBufferLength);

    /**
     * 语义代表在验证MiFareone卡对数据的写入操作. Write data only for MiFare one card
     * 
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index
     * @param[in] : unsigned char* pData : data
     * @param[in] : unsigned int nDataLength : data length return value : >= 0 :
     *            success < 0 : error code
     */
    public native static int write(int nSectorIndex, int nBlockIndex, byte[] pData, int nDataLength);

    /**
     * @param[in] : int nHandle : handle of this card reader
     * @param[out] : unsigned int* pHasMoreCards : 0 : only one PICC in the
     *             field 0x0A : more cards in the field(type A) 0x0B : more
     *             cards in the field(type B) 0xAB : more cards in the
     *             field(type A and type B)
     * @param[out] : unsigned int * pCardType : CONTACTLESS_CARD_TYPE_A_CPU
     *             0x0000 CONTACTLESS_CARD_TYPE_B_CPU 0x0100
     *             CONTACTLESS_CARD_TYPE_A_CLASSIC_MINI 0x0001
     *             CONTACTLESS_CARD_TYPE_A_CLASSIC_1K 0x0002
     *             CONTACTLESS_CARD_TYPE_A_CLASSIC_4K 0x0003
     *             CONTACTLESS_CARD_TYPE_A_UL_64 0x0004
     *             CONTACTLESS_CARD_TYPE_A_UL_192 0x0005
     *             CONTACTLESS_CARD_TYPE_A_MP_2K_SL1 0x0006
     *             CONTACTLESS_CARD_TYPE_A_MP_4K_SL1 0x0007
     *             CONTACTLESS_CARD_TYPE_A_MP_2K_SL2 0x0008
     *             CONTACTLESS_CARD_TYPE_A_MP_4K_SL2 0x0009
     *             CONTACTLESS_CARD_UNKNOWN 0x00FF return value : >= 0 : success
     *             < 0 : error code
     */
    public native static int queryInfo(int[] pHasMoreCards, int[] pCardType);

    /**
     * Read value from a block
     * 
     * @param[in] : int nHandle : handle of this card reader
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index
     * @param[out] : unsigned char* pValue : buffer for saving value. LSB,
     *             4bytes
     * @param[in] : unsigned int nValueBufLength : must be greater than 4
     * @param[out] : unsigned char* pAddrData : one byte, for saving a user data
     *             return value : >= 0 : success < 0 : error code
     */
    public native static int readValue(int nSectorIndex, int nBlockIndex, byte[] pValue,
            int nValueBufLength, byte[] pAddrData);

    /**
     * Write value to a block
     * 
     * @param[in] : int nHandle : handle of this card reader
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index
     * @param[in] : unsigned char* pValue : data for saving value. LSB, 4bytes
     * @param[in] : unsigned int nValueBufLength : must be 4
     * @param[in] : unsigned char* pAddrData : one byte, for saving a user data
     *            return value : >= 0 : success < 0 : error code
     */
    public native static int writeValue(int nSectorIndex, int nBlockIndex, int pValue,
            int nValueLength, byte pAddrData);

    /**
     * Increment value to a block, using it with the API :
     * hal_contactless_card_mc_restore and hal_contactless_card_mc_transfer
     * 
     * @param[in] : int nHandle : handle of this card reader
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index
     * @param[in] : unsigned char* pValue : buffer for saving value. LSB, 4bytes
     * @param[in] : unsigned int nValueBufLength : must be greater than 4 return
     *            value : >= 0 : success < 0 : error code
     */
    public native static int increment(int nSectorIndex, int nBlockIndex, int value);

    /**
     * Decrement value to a block, using it with the API :
     * hal_contactless_card_mc_restore and hal_contactless_card_mc_transfer
     * 
     * @param[in] : int nHandle : handle of this card reader
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index
     * @param[in] : unsigned char* pValue : buffer for saving value. LSB, 4bytes
     * @param[in] : unsigned int nValueBufLength : must be greater than 4 return
     *            value : >= 0 : success < 0 : error code
     */
    public native static int decrement(int nSectorIndex, int nBlockIndex, int value);

    /**
     * Save the value to the block from temporary buffer
     * 
     * @param[in] : int nHandle : handle of this card reader
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index return value : >= 0 :
     *            success < 0 : error code
     */
    public native static int transfer(int nSectorIndex, int nBlockIndex);

    /**
     * Read the value to the temporary from the block
     * 
     * @param[in] : int nHandle : handle of this card reader
     * @param[in] : unsigned int nSectorIndex : sector index
     * @param[in] : unsigned int nBlockIndex : block index return value : >= 0 :
     *            success < 0 : error code
     */
    public native static int restore(int nSectorIndex, int nBlockIndex);

    /**
     * 初始化的参数
     */
    public static final int NONE = -1;

    public static Object object = new Object();
    public static int nEventID = NONE;
    public static byte[] arryEventData = null;

    public static void callBack(int nEvent, byte[] nData) {
        nEventID = nEvent;
        if (nData!=null) {
            arryEventData = new byte[nData.length];
            for (int i = 0; i < nData.length; i++) {
                arryEventData[i] = nData[i];
            }
        }

        synchronized (object) {
            // Log.i("RFCard", "notify");
            object.notifyAll();
        }
    }

}
