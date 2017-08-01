package com.xunbao.slot;

import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.os.Handler;
import android.util.Log;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class MyWebViewClient extends WebViewClient {

	private static final String TAG = "web";
	protected ProgressDialog mDialog;
	private Handler mHandler;
	public MyWebViewClient(Handler handler) {
		mHandler = handler;
	}

	/**
	 * true����ڵ�ǰҳ����ת
	 */
	@Override
	public boolean shouldOverrideUrlLoading(WebView view, String url) {
		System.out.println("url : " + url);
		return false;
	}

	/**
	 * ҳ����ؽ���ʱ����
	 */
	@Override
	public void onPageFinished(WebView view, String url) {
		super.onPageFinished(view, url);
		System.out.println("finish url : " + url);
		mHandler.sendEmptyMessage(1);
	}

	/**
	 * ҳ����ؿ�ʼʱ����
	 */
	@Override
	public void onPageStarted(WebView view, String url, Bitmap favicon) {
		super.onPageStarted(view, url, favicon);
		System.out.println("start url : " + url);
	}

	/**
	 * ����ı�����
	 */
	@Override
	public void onScaleChanged(WebView view, float oldScale, float newScale) {
		super.onScaleChanged(view, oldScale, newScale);
		Log.i(TAG, "onScaleChanged " + oldScale + ": " + newScale);
	}

	/**
	 * ����https����
	 */
	@Override
	public void onReceivedError(WebView view, int errorCode,
			String description, String failingUrl) {
		super.onReceivedError(view, errorCode, description, failingUrl);
		Log.i(TAG, "onReceivedError ");
	}
}
