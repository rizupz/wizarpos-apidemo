package com.synjones.bluetooth;

import android.util.Log;

public class DecodeWlt {
	
	// wlt数据的存储路径，
	
	/**
	 * @param wltPath : 以wlt为格式的文件的存储路径，
	 * @param bmpPath : 对wlt为格式的文件进行解密后的bmp格式的图片的保存路径。
	 * @param errorCode : 返回错误码
	 * */
	public static native int Wlt2Bmp(String wltPath, String bmpPath);
	static{
		try {
			System.loadLibrary("DecodeWlt");
		} catch (Exception e) {
			// TODO: handle exception
			Log.e("jni", "i can't find this so!");
            e.printStackTrace();
		}
		
	}
}
