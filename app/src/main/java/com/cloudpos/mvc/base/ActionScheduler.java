package com.cloudpos.mvc.base;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;

import com.cloudpos.mvc.common.Logger;

/**
 * action调度器，调度ActionContext．如果请求过多，将会排队等候，直到执行完成．每个ActionContext都由一条线程执行并管理．
 * @author lizhou
 */
public class ActionScheduler extends Thread {
	private static ActionScheduler actionScheduler = new ActionScheduler();
	
	private LinkedBlockingQueue<ActionContext> mActionQueue = new LinkedBlockingQueue<ActionContext>(20);
	private ExecutorService service = Executors.newFixedThreadPool(30);
	
	public static ActionScheduler getInstance() {
		return actionScheduler;
	}
	
	public void run() {
		ActionContext mContext = null;
		while (true) {
			try {
				mContext = mActionQueue.take();
				service.submit(mContext);
			} catch (Exception e) {
				Logger.error("调度器发生错误", e);
			}
		}
	}
	
	public void setActionContext(ActionContext context) {
		if (context == null) {
			return;
		}
		try {
			mActionQueue.put(context);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
