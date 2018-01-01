
package com.cloudpos.apidemo.function;

import android.os.Handler;
import android.os.Message;

import com.cloudpos.apidemo.common.Common;
import com.cloudpos.apidemo.common.Enums.StateLog;
import com.cloudpos.mvc.base.ActionCallback;

public class ActionCallbackImpl extends ActionCallback {

    private Handler handler;

    /**
     * 三种写日志的方式:
     * <p>
     * sendResponse(String msgString)写 普通 日志, 类型为Log
     * </p>
     * <p>
     * sendSuccessMsg(String msgString)写 成功 日志, 类型为SUCCESSLOG
     * </p>
     * <p>
     * sendFailedMsg(String msgString)写 失败 日志, 类型为FAILEDLOG
     * </p>
     * 
     * @param handler
     */
    public ActionCallbackImpl(Handler handler) {
        this.handler = handler;
    }

    @Override
    public void sendResponse(int code, String msgString) {
        Message msg = new Message();
        msg.what = code;
        msg.obj = "\t\t" + msgString + "\n";
        handler.sendMessage(msg);
    }

    @Override
    public void sendResponse(String msgString) {
        Message msg = new Message();
        msg.what = StateLog.LOG;
        msg.obj = "\t\t" + msgString + "\n";
        handler.sendMessage(msg);
    }

    public void sendFailedMsg(String msg) {
        int code = StateLog.LOG_FAILED;
        sendResponse(code, msg);
    }

    public void sendSuccessMsg(String msg) {
        int code = StateLog.LOG_SUCCESS;
        sendResponse(code, msg);
    }
    
    public void sendFailedMsgInCheck(String msg) {
        int code = StateLog.LOG_FAILED;
        msg = Common.getMethodName() + " " + msg;
        sendResponse(code, msg);
    }

    public void sendSuccessMsgInCheck(String msg) {
        int code = StateLog.LOG_SUCCESS;
        msg = Common.getMethodName() + " " + msg;
        sendResponse(code, msg);
    }
}
