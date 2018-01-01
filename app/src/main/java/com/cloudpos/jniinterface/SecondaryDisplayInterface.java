package com.cloudpos.jniinterface;

public class SecondaryDisplayInterface {

	static {
		System.loadLibrary("jni_cloudpos_secondarydisplay");
	}

	/**
     * open device return value : NULL : faled in opening device. else : handle
     * of device
     * @return value : 0 : success 
     *                 < 0 : error code
     */
    public native static int open();

    /**
     * write picture point data (one point 4 bytes ARGB)
     * 
     * @param[in] : int nHandle : handle of this device.
     * 
     * @param[in] char* pData : all point data.
     * 
     * @param[in] int nDataLength : point data length. 
     * @return value : 0 : success 
     *                 < 0 : error code
     */
    public native static int writePicture(int nXcoordinate, int nYcoordinate,int nWidth, int nHeight, byte[] pData, int nDataLength);

    /**
     * 背景色 
     * @param [in] : int nColor : 
     * <ul>
     * <li>RED :0x001F</li>
     * <li>BLACK:0X0000</li>
     * <li>YELLOW:0X07FF</li>
     * <li>BLUE:0XF800</li>
     * <li>GRAY0:0XCE9A</li>
     * </ul>              
     * @return value : 0 : success 
     *                 < 0 : error code
     * */
    public native static int setBackground(int nColor);
    /**
     * 蜂鸣器
     * @return value : 0 : success 
     *                 < 0 : error code
     * */
    public native static int buzzerBeep();
    /**
     * 后灯
     * @param[in] : int nValue : 1:open led. 0:close led.
     * @return value : 0 : success 
     *                 < 0 : error code
     * */
    public native static int ledPower(int nValue);
    /**
     * 默认屏
     * @return value : 0 : success 
     *                 < 0 : error code
     * */
    public native static int displayDefaultScreen();

    /**
     * close the device
     * 
     * @param[in] : int nHandle : handle of this device return value : 0 :
     * success < 0 : error code
     * @return value : 0 : success 
     *                 < 0 : error code
     */
    public native static int close();

}
