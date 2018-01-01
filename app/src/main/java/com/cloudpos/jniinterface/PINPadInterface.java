
package com.cloudpos.jniinterface;

public class PINPadInterface {
    /* native interface */
    static {
        System.loadLibrary("jni_cloudpos_pinpad");
    }
    
    /**
     * ALGO_CHECK_VALUE 类型
     */
    public static final int ALGO_CHECK_VALUE_DEFAULT  =0;
    public static final int ALGO_CHECK_VALUE_SE919  =1;

    /** key类型dukpt */
    public final static int KEY_TYPE_DUKPT = 0;
    /** key类型tdukpt */
    public final static int KEY_TYPE_TDUKPT = 1;
    /** key类型master */
    public final static int KEY_TYPE_MASTER = 2;
    /** key类型public */
    public final static int KEY_TYPE_PUBLIC = 3;
    /** key类型fix */
    public final static int KEY_TYPE_FIX = 4;

    /** master keyID 只有再key类型为master才有效 */
    public static final int[] MASTER_KEY_ID = new int[] {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09
    };
    /** user keyID 只有再key类型为master才有效 */
    public static final int[] USER_KEY_ID = new int[] {
            0x00, 0x01
    };

    /** 加密算法 3DES */
    public static final int ALGORITH_3DES = 1;
    /** 加密算法 DES */
    public static final int ALGORITH_DES = 0;

    public final static int MAC_METHOD_X99 = 0;
    public final static int MAC_METHOD_ECB = 1;

    public native static int open();

    public native static int close();

    public native static int showText(int nLineIndex, byte arryText[], int nTextLength,
            int nFlagSound);

    public native static int selectKey(int nKeyType, int nMasterKeyID, int nUserKeyID, int nAlgorith);

    public native static int setPinLength(int nLength, int nFlag);

    public native static int encrypt(byte arryPlainText[], int nTextLength,
            byte arryCipherTextBuffer[]);

    public native static int calculatePINBlock(byte arryASCIICardNumber[], int nCardNumberLength,
            byte arryPinBlockBuffer[], int nTimeout_MS, int nFlagSound);

    public native static int calculateMac(byte arryData[], int nDataLength, int nMACFlag,
            byte arryMACOutBuffer[]);

    public native static int updateUserKey(int nMasterKeyID, int nUserKeyID,
            byte arryCipherNewUserKey[], int nCipherNewUserKeyLength);

    public native static int updateCipherMasterKey(int nMasterKeyID, byte[] arryCipherNewMasterKey,
            int nCipherNewMasterKeyLength, byte[] pCheckValue, int nCheckValueLen);

    public native static int updateUserKeyWithCheck(int nMasterKeyID, int nUserKeyID,
            byte[] arryCipherNewUserKey, int nCipherNewUserKeyLength, int nUserKeyType,
            byte[] checkValue,
            int checkValueLength);

    public native static int updateMasterKey(int nMasterKeyID, byte arrayOldKey[],
            int nOldKeyLength, byte arrayNewKey[], int nNewKeyLength);

    public native static int updateUserKeyWithCheckE(int nMasterKeyID, int nUserKeyID,
            byte[] arryCipherNewUserKey, int nCipherNewUserKeyLength, int nUserKeyType,
            byte[] checkValue, int checkValueLength, int algoCheckValue);

    public native static int updateCipherMasterKeyE(int nMasterKeyID,
            byte[] arryCipherNewMasterKey,
            int nCipherNewMasterKeyLength, byte[] pCheckValue, int nCheckValueLen,
            int algoCheckValue);

    /*
     * get serial number
     * @param[out] : unsigned char* pData : serial number buffer return value :
     * < 0 : error code >= 0 : success, length of serial number
     */
    public native static int getSerialNo(byte arrySerialNo[]);

    // 开启回调. (被回调的方法为 void pinpadCallback(byte[] v) - v[0]: 星号个数; v[1]: 额外参数
    // >=0 : success;
    // -1: has not find lib
    // -2: has not find pinpad_set_pinblock_callback in lib
    // -3: has not find PinpadCallback in Java code
    public native static int setPinblockCallback();

    private static PinPadCallbackHandler callbackHandler;

    public static int setupCallbackHandler(PinPadCallbackHandler handler) {
        callbackHandler = handler;
        if (handler != null) {
            return setPinblockCallback();
        }
        return 0;
    }

    public static void pinpadCallback(byte[] data) {
        if (callbackHandler != null) {
            callbackHandler.processCallback(data);
        }
    }
}
