package com.cloudpos.jniinterface;

public class LEDInterface {
	static{
		System.loadLibrary("jni_cloudpos_led");
	}
    public native static int open();
    public native static int close();
    public native static int turnOn(int index);
    public native static int turnOff(int index);
    /**
     * get the status of led
     * @param[in] : unsigned int nLedIndex : index of led, >= 0 && < MAX_LED_COUNT
     * @return value : == 0 : turn off
     *                 > 0 : turn on
     *                 < 0 : error code
     */
    public native static int getStatus(int index);

}
