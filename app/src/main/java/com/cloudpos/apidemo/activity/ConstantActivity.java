package com.cloudpos.apidemo.activity;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;

import com.cloudpos.apidemo.common.Enums.StateLog;
import com.cloudpos.apidemo.function.ListViewAdapter;
import com.cloudpos.mvc.base.ActionCallback;

public abstract class ConstantActivity extends Activity {

	protected static final String TAG = "MainActivity";
	public static Handler handler;
	public static ActionCallback callback;
	protected ListViewAdapter adapter;
	
	protected Map<String, Object> param;

	// 控件
	protected LinearLayout layoutJianjie; // 控制简介控件的显示和隐藏
	protected TextView txtJianjie;// 简介
	protected Button btnClean;
	protected Button btnExit;
	public static TextView txtResult;// log output log helper使用
	protected ListView lvwMain;
	public static TextView txtVersion;

	public static Activity host;
	protected Context context;
	
	// 设置点击事件的控件ID
	protected int[] btnWidget;
	protected boolean isMain = true;

	public static String mainMenu = "Main";

	/**
	 * ListView当前移动到的位置
	 */
	public static int position;

	// 分别存放strings.xml中和arrays.xml中获取的信息
	protected List<String> arryTag;
	protected List<String> arryText;

	// 是不是手持设备
	public static boolean isHand = false;
	// 是不是Q1
	public static boolean isQ1 = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		initView();

		
	}

	private void initView() {
		layoutJianjie = (LinearLayout) findViewById(R.id.layout_jianjie);
		txtJianjie = (TextView) findViewById(R.id.txt_jianjie);
		txtVersion = (TextView) findViewById(R.id.txt_version);
		txtResult = (TextView) findViewById(R.id.txt_result); // 定义txtResult
		txtResult.setMovementMethod(ScrollingMovementMethod.getInstance());
		lvwMain = (ListView) findViewById(R.id.lvw_main);
		btnExit = (Button) findViewById(R.id.btn_exit);
		btnClean = (Button) findViewById(R.id.btn_clean);
	}

	

	public static void writeLog(String obj) {
		Message msg = new Message();
		msg.what = StateLog.LOG;
		msg.obj = obj + "\n";
		handler.sendMessage(msg);
	}

}
