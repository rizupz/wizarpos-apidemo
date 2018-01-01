/*
 * IDCard.java
 *
 * Copyright (c) 2013 - 2020 UnionPay. All rights reserved.
 *
 *
 * UNIONPAY PROPRIETARY/CONFIDENTIAL.
 *
 */

package com.cloudpos.apidemo.function.idcard;

import android.graphics.Bitmap;

/**
 * 本接口主要用于表示身份证
 * 
 * @date August 10, 2015
 */
public class IDCard {
	private String name;
	private String sex;
	private String nation;
	private String born;
	private String address;
	private String IDCardNo;
	

	private String grantDept;
	private String validFromDate;
	private String validToDate;
	private String reserved;
	private Bitmap picture;
	private final String[] nations = { "解码错", // 00
			"汉", // 01
			"蒙古", // 02
			"回", // 03
			"藏", // 04
			"维吾尔", // 05
			"苗", // 06
			"彝", // 07
			"壮", // 08
			"布依", // 09
			"朝鲜", // 10
			"满", // 11
			"侗", // 12
			"瑶", // 13
			"白", // 14
			"土家", // 15
			"哈尼", // 16
			"哈萨克", // 17
			"傣", // 18
			"黎", // 19
			"傈僳", // 20
			"佤", // 21
			"畲", // 22
			"高山", // 23
			"拉祜", // 24
			"水", // 25
			"东乡", // 26
			"纳西", // 27
			"景颇", // 28
			"柯尔克孜", // 29
			"土", // 30
			"达斡尔", // 31
			"仫佬", // 32
			"羌", // 33
			"布朗", // 34
			"撒拉", // 35
			"毛南", // 36
			"仡佬", // 37
			"锡伯", // 38
			"阿昌", // 39
			"普米", // 40
			"塔吉克", // 41
			"怒", // 42
			"乌孜别克", // 43
			"俄罗斯", // 44
			"鄂温克", // 45
			"德昴", // 46
			"保安", // 47
			"裕固", // 48
			"京", // 49
			"塔塔尔", // 50
			"独龙", // 51
			"鄂伦春", // 52
			"赫哲", // 53
			"门巴", // 54
			"珞巴", // 55
			"基诺", // 56
			"编码错", // 57
			"其他", // 97
			"外国血统" // 98
	};

	/**
	 * 获取姓名
	 * 
	 * @return
	 */
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * 获取性别
	 * 
	 * @return
	 */
	public String getSex() {
		return sex;
	}
	
	public void setSex(String sex) {
		this.sex = sex;
	}

	/**
	 * 获取国籍
	 * 
	 * @return
	 */
	public String getNation() {
		int nationcode = Integer.parseInt(nation);
		if (nationcode >= 1 && nationcode <= 56) {
			this.nation = nations[nationcode];
		} else if (nationcode == 97) {
			this.nation = "其他";
		} else if (nationcode == 98) {
			this.nation = "外国血统";
		} else {
			this.nation = "编码错";
		}
		return nation;
	}
	
	public void setNation(String nation) {
		this.nation = nation;
	}

	/**
	 * 获取出生日期
	 * 
	 * @return
	 */
	public String getBorn() {
		return born;
	}
	
	public void setBorn(String born) {
		this.born = born;
	}

	/**
	 * 获取地址
	 * 
	 * @return
	 */
	public String getAddress() {
		return address;
	}
	
	public void setAddress(String address) {
		this.address = address;
	}

	/**
	 * 获取身份证号
	 * 
	 * @return
	 */
	public String getIDCardNo() {
		return IDCardNo;
	}
	
	public void setIDCardNo(String IDCardNo) {
		this.IDCardNo = IDCardNo;
	}

	/**
	 * 获取颁发部门
	 * 
	 * @return
	 */
	public String getGrantDept() {
		return grantDept;
	}
	
	public void setGrantDept(String grantDept) {
		this.grantDept = grantDept;
	}

	/**
	 * 获取有效期开始日期
	 * 
	 * @return
	 */
	public String getValidFromDate() {
		return validFromDate;
	}
	
	public void setValidFromDate(String validFromDate) {
		this.validFromDate = validFromDate;
	}

	/**
	 * 获取有效期结束日期
	 * 
	 * @return
	 */
	public String getValidToDate() {
		return validToDate;
	}
	
	public void setValidToDate(String validToDate) {
		this.validToDate = validToDate;
	}

	/**
	 * 获取其他保留信息
	 * 
	 * @return
	 */
	public String getReserved() {
		return reserved;
	}
	
	public void setReserved(String reserved) {
		this.reserved = reserved;
	}

	/**
	 * 获取对加密数据解密后的身份证相片
	 * 
	 * @return
	 */
	public Bitmap getPicture() {

		return picture;
	}

	public void setPicture(Bitmap picture) {
		this.picture = picture;
	}

}
