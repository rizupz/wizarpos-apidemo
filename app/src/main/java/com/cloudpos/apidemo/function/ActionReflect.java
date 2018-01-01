package com.cloudpos.apidemo.function;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;

import android.content.Context;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.util.StringUtility;

public class ActionReflect {

	//反射查找属性名和属性值
	/**
	 * 
	 * @param obj--要查找的类的实例，model--要查找的属性名
	 * @return 属性值--0表示没查找到model对应的属性名
	 */
	public static int getFields(Object obj, String model) {

		int stringId = 0;
		Field[] fields = obj.getClass().getFields();
		for (Field field : fields) {
			if (model.equals(field.getName())) {
				try {
					stringId = field.getInt(obj);
					return stringId;
				} catch (IllegalAccessException e1) {
					// e1.printStackTrace();
				} catch (IllegalArgumentException e2) {
					// e2.printStackTrace();
				}
			}
		}
		return stringId;
	}
	
	public static List<String> getStringsXml(Context con, String name){
		R.string strs = new R.string();
		int stringId = getFields(strs, name); // 找到该设备在R文件中存放的位置，从而获取目录等信息
		List<String> array = new ArrayList<String>();
		try{
		String temp = con.getString(stringId);
		String[] strs1 = StringUtility.spiltStrings(temp, ",");
		for (String string : strs1) {
			array.add(string);
		}
		}catch(Exception e){
			array = null;
		}
		return array;
	}
	
	public static List<String> getArraysXml(Context con, String name){
		R.array strs = new R.array();
		int stringId = getFields(strs, name); // 找到该设备在R文件中存放的位置，从而获取目录等信息
		List<String> array = new ArrayList<String>();
		try{
		String[] classItems = con.getResources().getStringArray(stringId);
		for(String classItem: classItems){
			array.add(classItem);
		}
		}catch(Exception e){
			array = null;
		}
		return array;
	}
	
}
