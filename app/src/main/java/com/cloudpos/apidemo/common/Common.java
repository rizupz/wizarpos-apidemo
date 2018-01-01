
package com.cloudpos.apidemo.common;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.cloudpos.apidemo.activity.ConstantActivity;
import com.cloudpos.apidemo.activity.MainActivity;
import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.function.ActionCallbackImpl;

public class Common {

    public static byte[] createMasterKey(int length) {
        byte[] array = new byte[length];
        for (int i = 0; i < length; i++) {
            array[i] = (byte) 0x38;
        }
        return array;
    }

    public static int transferErrorCode(int errorCode) {
        int a = -errorCode;
        int b = a & 0x0000FF;
        return -b;
    }

    public static String getModel() {
        String model = "";
        model = SystemProperties.getSystemPropertie("ro.product.model").trim();
        model = model.replace(" ", "_");
        model = model.toUpperCase();
        Log.e("model", model);
        // model = model.substring(0, model.length() - 1).trim();//
        // 找到对应的设备类型WIZARPOS,WIZARPAD,...
        // 判断是否为手持
        if (model.equals("WIZARHAND_Q1") || model.equals("MSM8610") || model.equals("WIZARHAND_Q0")
                || model.equals("FARS72_W_KK") || model.equals("WIZARHAND_M0")) {
            ConstantActivity.isHand = true;
            if (model.equals("WIZARHAND_Q1") || model.equals("MSM8610")
                    || model.equals("WIZARHAND_Q0")) {
                ConstantActivity.isQ1 = true;
            }
        }
        return model;
    }

    public static String getMethodName() {
        StackTraceElement[] eles = Thread.currentThread().getStackTrace();
//        for (StackTraceElement stackTraceElement : eles) {
//            Log.e("stackTraceElement", stackTraceElement.getMethodName());
//        }
        return eles[5].getMethodName();
    }

    public static boolean getActionField(Context context, String className, String fieldName) {
        boolean isOpened = false;
        try {
            Class<?> clazz = Class.forName(context.getResources().getString(
                    R.string.action_package_name)
                    + className);
            Object object = clazz.getInterfaces();
            Field field = clazz.getField(fieldName);
            isOpened = field.getBoolean(object);
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
            isOpened = false;
        } catch (IllegalAccessException e) {
            e.printStackTrace();
            isOpened = false;
        } catch (Exception e) {
            e.printStackTrace();
            isOpened = false;
        }
        return isOpened;
    }
}
