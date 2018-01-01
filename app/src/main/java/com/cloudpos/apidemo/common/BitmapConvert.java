package com.cloudpos.apidemo.common;

import java.nio.ByteBuffer;

import android.graphics.Bitmap;

public class BitmapConvert {
	public static byte[] Bitmap2Bytes(Bitmap bm) {
		int w = bm.getWidth();
		int h = bm.getHeight();

		// if (w > 480 || h > 800) {
		// bm = comp(bm);
		// w = bm.getWidth();
		// h = bm.getHeight();
		// }

		ByteBuffer dst = ByteBuffer.allocate(w * h * 4);
		bm.copyPixelsToBuffer(dst);
		dst.flip();
		byte[] pData = dst.array();
		return pData;
	}
}
