package com.cloudpos.apidemo.function.printer;

import java.io.Serializable;

import com.cloudpos.apidemo.util.StringUtility;

public class PurchaseBillForQ1Entity implements Serializable {
	private static final long serialVersionUID = 1L;

//	private String merchantCopy;			//商户存根

	private String merchantName;		//商户名称

	private String merchantNo;				//商户编号

	private String terminalNoAndOperator;				//终端编号, 操作员号

//	private String operator;				//操作员号

	private String cardNo;						//卡号		

	private String issuerAndAcquirer;							//发卡机构,可空, 收单机构,可空

//	private String acquirer;						//收单机构,可空

	private String transType;					//交易类型
	
	public String getIssuerAndAcquirer() {
		return issuerAndAcquirer;
	}

	public void setIssuerAndAcquirer(String issuerAndAcquirer) {
		this.issuerAndAcquirer = issuerAndAcquirer;
	}

	public String getDataTimeAndExpDate() {
		return dataTimeAndExpDate;
	}

	public void setDataTimeAndExpDate(String dataTimeAndExpDate) {
		this.dataTimeAndExpDate = dataTimeAndExpDate;
	}

	public String getRefNoAndBatchNo() {
		return refNoAndBatchNo;
	}

	public void setRefNoAndBatchNo(String refNoAndBatchNo) {
		this.refNoAndBatchNo = refNoAndBatchNo;
	}

	public String getVoucherAndAuthNo() {
		return voucherAndAuthNo;
	}

	public void setVoucherAndAuthNo(String voucherAndAuthNo) {
		this.voucherAndAuthNo = voucherAndAuthNo;
	}

	private String dataTimeAndExpDate;					//日期/时间 有效期

//	private String expDate;						//有效期,可空
	
	private String refNoAndBatchNo;							//交易参考号 批次号

//	private String batchNo;						//批次号
 
	private String voucherAndAuthNo;					//凭证号 授权码

//	private String authNo;						//授权码,可空

	private String amout;							//金额
	
	private String reference;					//备注,可空

	public boolean checkValidity() {

		if (StringUtility.isEmpty(merchantName)
				|| StringUtility.isEmpty(merchantNo)
				|| StringUtility.isEmpty(terminalNoAndOperator)
				|| StringUtility.isEmpty(cardNo)
				|| StringUtility.isEmpty(transType)
				|| StringUtility.isEmpty(dataTimeAndExpDate)
				|| StringUtility.isEmpty(refNoAndBatchNo) 
				|| StringUtility.isEmpty(voucherAndAuthNo)
				|| StringUtility.isEmpty(amout)) {
			return false;
		} else
			return true;
	}

//	public String getMerchantCopy() {
//		return merchantCopy;
//	}
//
//	public void setMerchantCopy(String merchantCopy) {
//		this.merchantCopy = merchantCopy;
//	}

	public String getCardNo() {
		return cardNo;
	}

	public void setCardNo(String cardNo) {
		this.cardNo = cardNo;
	}
	
	public String getTransType() {
		return transType;
	}

	public void setTransType(String transType) {
		this.transType = transType;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

	public String getMerchantName() {
		return merchantName;
	}

	public void setMerchantName(String merchantName) {
		this.merchantName = merchantName;
	}

	public String getMerchantNo() {
		return merchantNo;
	}

	public void setMerchantNo(String merchantNo) {
		this.merchantNo = merchantNo;
	}

	public String getTerminalNoAndOperator() {
		return terminalNoAndOperator;
	}

	public void setTerminalNoAndOperator(String terminalNoAndOperator) {
		this.terminalNoAndOperator = terminalNoAndOperator;
	}

	public String getAmout() {
		return amout;
	}

	public void setAmout(String amout) {
		this.amout = amout;
	}

	public String getReference() {
		return reference;
	}

	public void setReference(String reference) {
		this.reference = reference;
	}
}
