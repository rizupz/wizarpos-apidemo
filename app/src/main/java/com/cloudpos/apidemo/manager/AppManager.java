
package com.cloudpos.apidemo.manager;

import android.app.Application;

import com.cloudpos.mvc.base.ActionManager;

public class AppManager extends Application {
    
    /** 外接设备的类型
     */
    public static String model = "";

    @Override
    public void onCreate() {
        super.onCreate();

        // Common.getModel();

        model = "WIZARPOS";

        ActionManager.initActionContainer(new ActionContainerImpl(this, model));
    }

}
