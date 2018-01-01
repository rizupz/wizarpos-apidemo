
package com.cloudpos.apidemo.action;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.cloudpos.apidemo.activity.MainActivity;
import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.function.ActionCallbackImpl;
import com.cloudpos.apidemo.function.idcard.IDCard;
import com.cloudpos.jniinterface.IDCardInterface;
import com.cloudpos.jniinterface.IDCardProperty;
import com.synjones.bluetooth.DecodeWlt;

public class IDCardAction extends Activity implements OnClickListener {

    private static String TAG = "IDCardAction";
    private Activity host;
    private ActionCallbackImpl callback;
    TextView txtName;
    TextView txtSex;
    TextView txtNation;
    TextView txtBorn;
    TextView txtAddress;
    TextView txtGrantDept;
    TextView txtIDCardNo;
    TextView txtValidFromDate;
    TextView txtValidToDate;
    ImageView imgPicture;

    private Button btnBegin;
    private Button btnEnd;

    private String wltPath;
    private String bmpPath;

    private IDCardProperty property;

    private static boolean isRun = false;
    private HandlerImpl handler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_idcard);
        initView();
        initParameters();
    }

    private void initParameters() {
        this.callback = (ActionCallbackImpl) MainActivity.callback;
        this.host = this;
        this.handler = new HandlerImpl();
        // decrypt photo from wlt to bmp and save it
        wltPath = getFileStreamPath("photo.wlt").getAbsolutePath();
        bmpPath = getFileStreamPath("photo.bmp").getAbsolutePath();

        property = new IDCardProperty();
    }

    class HandlerImpl extends Handler {

        @Override
        public void handleMessage(Message msg) {
            IDCard card = new IDCard();
            String result = null;
            try {
                // name
                result = new String(property.strName, "UTF-16LE").substring(0,
                        property.strName.length / 2).trim();
                card.setName(result);
                txtName.setText(result);
                // sex
                result = new String(property.strSex, "UTF-16LE").substring(0,
                        property.strSex.length / 2).trim();
                card.setSex(result);
                txtSex.setText(result);
                // nation
                result = new String(property.strNation, "UTF-16LE").substring(0,
                        property.strNation.length / 2).trim();
                card.setNation(result);
                txtNation.setText(result);
                // born
                result = new String(property.strBorn, "UTF-16LE").substring(0,
                        property.strBorn.length / 2).trim();
                card.setBorn(result);
                txtBorn.setText(result);
                // address
                result = new String(property.strAddress, "UTF-16LE").substring(0,
                        property.strAddress.length / 2).trim();
                card.setAddress(result);
                txtAddress.setText(result);
                // grantDept
                result = new String(property.strGrantDept, "UTF-16LE").substring(0,
                        property.strGrantDept.length / 2).trim();
                card.setGrantDept(result);
                txtGrantDept.setText(result);
                // IDNo
                result = new String(property.strIDCardNo, "UTF-16LE").substring(0,
                        property.strIDCardNo.length / 2).trim();
                card.setIDCardNo(result);
                txtIDCardNo.setText(result);
                // ValidFromDate
                result = new String(property.strUserLifeBegin, "UTF-16LE").substring(0,
                        property.strUserLifeBegin.length / 2).trim();
                card.setValidFromDate(result);
                txtValidFromDate.setText(result);
                // ValidToDate
                result = new String(property.strUserLifeEnd, "UTF-16LE").substring(0,
                        property.strUserLifeEnd.length / 2).trim();
                card.setValidToDate(result);
                txtValidToDate.setText(result);
                // picture
                try {
                    File wltFile = new File(wltPath);
                    FileOutputStream fos = new FileOutputStream(wltFile);
                    fos.write(property.strPicture);
                    fos.close();
                    Log.e(TAG, "wltPath = " + wltPath);
                    Log.e(TAG, "bmpPath = " + bmpPath);
                    int dwResult = DecodeWlt.Wlt2Bmp(wltPath, bmpPath);
                    Log.e(TAG, "DecodeWlt.Wlt2Bmp result = " + dwResult);
                } catch (IOException ioe) {
                    ioe.printStackTrace();
                }
                Bitmap bitmap = BitmapFactory.decodeFile(bmpPath);
                if (bitmap != null) {
                    imgPicture.setImageBitmap(bitmap);
                }
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void initView() {
        txtName = (TextView) findViewById(R.id.txt_name_content);
        txtSex = (TextView) findViewById(R.id.txt_sex_content);
        txtNation = (TextView) findViewById(R.id.txt_nation_content);
        txtBorn = (TextView) findViewById(R.id.txt_born_content);
        txtAddress = (TextView) findViewById(R.id.txt_address_content);
        txtGrantDept = (TextView) findViewById(R.id.txt_grantdept_content);
        txtIDCardNo = (TextView) findViewById(R.id.txt_id_no_content);
        txtValidFromDate = (TextView) findViewById(R.id.txt_userlifebegin_content);
        txtValidToDate = (TextView) findViewById(R.id.txt_userlifeend_content);

        txtName.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtSex.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtNation.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtBorn.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtAddress.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtGrantDept.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtIDCardNo.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtValidFromDate.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
        txtValidToDate.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);

        imgPicture = (ImageView) findViewById(R.id.img_picture);

        btnBegin = (Button) findViewById(R.id.btn_begin);
        btnEnd = (Button) findViewById(R.id.btn_end);
        btnBegin.setOnClickListener(this);
        btnEnd.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_begin:
                ReadIDCardThread thread = new ReadIDCardThread();
                thread.start();
                break;
            case R.id.btn_end:
                isRun = false;
                IDCardAction.this.finish();
                break;
            default:
                break;
        }
    }

    class ReadIDCardThread extends Thread {

        @Override
        public void run() {
            isRun = true;
            open();
            if (!isOpened) {
                return;
            }
            while (isRun) {
                searchTarget();
                int result = getInformation(property);
                if (result < 0) {
                    // 获取信息出错
                } else {
                    Message msg = new Message();
                    handler.sendMessage(msg);
                    isRun = false;
                }
            }
            close();
        }

    }

    private static boolean isOpened;

    private void open() {
        if (isOpened) {
            callback.sendFailedMsg(getResources().getString(R.string.device_opened));
        } else {
            try {
                int result = IDCardInterface.open();
                if (result < 0) {
                    callback.sendFailedMsg(getResources().getString(R.string.operation_with_error)
                            + result);
                } else {
                    isOpened = true;
                    callback.sendSuccessMsg(getResources().getString(R.string.operation_successful));
                }
            } catch (Throwable e) {
                callback.sendFailedMsg(getResources().getString(R.string.operation_failed));
            }
        }
    }

    private void close() {
        if (!isOpened) {
            callback.sendFailedMsg(getResources().getString(R.string.device_not_open));
        } else {
            try {
                int result = IDCardInterface.close();
                isOpened = false;
                if (result < 0) {
                    callback.sendFailedMsg(getResources().getString(R.string.operation_with_error)
                            + result);
                } else {
                    callback.sendSuccessMsg(getResources().getString(R.string.operation_successful));
                }
            } catch (Throwable e) {
                callback.sendFailedMsg(getResources().getString(R.string.operation_failed));
            }
        }
    }

    private int searchTarget() {
        int result = checkOpenAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = IDCardInterface.searchTarget();
                return result;
            }
        });
        return result;
    }

    private int getInformation(final IDCardProperty property) {
        int result = checkOpenAndGetData(new DataAction() {

            @Override
            public int getResult() {
                int result = IDCardInterface.getInformation(property);
                return result;
            }
        });
        return result;
    }

    int checkOpenAndGetData(DataAction action) {
        int result = 0;
        try {
            result = action.getResult();
            if (result < 0) {
                callback.sendFailedMsg(host.getResources().getString(R.string.operation_with_error)
                        + result);
            }
        } catch (Throwable e) {
            e.printStackTrace();
            callback.sendFailedMsg(host.getResources().getString(R.string.operation_failed));
        }
        return result;
    }

}
