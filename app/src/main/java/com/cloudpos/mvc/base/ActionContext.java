package com.cloudpos.mvc.base;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.Method;
import java.net.UnknownHostException;
import java.util.Map;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

import android.content.Context;

import com.cloudpos.mvc.common.Logger;

/**
 * ActionContext，管理Action的方法调用，控制其参数输入与结果输出．其生命周期仅为一次请求之间．类似于http请求.
 * @author lizhou
 */
public class ActionContext implements Runnable {

	private AbstractAction action;
	
	private Context context;
	private Map<String, Object> param;
	private ActionCallback callback;
	private Object result = null;
	
	private String actionUrl;
	private String methodName;
	
	private ReentrantLock resultLock = new ReentrantLock();
	private Condition resultCondition = resultLock.newCondition(); 
	private boolean hasReturn = false;

	@Override
	public void run() {
		if (action == null) {
			Logger.error("Not found action! Please initional ActionContain and register your Action Class");
			return;
		}
		if (callback == null) {
			Logger.warn("No call back");
		}
		try {
			resultLock.lock();
			
			action.setContext(context);
			action.doBefore(param, callback);
			invoke();
			action.doAfter(param, callback);
		} catch (Exception e) {
			String errorMsg = "Invoke method error: " + action.getClass().getName() + "#" + methodName;
			if (e.getCause() == null) {
				Logger.error(errorMsg, e);
			} else {
				if (e.getCause() instanceof UnknownHostException) {
					Logger.error(errorMsg);
					Logger.error(getStackTraceString(e.getCause()));
				} else {
					Logger.error(errorMsg, e.getCause());
				}
			}
		} finally {
			hasReturn = true;
			resultCondition.signalAll();
			resultLock.unlock();
		}
	}
	
	/** 同步取返回结果 */
	public Object getResult() {
		resultLock.lock();
		try {
			if (hasReturn == false) {
				resultCondition.await();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			resultLock.unlock();
		}
		return result;
	}
	
	private void invoke() throws Exception {
		parseActionUrl();
		Class<?> callbackParam = ActionCallback.class;
		if (callback == null) {
		} else {
			callbackParam = callback.getClass().getSuperclass();
		}
		BeanHelper helper = new BeanHelper(action);
		Method method = helper.getMethod(methodName, Map.class, callbackParam);
		result = method.invoke(action, param, callback);
	}
	
	
	/**
	 * 解析actionId
	 * @param actionUrl
	 * @return
	 */
	public static String parseActionId(String actionUrl) {
		int index = actionUrl.indexOf("/");
		if (index == -1) {
			return actionUrl;
		}
		return actionUrl.substring(0, index);
	}
	
	private void parseActionUrl() {
		int index = actionUrl.indexOf("/");
		if (index == -1) {
			methodName = "execute";
		} else {
			methodName = actionUrl.substring(index + 1);
		}
	}
	
	public static String getStackTraceString(Throwable tr) {
        if (tr == null) {
            return "";
        }
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        tr.printStackTrace(pw);
        return sw.toString();
    }

	public void setParam(Map<String, Object> param) {
		this.param = param;
	}

	public void setCallback(ActionCallback callback) {
		this.callback = callback;
	}

	public void setActionUrl(String actionUrl) {
		this.actionUrl = actionUrl;
	}

	public void setContext(Context context) {
		this.context = context;
	}
	
	public void setAction(AbstractAction action) {
		this.action = action;
	}
	
}
