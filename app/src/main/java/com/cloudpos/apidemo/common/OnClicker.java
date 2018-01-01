package com.cloudpos.apidemo.common;

import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;

public class OnClicker {
	/**
	 * 控件组绑定事件
	 */
	public static void setOnClickListenerByIds(Activity host, int[] widget, OnClickListener listener) {
		for (int viewId : widget) {
			setOnClickListenerById(host, viewId, listener);
		}
	}
	
	/**
	 * 控件绑定事件
	 * 
	 * @param viewId 控件id
	 * @param listener
	 *            事件
	 */
	public static void setOnClickListenerById(Activity host, int viewId, View.OnClickListener listener) {
		host.findViewById(viewId).setOnClickListener(listener);
	}
}
