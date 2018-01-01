package com.cloudpos.jniinterface;

public class IDCardProperty {
	public byte[] strName;
	public byte[] strSex;
	public byte[] strNation;
	public byte[] strBorn;
	public byte[] strAddress;
	public byte[] strIDCardNo;
	public byte[] strGrantDept;
	public byte[] strUserLifeBegin;
	public byte[] strUserLifeEnd;
	public byte[] strReserved;
	public byte[] strPicture;

	public IDCardProperty() {
		strName = new byte[30];
		strSex = new byte[2];
		strNation = new byte[4];
		strBorn = new byte[16];
		strAddress = new byte[70];
		strIDCardNo = new byte[36];
		strGrantDept = new byte[30];
		strUserLifeBegin = new byte[16];
		strUserLifeEnd = new byte[16];
		strReserved = new byte[36];
		strPicture = new byte[1024];
	}
}
