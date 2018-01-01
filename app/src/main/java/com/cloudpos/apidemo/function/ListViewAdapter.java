package com.cloudpos.apidemo.function;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.cloudpos.apidemo.activity.R;

public class ListViewAdapter extends BaseAdapter {
	private List<String> arryText;
	private List<String> arryTag;
	private LayoutInflater inflater;
	TextView txtListBtn;

	public ListViewAdapter(List<String> arryText, List<String> arryTag, Activity mainActivity) {
		this.arryText = arryText;
		this.arryTag = arryTag;
		this.inflater = LayoutInflater.from(mainActivity);
	}

	@Override
	public int getCount() {
		if (arryText == null) {
			return 0;
		}
		return arryText.size();
	}

	@Override
	public Object getItem(int position) {
		if (arryText == null) {
			return null;
		}
		return arryText.get(position);
	}

	@Override
	public long getItemId(int position) {
		if (arryText == null) {
			return 0;
		}
		return position;
	}

	@Override
	public View getView(int position, View contentview, ViewGroup arg2) {
		View view = contentview;
		if (contentview == null) {
			view = inflater.inflate(R.layout.item_main, null);
			// holder.listbtn = (TextView) view.findViewById(R.id.listview_btn);
			// 这个错误很无语，难道只有花了很多时间改得错误才能记住吗？
		}
		txtListBtn = (TextView) view.findViewById(R.id.lvw_button);

		txtListBtn.setTag(arryTag.get(position)); // 设置主菜单的button的Tag
		txtListBtn.setText(arryText.get(position));
		return view;
	}

	public static class holder {
		public static TextView listbtn;
	}

	public void refreshData(List<String> arryText, List<String> arryTag) {
		this.arryText.clear();
		this.arryText = arryText;
		this.arryTag.clear();
		this.arryTag = arryTag;

		notifyDataSetChanged();
	}

	public List<String> getListTag() {
		List<String> lsTag = new ArrayList<String>();
		for (int i = 0; i < arryText.size(); i++) {
			lsTag.add(arryTag.get(i));
		}
		return lsTag;
	}

}
