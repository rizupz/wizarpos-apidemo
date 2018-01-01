package com.cloudpos.mvc.base;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import com.cloudpos.mvc.common.Logger;

/**
 * Action容器，所有的action在其子类中初始化注入．
 * @author lizhou
 */
public abstract class ActionContainer {
	
	protected Map<String, Object> actions = new HashMap<String, Object>();
	
	/**
	 * 初始化Action
	 */
	public abstract void initActions();
	
	public Map<String, Object> getActions() {
		return this.actions;
	}
	
	private Object searchInstance(Class<? extends AbstractAction> clazz) throws Exception {
		for (Entry<String, Object> entry : actions.entrySet()) {
			Object value = entry.getValue();
			if (value == null) {
				continue;
			}
			if (value.getClass().equals(clazz)) {
				return value;
			}
		}
		return clazz.newInstance();
	}
	
	/**
	 * 添加Action到ActionContainer．在添加Action时，会根据singleton的值来确定是否创建对象，确保对象的单例模式
	 * @param actionId　action id
	 * @param clazz 对象的class
	 * @param singleton 是否单例
	 * @return true if success, or false if failure
	 */
	protected boolean addAction(String actionId, Class<? extends AbstractAction> clazz, boolean singleton) {
		if (singleton) {
			try {
				actions.put(actionId, searchInstance(clazz));
			} catch (Exception e) {
				Logger.error("build singleton instance occur an error:", e);
				return false;
			}
		} else {
			actions.put(actionId, clazz);
		}
		return true;
	}
	
	/**
	 * @see #addAction(String, Class, boolean)．每个Action在使用时都会创建一个新的对象
	 * @param actionId actoin id
	 * @param clazz 对象的class
	 * @return true if success, or false if failure
	 */
	protected boolean addAction(String actionId, Class<? extends AbstractAction> clazz) {
		return addAction(actionId, clazz, false);
	}
	
	
	/**
	 * 添加Action到ActionContainer．在添加Action时，会根据singleton的值来确定是否创建对象，确保对象的单例模式.<br/>
	 * The actionId is class.getName()
	 * @see #addAction(String, Class, boolean)
	 * @param clazz action class
	 * @param singleton 是否单例
	 * @return
	 */
	protected boolean addAction(Class<? extends AbstractAction> clazz, boolean singleton) {
		return addAction(clazz.getName(), clazz, singleton);
	}
	
	/**
	 * 每次都创建新的实例
	 * @see #addAction(Class, boolean) 
	 * @param clazz action class
	 * @return
	 */
	protected boolean addAction(Class<? extends AbstractAction> clazz) {
		return addAction(clazz, false);
	}
}
