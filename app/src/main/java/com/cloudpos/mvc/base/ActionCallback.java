package com.cloudpos.mvc.base;

import android.content.Context;

/**
 * 回调方法，允许扩展，适应实际需求.
 * @author lizhou
 */
public abstract class ActionCallback {
	protected Context context;
	
	public ActionCallback() {}
	
	public ActionCallback(Context context) {
		this.context = context;
	}
	
	public Context getContext() {
		return this.getContext();
	}

	public void sendResponse(int code) {}
	public void sendResponse(String msg) {}
	public void sendResponse(int code, String msg) {}
}
