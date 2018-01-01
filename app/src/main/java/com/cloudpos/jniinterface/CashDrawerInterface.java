package com.cloudpos.jniinterface;

public class CashDrawerInterface {
	static {
		System.loadLibrary("jni_cloudpos_cashdrawer");
	}
	/*
	 * open the money box device
	 * @return value : < 0 : error code
	 * 				   >= 0 : success;	
	 */
	public native static int open();
	/*
	 * close the money box device
	 * @return value : < 0 : error code
	 * 				   >= 0 : success;
	 */
	
	public native static int close();
	/*
	 * open money box
	 * @return value : < 0 : error code;
	 *                 >= 0 : success
	 */
	public native static int kickOut();
}
