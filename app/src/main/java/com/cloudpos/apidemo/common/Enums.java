package com.cloudpos.apidemo.common;

public class Enums {
	
	//Log类型
	/**
	 * 日志类型
	 * @author zxd
	 *
	 */
	public class StateLog{
		public static final int LOG = 1;
		public static final int LOG_SUCCESS = 2;
		public static final int LOG_FAILED = 3;
	}
	
	/**
	 * 正在使用的网络类型
	 */
	public class NetType{
		public static final int WIFI = 1;
		public static final int ETH0 = 2;
		public static final int G3 = 3;
		public static final int ALL = 0;
	}
	//状态
	/**
	 * 主菜单类型
	 * @author zxd
	 *
	 */
	public enum StateType {
		pinpad, 
		printer, 
		smartcard, 
		contactless, 
		msr, 
		networktest, 
		soundtest, 
		serial, 
		psam, 
		sdcard, 
		led, 
		about, 
		btntest, 
		moneybox, 
		serialno, 
		hsm1850, 
		customer, 
		yali
	}

}
