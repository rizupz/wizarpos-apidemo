package com.cloudpos.apidemo.function.printer;

import android.graphics.Bitmap;

public class PrintTagForQ1 
{

    /**
     * 签购单标签
     */
    static public final class PurchaseBillTag 
    {
    	static public final Bitmap PURCHASE_BILL_TITLE = null;
    	
    	static public final String MERCHANT_COPY_TAG = "商户存根        MERCHANT COPY";
    	
    	static public final String SEPARATE = "-------------------------------";

    	static public final String MERCHANT_NAME_TAG = "商户名称（MERCHANT NAME）";

    	static public final String MERCHANT_NO_TAG = "商户编号（MERCHANT NO）";

    	static public final String TERMINAL_NO_AND_OPERATOR_TAG = "终端编号（TERMINAL NO）  操作员号（OPERATOR）";

//    	static public final String OPERATOR_TAG = "操作员号（OPERATOR）";

    	static public final String CARD_NO_TAG = "卡号（CARD NO）";

    	static public final String ISSUER_ACQUIRER_TAG = "发卡机构（ISSUER）    收单机构（ACQUIRER）";

//    	static public final String ACQUIRER_TAG = "收单机构（ACQUIRER）";

    	static public final String TRANS_TYPE_TAG = "交易类型（TRANS TYPE）";
    	
    	static public final String DATE_TIME_EXP_DATE_TAG = "日期/时间（DATE/TIME）    有效期（EXP DATE）";

//    	static public final String EXP_DATE_TAG = "有效期（EXP DATE）";
    	
    	static public final String REF_NO_BATCH_NO_TAG = "交易参考号（REF NO）    批次号（BATCH NO）";

//    	static public final String BATCH_NO_TAG = "批次号（BATCH NO）";

    	static public final String VOUCHER_NO_AUTH_NO_TAG = "凭证号（VOUCHER）    授权码（AUTH NO）";
    	
//    	static public final String AUTH_NO_TAG = "授权码（AUTH NO）";
    	
    	static public final String AMOUT_TAG = "金额（AMOUT）：";

    	static public final String REFERENCE_TAG = "备注/REFERENCE";

    	static public final String CARD_HOLDER_SIGNATURE_TAG = "持卡人签名(CARD HOLDER SIGNATURE)";

    	static public final String C_AGREE_TRADE_TAG = "本人确认以上交易，同意将其记入本卡账户";
    	static public final String E_AGREE_TRADE_TAG = "I ACKNOWLEDGE SATISFACTORY RECEIPT OF RELATIVE GOODS/SERVICE";
    }

}
