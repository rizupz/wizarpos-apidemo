package com.cloudpos.apidemo.function;

import android.os.Handler;
import android.os.Message;
import android.widget.TextView;

import com.cloudpos.apidemo.common.Enums.StateLog;
import com.cloudpos.apidemo.util.LogHelper;

public class HandlerImpl extends Handler {

	private TextView txtResult;

	/**
	 * 将信息输出到显示屏上
	 */
	public HandlerImpl(TextView textView) {
		this.txtResult = textView;
	}

	@Override
	public void handleMessage(Message msg) {
		switch (msg.what) {
		case StateLog.LOG:
			LogHelper.infoAppendMsg((String) msg.obj, txtResult);
			break;
		case StateLog.LOG_SUCCESS:
			LogHelper.infoAppendMsgForSuccess((String) msg.obj, txtResult);
			break;
		case StateLog.LOG_FAILED:
			LogHelper.infoAppendMsgForFailed((String) msg.obj, txtResult);
			break;
		default:
			LogHelper.infoAppendMsg((String) msg.obj, txtResult);
			break;
		}
	}

}
