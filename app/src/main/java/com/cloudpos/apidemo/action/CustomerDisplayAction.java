
package com.cloudpos.apidemo.action;

import java.util.Map;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.common.BitmapConvert;
import com.cloudpos.apidemo.function.ActionCallbackImpl;
import com.cloudpos.jniinterface.SecondaryDisplayInterface;

public class CustomerDisplayAction extends ConstantAction {

    private void setParams(Map<String, Object> param, ActionCallbackImpl callback) {
        this.mCallback = callback;
    }

    public void open(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        if (isOpened) {
            callback.sendFailedMsg(mContext.getResources().getString(R.string.device_opened));
        } else {
            try {
                int result = SecondaryDisplayInterface.open();
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
                isOpened = false;
                int result = SecondaryDisplayInterface.close();
                return result;
            }
        });
    }

    public void buzzerBeep(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = SecondaryDisplayInterface.buzzerBeep();
                return result;
            }
        });
    }

    public void displayDefaultScreen(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = SecondaryDisplayInterface.displayDefaultScreen();
                return result;
            }
        });
    }

    public void setBackground(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = SecondaryDisplayInterface.setBackground(0x001F);
                return result;
            }
        });
    }

    public void writePicture(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        final Bitmap bitmap = BitmapFactory.decodeResource(mContext.getResources(),
                R.drawable.customer_display_picture);
        final byte[] arryBitmapData = BitmapConvert.Bitmap2Bytes(bitmap);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                result = SecondaryDisplayInterface.writePicture(0, 0, bitmap.getWidth(),
                        bitmap.getHeight(), arryBitmapData, arryBitmapData.length);
                return result;
            }
        });
    }
}
