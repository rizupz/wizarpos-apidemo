
package com.cloudpos.apidemo.action;

import java.util.Map;

import android.app.Activity;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.common.Common;
import com.cloudpos.apidemo.function.ActionCallbackImpl;
import com.cloudpos.apidemo.util.StringUtility;
import com.cloudpos.jniinterface.PINPadInterface;
import com.cloudpos.mvc.base.AbstractAction;

public class PINPadAction extends ConstantAction {

    private int masterKeyID = 0;
    private int userKeyID = 0;

    private int minPINLength = 4;
    private int maxPINLength = 12;

    private void setParams(Map<String, Object> param, ActionCallbackImpl callback) {
        this.mCallback = callback;
    }

    public void open(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        if (isOpened) {
            callback.sendFailedMsg(mContext.getResources().getString(R.string.device_opened));
        } else {
            try {
                int result = PINPadInterface.open();
                if (result < 0) {
                    callback.sendFailedMsg(mContext.getResources().getString(
                            R.string.operation_with_error)
                            + result);
                } else {
                    isOpened = true;
                    callback.sendSuccessMsg(mContext.getResources().getString(
                            R.string.operation_successful));
                }
            } catch (Throwable e) {
                e.printStackTrace();
                callback.sendFailedMsg(mContext.getResources().getString(R.string.operation_failed));
            }
        }
    }

    public void close(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);

        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                isOpened = false;
                result = PINPadInterface.close();
                return result;
            }
        });
    }

    public void setPINLength(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.setPinLength(minPINLength, 0);
                return result;
            }
        });
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.setPinLength(maxPINLength, 1);
                return result;
            }
        });
    }

    public void showText(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        final byte[] arryTextLine0 = "Show Test:".getBytes();
        final byte[] arryTextLine1 = new byte[] {
                (byte) 0x83, (byte) 0x84, (byte) 0x85, (byte) 0x86, (byte) 0x87
        };
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.showText(0, arryTextLine0, arryTextLine0.length, 0);
                return result;
            }
        });
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.showText(1, arryTextLine1, arryTextLine1.length, 0);
                return result;
            }
        });
    }

    public void clearText(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.showText(0, null, 0, 0);
                return result;
            }
        });
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.showText(1, null, 0, 0);
                return result;
            }
        });
    }

    public void updateMasterKey(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        String masterKey = "09 FA 17 0B 03 11 22 76 09 FA 17 0B 03 11 22 76";
        final byte[] arryCipherNewMasterKey = new byte[16];
        StringUtility.StringToByteArray(masterKey, arryCipherNewMasterKey);
        final byte[] checkValue = new byte[4];
        StringUtility.StringToByteArray("A5 17 3A D5", checkValue);

        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.updateCipherMasterKey(0, arryCipherNewMasterKey,
                        arryCipherNewMasterKey.length, checkValue, checkValue.length);
                return result;
            }
        });
    }

    public void updateMasterKeyE(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        String masterKey = "AF CF 1B E6 80 AF EF E7 CB 14 6A 78 36 CB 3C DF";
        final byte[] arryCipherNewMasterKey = new byte[16];
        StringUtility.StringToByteArray(masterKey, arryCipherNewMasterKey);
        final byte[] checkValue = new byte[8];
        // String checkValues =
        // "AF CF 1B E6 80 AF EF E7 CB 14 6A 78 36 CB 3C DF 00 00 00 00 00 00 00 00";
        String checkValues = "A4 E1 F0 7B 50 08 7E 1F";
        StringUtility.StringToByteArray(checkValues, checkValue);

        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.updateCipherMasterKeyE(0, arryCipherNewMasterKey,
                        arryCipherNewMasterKey.length, checkValue, checkValue.length,
                        PINPadInterface.ALGO_CHECK_VALUE_SE919);
                return result;
            }
        });
    }

    public void updateUserKey(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        String userKey = "09 FA 17 0B 03 11 22 76 09 FA 17 0B 03 11 22 76";
        final byte[] arryCipherNewUserKey = new byte[16];
        StringUtility.StringToByteArray(userKey, arryCipherNewUserKey);

        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.updateUserKey(0, 0, arryCipherNewUserKey,
                        arryCipherNewUserKey.length);
                return result;
            }
        });
    }

    public void updateUserKeyWithCheck(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        // plain user key = 00 01 02 03 04 05 06 07 00 01 02 03 04 05 06 07
        String userKey = "09 FA 17 0B 03 11 22 76 09 FA 17 0B 03 11 22 76";
        final byte[] arryCipherNewUserKey = new byte[16];
        StringUtility.StringToByteArray(userKey, arryCipherNewUserKey);
        String checkValue = "A5 17 3A D5";
        final byte[] arryCheckValue = new byte[4];
        StringUtility.StringToByteArray(checkValue, arryCheckValue);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.updateUserKeyWithCheck(0, 0, arryCipherNewUserKey,
                        arryCipherNewUserKey.length, 0, arryCheckValue, arryCheckValue.length);
                return result;
            }
        });
    }

    public void updateUserKeyWithCheckE(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        String userKey = "20 AA 6B 47 80 43 EB 38 71 82 31 E9 3C F3 74 C9";
        final byte[] arryCipherNewUserKey = new byte[16];
        StringUtility.StringToByteArray(userKey, arryCipherNewUserKey);
        // String checkValue =
        // "20 AA 6B 47 80 43 EB 38 71 82 31 E9 3C F3 74 C9 01 00 00 00 00 00 00 00";
        String checkValue = "FF 76 69 A5 78 EE 60 A7";
        final byte[] arryCheckValue = new byte[8];
        StringUtility.StringToByteArray(checkValue, arryCheckValue);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = PINPadInterface.updateUserKeyWithCheckE(0, 1, arryCipherNewUserKey,
                        arryCipherNewUserKey.length, 1, arryCheckValue, arryCheckValue.length,
                        PINPadInterface.ALGO_CHECK_VALUE_SE919);
                return result;
            }
        });
    }

    public void getSerialNo(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        final byte[] arrySerialNo = new byte[40];
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = PINPadInterface.getSerialNo(arrySerialNo);
                if (result >= 0) {
                    mCallback.sendSuccessMsg("SerialNo = "
                            + StringUtility.ByteArrayToString(arrySerialNo, result));
                }
                return result;
            }
        });

    }

    private void selectKey(final int masterKeyID, final int userKeyID) {
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                return PINPadInterface.selectKey(PINPadInterface.KEY_TYPE_MASTER, masterKeyID,
                        userKeyID, 0);
            }
        });
    }

    public void selectKey(Map<String, Object> param, ActionCallbackImpl callback) {
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                return PINPadInterface.selectKey(PINPadInterface.KEY_TYPE_MASTER, 0,
                        0, 0);
            }
        });
    }

    public void encryptData(Map<String, Object> param, ActionCallbackImpl callback) {
        // the default userkeyID to encrypt data is 2--Data key
        setParams(param, callback);
        userKeyID = 2;
        selectKey(masterKeyID, userKeyID);
        // The length of arrPlainData can be 255
        final byte[] arryPlainData = Common.createMasterKey(8);
        callback.sendSuccessMsg("Plain Data = " + StringUtility.ByteArrayToString(arryPlainData, 8));
        final byte[] arryCipherData = new byte[255];
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = PINPadInterface.encrypt(arryPlainData, arryPlainData.length,
                        arryCipherData);
                if (result >= 0) {
                    mCallback.sendSuccessMsg("Encrypted Data = "
                            + StringUtility.ByteArrayToString(arryCipherData, result));
                }
                return result;
            }
        });
    }

    public void calculateMAC(Map<String, Object> param, ActionCallbackImpl callback) {
        // the default userkeyID to calculateMAC is 1--MAC key
        setParams(param, callback);
        userKeyID = 1;
        selectKey(masterKeyID, userKeyID);
        // The length of arrPlainData is 8
        final byte[] arryMACInData = Common.createMasterKey(16);
        callback.sendSuccessMsg("MAC In Data = "
                + StringUtility.ByteArrayToString(arryMACInData, 16));
        final byte[] arryMACOutData = new byte[32];
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = PINPadInterface.calculateMac(arryMACInData, arryMACInData.length, 0,
                        arryMACOutData);
                if (result >= 0) {
                    mCallback.sendSuccessMsg("MAC Out Data = "
                            + StringUtility.ByteArrayToString(arryMACOutData, result));
                }
                return result;
            }
        });
    }

    public void calculatePINBlock(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        PINBlockThread thread = new PINBlockThread();
        thread.start();
    }

    class PINBlockThread extends Thread {

        @Override
        public void run() {
            // the default userkeyID to calculatePINBlock is 0--PIN key
            // The length of pan should be 13-19
            userKeyID = 0;
            selectKey(masterKeyID, userKeyID);
            String pan = "1234567890123456789";
            byte[] arryPINblock = new byte[18];
            try {
                int result = PINPadInterface.calculatePINBlock(pan.getBytes(), pan.length(),
                        arryPINblock, -1, 0);
                if (result < 0) {
                    mCallback.sendFailedMsg(mContext.getResources().getString(
                            R.string.operation_with_error)
                            + result);
                } else {
                    mCallback.sendSuccessMsg("PIN Block = "
                            + StringUtility.ByteArrayToString(arryPINblock, result));
                }
            } catch (Throwable e) {
                e.printStackTrace();
                mCallback.sendFailedMsg(mContext.getResources()
                        .getString(R.string.operation_failed));
            }
        }

    }

}
