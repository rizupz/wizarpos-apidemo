package com.cloudpos.jniinterface;

public class RFCardEvent 
{
	public static int nMaxEventDataLength = 0xFF;
	public int nEventID;
	public byte arryEventData[];
	public int nEventDataLength;
	
	public RFCardEvent()
	{
		arryEventData = new byte[nMaxEventDataLength];
		nEventDataLength = 0;
		nEventID = -1;
	}
}
