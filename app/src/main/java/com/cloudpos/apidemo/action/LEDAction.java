package com.cloudpos.apidemo.action;

import java.util.Map;

import android.app.Activity;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.common.Common;
import com.cloudpos.apidemo.function.ActionCallbackImpl;
import com.cloudpos.jniinterface.LEDInterface;
import com.cloudpos.jniinterface.PINPadInterface;
import com.cloudpos.jniinterface.SmartCardInterface;
import com.cloudpos.mvc.base.AbstractAction;

public class LEDAction extends ConstantAction {
	private static int index = 0;

	private void setParams(Map<String, Object> param, ActionCallbackImpl callback) {
		this.mCallback = callback;
	}

	public void open(Map<String, Object> param, ActionCallbackImpl callback) {
		setParams(param, callback);
		if (isOpened) {
			callback.sendFailedMsg(mContext.getResources().getString(R.string.device_opened));
		} else {
			try {
				int result = LEDInterface.open();
				if (result < 0) {
					callback.sendFailedMsg(mContext.getResources().getString(R.string.operation_with_error) + result);
				} else {
					isOpened = true;
					callback.sendSuccessMsg(mContext.getResources().getString(R.string.operation_successful));
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
                int result = LEDInterface.close();
                return result;
            }
        });
	}

	public void turnOn(Map<String, Object> param, ActionCallbackImpl callback) {
		setParams(param, callback);
		checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                return LEDInterface.turnOn(index);
            }
        });
	}

	public void turnOff(Map<String, Object> param, ActionCallbackImpl callback) {
		setParams(param, callback);
		checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                return LEDInterface.turnOff(index);
            }
        });
	}

	public void getStatus(Map<String, Object> param, ActionCallbackImpl callback) {
		setParams(param, callback);
		checkOpenedAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = LEDInterface.getStatus(index);
                if (result == 0) {
                    mCallback.sendSuccessMsg("LED " + index + " is OFF");
                } else if (result > 0) {
                    mCallback.sendSuccessMsg("LED " + index + " is ON");
                }
                return result;
            }
        });
	}
}
