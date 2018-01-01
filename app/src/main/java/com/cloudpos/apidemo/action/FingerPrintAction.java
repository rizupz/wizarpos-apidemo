
package com.cloudpos.apidemo.action;

import java.util.Map;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.function.ActionCallbackImpl;
import com.cloudpos.jniinterface.FingerPrintInterface;

public class FingerPrintAction extends ConstantAction {
    
    static byte[] arryFea1 = new byte[4096];

    private void setParams(Map<String, Object> param, ActionCallbackImpl callback) {
        this.mCallback = callback;
    }

    public void open(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        if (isOpened) {
            callback.sendFailedMsg(mContext.getResources().getString(R.string.device_opened));
        } else {
            try {
                int result = FingerPrintInterface.open();
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
                int result = FingerPrintInterface.close();
                return result;
            }
        });
    }

    public void getFea(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        //该长度应>=2K
        final int nFeaLength = 4096;
        final int[] pRealFeaLength = new int[1];
        final int n_TimeOut_S = -1;
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = FingerPrintInterface
                        .getFea(arryFea1, nFeaLength, pRealFeaLength, n_TimeOut_S);
                return result;
            }
        });
    }

    public void getLastImage(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
      //该长度应>=32K
        final byte[] pImgBuffer = new byte[33*1024];
        final int nImgLength = 33*1024;
        final int[] pRealImaLength = new int[1];
        final int[] pImgWidth = new int[1];
        final int[] pImgHeight = new int[1];
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                return FingerPrintInterface.getLastImage(pImgBuffer, nImgLength, pRealImaLength,
                        pImgWidth, pImgHeight);
            }
        });
    }
    
    public void getFea(final byte[] arryFea) {
        //该长度应>=2K
        final int nFeaLength = 4096;
        final int[] pRealFeaLength = new int[1];
        final int n_TimeOut_S = -1;
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = FingerPrintInterface
                        .getFea(arryFea, nFeaLength, pRealFeaLength, n_TimeOut_S);
                return result;
            }
        });
    }

    public void match(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
//该长度应>=2K
        final byte[] arryFea2 = new byte[4096];
        getFea(arryFea2);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = FingerPrintInterface.match(arryFea1, arryFea1.length, arryFea2,
                        arryFea2.length);
                return result;
            }
        });
    }

    public void cancel(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = FingerPrintInterface.cancel();
                return result;
            }
        });
    }
}
