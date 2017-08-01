package com.xq5.xq5gdy.UPPay;

import org.cocos2dx.cpp.AppActivity;

import aj.ajention.tools.AJLog;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Handler.Callback;
import android.os.Message;

import com.unionpay.UPPayAssistEx;
import com.xq5.xq5gdy.R;

public class UPPay implements Callback {

	public static final String LOG_TAG = "test";
	private AppActivity mAppActivity = null;

	private static final int ying_lian_Pay_Message = 10;
	private ProgressDialog mLoadingDialog = null;

	public static final int PLUGIN_VALID = 0;
	public static final int PLUGIN_NOT_INSTALLED = -1;
	public static final int PLUGIN_NEED_UPGRADE = 2;
	/*****************************************************************
	 * mMode参数解释： "00" - 启动银联正式环境 "01" - 连接银联测试环境
	 *****************************************************************/
	private final String mMode = "00";

	private static String s_GetTn = "";

	private Handler mHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			if (msg.what == ying_lian_Pay_Message) {
				AJLog.e("test", "getIn" + s_GetTn);
				doStartUnionPayPlugin(mAppActivity, s_GetTn, mMode);
			}
		}
	};

	public UPPay(AppActivity appActivity) {
		mAppActivity = appActivity;
	}

	public void setUPPay(String kTn){
		s_GetTn = kTn;
    	AJLog.e("test","setUPPay = " + kTn);
	    Message msg = mHandler.obtainMessage();
		msg.what = ying_lian_Pay_Message;
	    mHandler.sendMessage(msg);
	}
	
	@Override
	public boolean handleMessage(Message msg) {
		AJLog.e(LOG_TAG, " " + "" + msg.obj);
		if (mLoadingDialog.isShowing()) {
			mLoadingDialog.dismiss();
		}

		String tn = "";
		if (msg.obj == null || ((String) msg.obj).length() == 0) {
			AlertDialog.Builder builder = new AlertDialog.Builder(mAppActivity);
			builder.setTitle(mAppActivity.getString(R.string.app_error));
			builder.setMessage(mAppActivity.getString(R.string.app_newwork_error));
			builder.setNegativeButton(mAppActivity.getString(R.string.app_ok),
					new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							dialog.dismiss();
						}
					});
			builder.create().show();
		} else {
			tn = (String) msg.obj;
			/*************************************************
			 * 步骤2：通过银联工具类启动支付插件
			 ************************************************/
			doStartUnionPayPlugin(mAppActivity, tn, mMode);
		}

		return false;
	}
	public void doStartUnionPayPlugin(Activity activity, String tn, String mode) {
		// mMode参数解释：
		// 0 - 启动银联正式环境
		// 1 - 连接银联测试环境
		int ret = UPPayAssistEx.startPay(mAppActivity, null, null, tn, mode);
		if (ret == PLUGIN_NEED_UPGRADE || ret == PLUGIN_NOT_INSTALLED) {
			// 需要重新安装控件
			AJLog.e(LOG_TAG, " plugin not found or need upgrade!!!");

			AlertDialog.Builder builder = new AlertDialog.Builder(mAppActivity);
			builder.setTitle(mAppActivity.getString(R.string.app_tip));
			builder.setMessage(mAppActivity.getString(R.string.install_uppay));

			builder.setNegativeButton(mAppActivity.getString(R.string.app_ok),
					new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							UPPayAssistEx.installUPPayPlugin(mAppActivity);
							dialog.dismiss();
						}
					});

			builder.setPositiveButton(mAppActivity.getString(R.string.app_cancel),
					new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							dialog.dismiss();
						}
					});
			builder.create().show();

		}
		//AJLog.e(LOG_TAG, "" + ret);
	}
//	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
//		/*************************************************
//		 * 步骤3：处理银联手机支付控件返回的支付结果
//		 ************************************************/
//		if (data == null) {
//			return;
//		}
//
//		String msg = "";
//		/*
//		 * 支付控件返回字符串:success、fail、cancel 分别代表支付成功，支付失败，支付取消
//		 */
//		String str = data.getExtras().getString("pay_result");
//		if (str.equalsIgnoreCase("success")) {
//			msg = "支付成功！";
//		} else if (str.equalsIgnoreCase("fail")) {
//			msg = "支付失败！";
//		} else if (str.equalsIgnoreCase("cancel")) {
//			msg = "用户取消了支付";
//		}
//
//		AlertDialog.Builder builder = new AlertDialog.Builder(mAppActivity);
//		builder.setTitle("支付结果通知");
//		builder.setMessage(msg);
//		builder.setInverseBackgroundForced(true);
//		// builder.setCustomTitle();
//		builder.setNegativeButton("确定", new DialogInterface.OnClickListener() {
//			@Override
//			public void onClick(DialogInterface dialog, int which) {
//				dialog.dismiss();
//			}
//		});
//		builder.create().show();
//	}

//	@Override
//	public void run() {
//		String tn = null;
//		InputStream is;
//		try {
//
//			String url = TN_URL_01;
//
//			URL myURL = new URL(url);
//			URLConnection ucon = myURL.openConnection();
//			ucon.setConnectTimeout(120000);
//			is = ucon.getInputStream();
//			int i = -1;
//			ByteArrayOutputStream baos = new ByteArrayOutputStream();
//			while ((i = is.read()) != -1) {
//				baos.write(i);
//			}
//
//			tn = baos.toString();
//			is.close();
//			baos.close();
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//
//		Message msg = mHandler.obtainMessage();
//		msg.obj = tn;
//		mHandler.sendMessage(msg);
//	}

//	int startpay(Activity act, String tn, int serverIdentifier) {
//		return 0;
//	}



}
