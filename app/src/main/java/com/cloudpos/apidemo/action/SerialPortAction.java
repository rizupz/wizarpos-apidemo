
package com.cloudpos.apidemo.action;

import java.util.Map;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.common.SystemProperties;
import com.cloudpos.apidemo.function.ActionCallbackImpl;
import com.cloudpos.jniinterface.SerialPortInterface;

public class SerialPortAction extends ConstantAction {

    private int baudrate = 115200;
    private String testString = "wizarpos";
    private static String deviceName = "";

    private void setParams(Map<String, Object> param, ActionCallbackImpl callback) {
        this.mCallback = callback;
    }

    public void open(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        if (isOpened) {
            mCallback.sendFailedMsg(mContext.getResources().getString(R.string.device_opened));
        } else {
            try {
                int result = SerialPortInterface.open(getModelName());
                if (result < 0) {
                    mCallback.sendFailedMsg("open "
                            + mContext.getResources().getString(R.string.operation_with_error)
                            + result);
                } else {
                    isOpened = true;
                    mCallback.sendSuccessMsg("open "
                            + mContext.getResources().getString(R.string.operation_successful));
                }
            } catch (Throwable e) {
                e.printStackTrace();
                mCallback.sendFailedMsg("open "
                        + mContext.getResources().getString(R.string.operation_failed));
            }
        }
    }

    public void close(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                isOpened = false;
                int result = SerialPortInterface.close();
                return result;
            }
        });
    }

    public void flushIO(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = SerialPortInterface.flushIO();
                return result;
            }
        });
    }

    public void read(Map<String, Object> param, ActionCallbackImpl callback) {
        final byte[] arryData = new byte[testString.length()];
        final int length = testString.length();
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = SerialPortInterface.read(arryData, length, 3000);
                return result;
            }
        });
    }

    public void write(Map<String, Object> param, ActionCallbackImpl callback) {
        final byte[] arryData = new String(testString).getBytes();
        final int length = 2;
        final int offset = 2;
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = SerialPortInterface.write(arryData, offset, length);
                return result;
            }
        });
    }

    public void setBaudrate(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = SerialPortInterface.setBaudrate(baudrate);
                return result;
            }
        });
    }

    private String getModelName() {
        String deviceName = "/dev/s3c2410_serial2";
        String model = SystemProperties.getSystemPropertie("ro.product.model").trim();
        model = model.replace(" ", "_");
        model = model.toUpperCase();
        if (model.equals("WIZARHAND_Q1") || model.equals("MSM8610") || model.equals("WIZARHAND_Q0")) {
            deviceName = "WIZARHANDQ1";
        }
        return deviceName;
    }

}
