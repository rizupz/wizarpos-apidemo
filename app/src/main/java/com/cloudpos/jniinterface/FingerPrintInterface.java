
package com.cloudpos.jniinterface;

public class FingerPrintInterface {
    static {
        System.loadLibrary("jni_cloudpos_fingerprint");
    }

    public native static int open();

    public native static int close();

    public native static int getFea(byte[] arryFea, int nFeaLength, int[] pRealFeaLength,
            int n_TimeOut_S);

    public native static int getLastImage(byte[] pImgBuffer, int nImgLength, int[] pRealImaLength,
            int[] pImgWidth, int[] pImgHeight);

    public native static int match(byte[] pFeaBuffer1, int nFea1Length, byte[] pFealBuffer2,
            int nFea2Length);

    public native static int cancel();
}
