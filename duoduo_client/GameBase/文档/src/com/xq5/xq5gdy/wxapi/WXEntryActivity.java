package com.xq5.xq5gdy.wxapi;

import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.cocos2dx.cpp.AppActivity;
import org.json.JSONObject;

import aj.ajention.tools.AJLog;
import aj.ajention.tools.MD5;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXTextObject;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.xq5.xq5gdy.R;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler {

	public static final String APP_ID = "wx58e2d350e978208d";
	/** �̼���Ƹ�ͨ������̼�id */
	public static final String PARTNER_ID = "1221848401";

	// ��� ʱ���
	private long timeStamp;
	private String nonceStr, packageValue;

	// IWXAPI �ǵ���app��΢��ͨ�ŵ�openapi�ӿ�
	private IWXAPI api;

	public static String ResAppData = "WxResAppData";
	public static String ResWebpage = "WxResWebpage";
	public static String ResWXPay = "WxResWxPay";
	public static String tag = "test";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		AJLog.v(tag, "WX entry onCreate");
		super.onCreate(savedInstanceState);
		api = WXAPIFactory.createWXAPI(this, APP_ID);
		api.registerApp(APP_ID);
		api.handleIntent(getIntent(), this);

		if (!api.isWXAppInstalled()) {
			Intent intent = new Intent();
			intent.putExtra("WXisInstalled", "false");
			setResult(RESULT_OK, intent);
			finish();
			return;
		}
		Intent intent = getIntent();

		if (intent.hasExtra(ResAppData)) {
			String data = intent.getStringExtra(ResAppData);
			sendAppData(data);
		}
		if (intent.hasExtra(ResWebpage)) {
			String data = intent.getStringExtra(ResWebpage);
			sendWebpage(data);
			// sendText(data);
		}
		if (intent.hasExtra(ResWXPay)) {
			String data = intent.getStringExtra(ResWXPay);
			sendPayReq(data);
		}
		// sendWebpage();
		finish();
	}

	@Override
	protected void onResume() {
		AJLog.d(tag, "WX onResume");
		super.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();
		AJLog.d(tag, "WX onPause");
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		AJLog.d(tag, "WX keyCode =" + keyCode);
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public void onDestroy() {
		AJLog.d(tag, "WX onDestroy");
		super.onDestroy();
	}

	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis())
				: type + System.currentTimeMillis();
	}

	public boolean isWXAppInstalled() {
		// IWXAPI.is
		return false;
	}

	public void sendWebpage(String msgInfo) {

		AJLog.d("test", "sendWebpage---115");
		// WXWebpageObject webpage = new WXWebpageObject();
		// webpage.webpageUrl = "http://www.xq5.com";
		// WXMediaMessage msg = new WXMediaMessage(webpage);
		//
		// msg.title = getString(R.string.app_name);
		// msg.description = msgInfo;//getString(msgInfo);
		// Bitmap thumb = BitmapFactory.decodeResource(this.getResources(),
		// R.drawable.icon);
		// msg.thumbData = Util.bmpToByteArray(thumb, true);
		//
		// SendMessageToWX.Req req = new SendMessageToWX.Req();
		// req.transaction = buildTransaction("webpage");
		// Log.e("test","req.transaction ="+req.transaction);
		// req.message = msg;
		// req.scene = SendMessageToWX.Req.WXSceneTimeline;
		// //req.openId = getOpenId();
		// if(req.checkArgs())
		// Log.e("test","req.checkArgs =ok");
		// if(webpage.checkArgs())
		// Log.e("test","webpage.checkArgs =ok");
		// api.sendReq(req);

		AJLog.d("test", "sendWebpage1");
		WXWebpageObject webpage = new WXWebpageObject();
		webpage.webpageUrl = "http://www.xq5.com";
		// webpage.
		WXMediaMessage msg = new WXMediaMessage(webpage);

		msg.title = msgInfo;
		msg.description = msgInfo;
		msg.messageAction = "messageAction";
		msg.messageExt ="messageExt";
		
		msg.mediaTagName="webpage";
		Bitmap thumb = BitmapFactory.decodeResource(this.getResources(),
				R.drawable.share);
		msg.thumbData = Util.bmpToByteArray(thumb, true);
		AJLog.d(tag, "thumbData= " + msg.thumbData.length);
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("webpage");
		req.message = msg;
		
		req.scene = SendMessageToWX.Req.WXSceneTimeline;
		api.sendReq(req);
		AJLog.d("test", "sendWebpage2");
		if (req.checkArgs()) {

			AJLog.e("test", "req.checkArgs =ok");
		} else {
			AJLog.d("test", "req=" + req.toString());
		}
		if (webpage.checkArgs())
			AJLog.e("test", "webpage.checkArgs =ok");
	}

	public void sendText(String msgInfo) {
		WXTextObject textObj = new WXTextObject();

		String text = msgInfo;// getString(R.string.send_web_info_str);
		textObj.text = text;

		// ��WXTextObject�����ʼ��һ��WXMediaMessage����
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		// �����ı����͵���Ϣʱ��title�ֶβ�������
		// msg.title = "Will be ignored";
		msg.description = text;

		// ����һ��Req
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("text"); // transaction�ֶ�����Ψһ��ʶһ������
		req.message = msg;
		req.scene = SendMessageToWX.Req.WXSceneTimeline;

		// ����api�ӿڷ�����ݵ�΢��
		api.sendReq(req);
	}

	public void sendAppData(String msgInfo) {
		final WXAppExtendObject appdata = new WXAppExtendObject();
		appdata.extInfo = msgInfo;// getString(R.string.send_app_info_str);

		final WXMediaMessage msg = new WXMediaMessage();
		msg.title = getString(R.string.app_name);
		msg.description = msgInfo;// getString(R.string.send_app_info_str);

		msg.mediaObject = appdata;

		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("appdata");
		req.message = msg;
		req.scene = SendMessageToWX.Req.WXSceneSession;
		api.sendReq(req);
		if (req.checkArgs())
			AJLog.e("test", "sendAppData req.checkArgs =ok");
	}

	public void sendPayReq(String data) {
		Log.e("test", "sendPayReq =" + data);
		String APP_Key = "";
		String prepayId = "";
		packageValue = "";
		try {
			JSONObject json = new JSONObject(data);
			if (json.has("prepay_id")) { // success case
				prepayId = json.getString("prepay_id");
			}
			if (json.has("app_key")) {
				APP_Key = json.getString("app_key");
			}
			if (json.has("package")) {
				packageValue = json.getString("package");
			}

			// Log.e("test","prepayId= "+prepayId);
			// Log.e("test","APP_Key= "+APP_Key);
			// Log.e("test","packageValue= "+packageValue);

		} catch (Exception e) {
			// localRetCode = LocalRetCode.ERR_JSON;
		}

		PayReq req = new PayReq();
		req.appId = APP_ID;
		req.partnerId = PARTNER_ID;
		/** �̻�id */
		req.prepayId = prepayId;
		/** Ԥ֧������ */
		req.nonceStr = genNonceStr();
		req.timeStamp = String.valueOf(genTimeStamp());

		req.packageValue = "Sign=" + packageValue;

		List<NameValuePair> signParams = new LinkedList<NameValuePair>();
		signParams.add(new BasicNameValuePair("appid", req.appId));
		signParams.add(new BasicNameValuePair("appkey", APP_Key));
		signParams.add(new BasicNameValuePair("noncestr", req.nonceStr));
		signParams.add(new BasicNameValuePair("package", req.packageValue));
		signParams.add(new BasicNameValuePair("partnerid", req.partnerId));
		signParams.add(new BasicNameValuePair("prepayid", req.prepayId));
		signParams.add(new BasicNameValuePair("timestamp", req.timeStamp));
		req.sign = genSign(signParams);
		Log.e("test", "req = " + req.toString());
		// ��֧��֮ǰ�����Ӧ��û��ע�ᵽ΢�ţ�Ӧ���ȵ���IWXMsg.registerApp��Ӧ��ע�ᵽ΢��
		if (api.sendReq(req))
			AJLog.e("test", "sendReqOk");

	}

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);

		setIntent(intent);
		api.handleIntent(intent, this);
	}

	// ΢�ŷ������󵽵���Ӧ��ʱ����ص����÷���
	@Override
	public void onReq(BaseReq req) {
		AJLog.e("test", "onReq");
		AJLog.e("test", "onReq req" + req);
		AJLog.e("test", "onReq req" + req.getType());
		switch (req.getType()) {
		case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX:
			// goToGetMsg();

			break;
		case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX:
			// goToShowMsg((ShowMessageFromWX.Req) req);
			Intent intent = new Intent(WXEntryActivity.this, AppActivity.class);
			startActivity(intent);
			break;
		default:
			break;
		}
		finish();
	}

	// ����Ӧ�÷��͵�΢�ŵ�����������Ӧ����ص����÷���
	@Override
	public void onResp(BaseResp resp) {
		int result = 0;
		AJLog.e("test", "onResp");
		AJLog.e("test", "onResp =" + resp);
		AJLog.e("test", "onResp =" + resp.errCode);
		switch (resp.errCode) {
		case BaseResp.ErrCode.ERR_OK: {
			// Log.e("test", "ERR_OK");
			result = R.string.errcode_success;
			AppActivity _temp = (AppActivity) AppActivity.getApkInstance();
			_temp.wixinApiResp(resp.errCode);
		}
			break;
		case BaseResp.ErrCode.ERR_USER_CANCEL:
			result = R.string.errcode_cancel;
			break;
		case BaseResp.ErrCode.ERR_AUTH_DENIED:
			result = R.string.errcode_deny;
			break;
		default:
			result = R.string.errcode_unknown;
			break;
		}
		Toast.makeText(this, result, Toast.LENGTH_LONG).show();

		// Intent intent = new Intent();
		// intent.putExtra("error_code", result);
		// setResult(RESULT_OK, intent);
		finish();
	}

	private String genNonceStr() {
		Random random = new Random();
		return MD5.getMessageDigest(String.valueOf(random.nextInt(10000))
				.getBytes());
	}

	private long genTimeStamp() {
		return System.currentTimeMillis() / 1000;
	}

	/**
	 * ע�⣺�̻�ϵͳ�ڲ��Ķ�����,32���ַ��ڡ��ɰ���ĸ,ȷ�����̻�ϵͳΨһ
	 */
	private String genOutTradNo() {
		Random random = new Random();
		return MD5.getMessageDigest(String.valueOf(random.nextInt(10000))
				.getBytes());
	}

	private String genSign(List<NameValuePair> params) {
		StringBuilder sb = new StringBuilder();

		int i = 0;
		for (; i < params.size() - 1; i++) {
			sb.append(params.get(i).getName());
			sb.append('=');
			sb.append(params.get(i).getValue());
			sb.append('&');
		}
		sb.append(params.get(i).getName());
		sb.append('=');
		sb.append(params.get(i).getValue());

		String sha1 = Util.sha1(sb.toString());
		// Log.d("test", "genSign, sha1 = " + sha1);
		return sha1;
	}

	// public native void wixinApiResp(final int code);
}