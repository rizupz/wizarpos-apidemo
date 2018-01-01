package com.cloudpos.jniinterface;

public class IDCardInterface {
	static
	{
		System.loadLibrary("jni_cloudpos_idcard");
	}
	/*native interface */
	
			
	public native static int open();
	public native static int close();
	public native static int getInformation(IDCardProperty data);
	public native static int searchTarget();

}
