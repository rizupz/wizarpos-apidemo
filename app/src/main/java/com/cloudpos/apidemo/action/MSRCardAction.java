
package com.cloudpos.apidemo.action;

import java.util.Map;

import android.util.Log;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.function.ActionCallbackImpl;
import com.cloudpos.jniinterface.MSRInterface;

public class MSRCardAction extends ConstantAction {

    private void setParams(Map<String, Object> param, ActionCallbackImpl callback) {
        this.mCallback = callback;
    }

    public void open(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        if (isOpened) {
            callback.sendFailedMsg(mContext.getResources().getString(R.string.device_opened));
        } else {
            try {
                int result = MSRInterface.open();
                if (result < 0) {
                    callback.sendFailedMsg(mContext.getResources().getString(
                            R.string.operation_with_error)
                            + result);
                } else {
                    isOpened = true;
                    callback.sendSuccessMsg(mContext.getResources().getString(
                            R.string.operation_successful));
                    CallBackThread thread = new CallBackThread();
                    thread.start();
                }
            } catch (Throwable e) {
                e.printStackTrace();
                callback.sendFailedMsg(mContext.getResources().getString(R.string.operation_failed));
            }
        }
    }

    public void close(Map<String, Object> param, ActionCallbackImpl callback) {
        setParams(param, callback);
        cancelCallBack();
        checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = 0;
                isOpened = false;
                result = MSRInterface.close();
                return result;
            }
        });
    }

    public void cancelCallBack() {
        synchronized (MSRInterface.object) {
            Log.i("MSRCard", "notify");
            MSRInterface.object.notifyAll();
            MSRInterface.eventID = EVENT_ID_CANCEL;
        }
    }

    private int getTrackData(final int trackNo, final byte[] arryData) {
        int result = checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = MSRInterface.getTrackData(trackNo, arryData, arryData.length);
                return result;
            }
        });
        return result;
    }

    private int getTrackDataLength(final int trackNo) {
        int result = checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = MSRInterface.getTrackDataLength(trackNo);
                return result;
            }
        });
        return result;
    }

    private int getTrackError(final int trackNo) {
        int result = checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = MSRInterface.getTrackError(trackNo);
                return result;
            }
        });
        return result;
    }

    class CallBackThread extends Thread {

        @Override
        public void run() {
            synchronized (MSRInterface.object) {
                try {
                    MSRInterface.object.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            if (MSRInterface.eventID == MSRInterface.CONTACTLESS_CARD_EVENT_FOUND_CARD) {
                mCallback.sendSuccessMsg("Find a card");
                int result = 0;
                for (int trackNo = 0; trackNo < MSRInterface.TRACK_COUNT; trackNo++) {
                    result = getTrackError(trackNo);
                    if (result < 0) {
                        continue;
                    }
                    result = getTrackDataLength(trackNo);
                    if (result < 0) {
                        break;
                    }
                    byte[] arryTrackData = new byte[result];
                    result = getTrackData(trackNo, arryTrackData);
                    if (result < 0) {
                        break;
                    }
                }
            } else if (MSRInterface.eventID == EVENT_ID_CANCEL) {
                mCallback.sendSuccessMsg("Cancel notifier");
            }
        }
    }

}
