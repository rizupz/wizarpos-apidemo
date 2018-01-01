
package com.cloudpos.apidemo.manager;

import java.util.List;

import android.content.Context;
import android.util.Log;

import com.cloudpos.apidemo.activity.R;
import com.cloudpos.apidemo.function.ActionReflect;
import com.cloudpos.mvc.base.ActionContainer;

public class ActionContainerImpl extends ActionContainer {
    private static final String TAG = "ActionContainerImpl";
    private String model;
    private Context context;

    public ActionContainerImpl(Context context, String model) {
        this.context = context;
        this.model = model;
    }

    @Override
    public void initActions() {
        Log.e(TAG, "Model = " + model);
        List<String> classItems = ActionReflect.getArraysXml(context, model);
        if (classItems == null) {
            classItems = ActionReflect.getArraysXml(context, "WIZARPOS");
        }
        for (int i = 0; i < classItems.size(); i++) {
            try {
                Class clazz = Class.forName(context.getResources().getString(
                        R.string.action_package_name)
                        + classItems.get(i));
                addAction(classItems.get(i), clazz, true);
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(TAG, "Can't find this action");
            }
        }
    }

}
