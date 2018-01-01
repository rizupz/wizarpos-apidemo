package com.cloudpos.mvc.base;

import java.util.Map;

import android.content.Context;

/**
 * Action的父类
 * @author lizhou
 */
public abstract class AbstractAction {
	protected Context mContext;
	
	public void setContext(Context context) {
		this.mContext = context;
	}

	/**
	 * 这个方法在每个方法被调用之前都会被调用一次.
	 */
	protected void doBefore(Map<String, Object> param, ActionCallback callback) {
	}
	
	/**
	 * 这个方法在每个方法被调用之前都会被调用一次.
	 */
	protected void doAfter(Map<String, Object> param, ActionCallback callback) {
	}
}
