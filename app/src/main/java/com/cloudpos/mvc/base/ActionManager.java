package com.cloudpos.mvc.base;

import java.util.HashMap;
import java.util.Map;

import android.content.Context;

import com.cloudpos.mvc.common.Logger;
import com.cloudpos.mvc.common.MVCException;

/**
 * Action管理器．允许修改此类，初始化Action．UI通过此入口，向Action提交消息
 * @author lizhou
 */
public final class ActionManager {
	private static ActionManager actionManager = new ActionManager();
	
	private ActionScheduler actionScheduler = ActionScheduler.getInstance();
	protected Map<String, Object> mActionContainer = new HashMap<String, Object>();
	private boolean isStart = false;

	private static ActionManager getInstance() {
		if (actionManager.isStart == false) {
			actionManager.start();
			actionManager.isStart = true;
		}
		return actionManager;
	}
	
	/** 开启调度器 */
	private void start() {
		this.actionScheduler.start();
	}
	
	/** 第一步：请记得初始化ActionContainer */
	public static void initActionContainer(ActionContainer actions) {
		actions.initActions();
		getInstance().mActionContainer.putAll(actions.getActions());
	}
	
	/**
	 * 向action提交消息，异步返回消息
	 * @param actionUrl 访问action的url. 形式：actionId/methodName.
	 * @param param 参数
	 * @param callback 响应回调
	 */
	public static void doSubmit(String actionUrl, Map<String, Object> param, ActionCallback callback) {
		doSubmit(actionUrl, null, param, callback);
	}
	
	/**
	 * 向action提交消息，异步返回消息
	 * @param clazz 访问的类
	 * @param methodName 方法名称
	 * @param param 参数
	 * @param callback　响应回调
	 */
	public static void doSubmit(Class<? extends AbstractAction> clazz, String methodName, Map<String, Object> param, ActionCallback callback) {
		doSubmit(clazz.getName() + "/" + methodName, param, callback);
	}
	
	/**
	 * 向action提交消息，异步返回消息
	 * @param actionUrl 访问action的url. 形式：actionId/methodName.
	 * @param context android context
	 * @param param 参数
	 * @param callback 响应回调
	 */
	public static void doSubmit(String actionUrl, Context context, Map<String, Object> param, ActionCallback callback) {
		getInstance().newActionContext(actionUrl, context, param, callback);
	}
	
	/**
	 * 向action提交消息，异步返回消息
	 * @param clazz 访问的类
	 * @param methodName 方法名称
	 * @param context android context
	 * @param param 参数
	 * @param callback　响应回调
	 */
	public static void doSubmit(Class<? extends AbstractAction> clazz, String methodName, Context context, Map<String, Object> param, ActionCallback callback) {
		doSubmit(clazz.getName() + "/" + methodName, context, param, callback);
	}

	/**
	 * 向action提交消息，同步返回消息
	 * @param actionUrl 访问action的url. 形式：actionId/methodName.
	 * @param param 参数
	 * @param callback　响应回调
	 * @return 同步返回值
	 */
	public static <T> T doSubmitForResult(String actionUrl, Map<String, Object> param, ActionCallback callback) {
		return doSubmitForResult(actionUrl, null, param, callback);
	}
	
	/**
	 * 向action提交消息，同步返回消息
	 * @param clazz 访问的类
	 * @param methodName 方法名称
	 * @param param 参数
	 * @param callback　响应回调
	 * @return
	 */
	public static <T> T doSubmitForResult(Class<? extends AbstractAction> clazz, String methodName, Map<String, Object> param, ActionCallback callback) {
		return doSubmitForResult(clazz.getName() + "/" + methodName, param, callback);
	}
	
	/**
	 * 向action提交消息，同步返回消息
	 * @param actionUrl 访问action的url. 形式：actionId/methodName.
	 * @param context 
	 * @param param 参数
	 * @param callback　响应回调
	 * @return 同步返回值
	 */
	@SuppressWarnings("unchecked")
	public static <T> T doSubmitForResult(String actionUrl, Context context, Map<String, Object> param, ActionCallback callback) {
		ActionContext acontext = getInstance().newActionContext(actionUrl, context, param, callback);
		return (T) acontext.getResult();
	}
	
	/**
	 * 向action提交消息，同步返回消息
	 * @param clazz 访问的类
	 * @param methodName 方法名称
	 * @param context android context
	 * @param param 参数
	 * @param callback　响应回调
	 * @return
	 */
	public static <T> T doSubmitForResult(Class<? extends AbstractAction> clazz, String methodName, Context context, Map<String, Object> param, ActionCallback callback) {
		return doSubmitForResult(clazz.getName() + "/" + methodName, context, param, callback);
	}
	
	/** 统一初始化ActionContext */
	private ActionContext newActionContext(String actionUrl, Context context, Map<String, Object> param, ActionCallback callback) {
		ActionContext acontext = new ActionContext();
		acontext.setActionUrl(actionUrl);
		acontext.setParam(param);
		acontext.setCallback(callback);
		if (acontext != null) {
			acontext.setContext(context);
		}
		setAction(actionUrl, acontext);
		actionScheduler.setActionContext(acontext);
		return acontext;
	}
	
	/** set action into ActionContext */
	private void setAction(String actionUrl, ActionContext context) {
		String actionId = ActionContext.parseActionId(actionUrl);
		Object obj = mActionContainer.get(actionId);//从key的value
		if (obj == null) {
			throw new MVCException("Not found actionId in ActionContainer. The actionId is [" + actionId + "].");
		}
		if (Class.class.isInstance(obj)) {
			try {
				context.setAction((AbstractAction) Class.class.cast(obj).newInstance());
			} catch (Exception e) {
				Logger.error("build instance error:", e);
			}
		} else {
			context.setAction((AbstractAction) obj);
		}
	}

}
