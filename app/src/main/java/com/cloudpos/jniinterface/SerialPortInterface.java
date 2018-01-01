
package com.cloudpos.jniinterface;

public class SerialPortInterface
{
    static
    {
        System.loadLibrary("jni_cloudpos_serial");
    }

    /* native interface */
    public native static int open(String deviceName);

    public native static int close();

    public native static int read(byte pDataBuffer[], int nExpectedDataLength, int nTimeout_MS);

    public native static int write(byte pDataBuffer[], int offset, int nDataLength);

    public native static int setBaudrate(int nBaudrate);

    public native static int flushIO();

}
