package com.cloudpos.mvc.base;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.cloudpos.mvc.common.Logger;

class BeanHelper {
	private Class<?> mClass;
	private Object mObject;
	private Method[] declaredMethods;
	
	public BeanHelper(Class<?> clazz) {
		mClass = clazz;
	}
	
	public BeanHelper(Object obj) {
		mObject = obj;
		mClass = mObject.getClass();
	}

	public Method getMethod(String methodName, Class<?>...classes) throws NoSuchMethodException {
		declaredMethods = mClass.getDeclaredMethods();
		Method result = null;
		int matchLevel = -1;
		boolean isFirst = true;
		for (Method method : declaredMethods) {
			String name = method.getName();
			if (!name.equals(methodName)) { // filter the item which name dont matched
				continue;
			}
			Class<?>[] paramTypes = method.getParameterTypes();
			if (paramTypes.length != classes.length) { // filter the item which the length of parameter list dont matched
				continue;
			}
			int tempMatchLevel = matchLevel(paramTypes, classes);
			if (tempMatchLevel < 0) {
				continue;
			}
			if (isFirst && matchLevel < tempMatchLevel) {
				isFirst = false;
				matchLevel = tempMatchLevel;
			} else if (matchLevel < tempMatchLevel) { // compare and find the method of matching degree is higher.
				matchLevel = tempMatchLevel;
			} else {
				continue;
			}
			result = method;
		}
		if (result == null) {
			throw new NoSuchMethodException(methodName + " " + Arrays.asList(classes).toString() + "");
		}
		return result;
	}
	
	public Class<?> getClosestClass(Class<?> clazz) {
		return clazz.getSuperclass();
	}
	
	/**
	 * calculate the method matching degree. which is represent with int level.It's important
	 * @param paramTypes
	 * @param destParamTypes
	 * @return metch level. begin from 1. 1 is th highest level. the level number is bigger, matching degree is lower.
	 */
	public int matchLevel(Class<?>[] paramTypes, Class<?>[] transferParamTypes) {
		int matchLevel = -1;
		for (int m = 0; m < paramTypes.length; m++) {
			Class<?> paramType = paramTypes[m];
			Class<?> tParamType = transferParamTypes[m];
			if (paramType.equals(tParamType)) {
				matchLevel += 1;
				continue;
			}
			List<Class<?>> superClasses = getAllSuperClass(tParamType);
			for (int n = 1; n <= superClasses.size(); n++) {
				Class<?> superClass = superClasses.get(n - 1);
				if (superClass == null || superClass.equals(paramType)) {
					matchLevel += n;
				} else {
					break;
				}
			}
		}
		return matchLevel;
	}
	
	/**
	 * Get all super class of current Class
	 * @param clazz Current Class
	 * @return a super class list
	 */
	public static List<Class<?>> getAllSuperClass(Class<?> clazz) {
		List<Class<?>> classes = new ArrayList<Class<?>>();
		Class<?> cla = clazz;
		do {
			cla = cla.getSuperclass();
			Logger.debug("class: " + clazz + ", super class: " + cla);
			if (cla != null) {
				classes.add(cla);
			}
			if (cla != null && cla.equals(Object.class)) {
				break;
			}
		} while (cla != null);
		return classes;
	}
	
}
