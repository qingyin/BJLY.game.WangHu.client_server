package com.pkqp.game.duoduo.wxapi;


import java.io.File;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import org.cocos2dx.cpp.Native;
import org.cocos2dx.cpp.Util;
import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.ShowMessageFromWX;
import com.tencent.mm.sdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.sdk.modelmsg.WXImageObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXTextObject;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler{
	
	public static String Tag = "WXEntryActivity";
	private static final int TIMELINE_SUPPORTED_VERSION = 0x21020001;
	private static WXEntryActivity sContext = null;
	public static boolean bLogonWX = false;
	private static final int THUMB_SIZE = 150;

    public static final String APP_ID = "wxe8ef8836c0b659fe";
    public static final String AppSecret = "64aa79f62de73c04ea929592e23365c5";
	
    private IWXAPI 			  api;
    
	private static final int SceneSession = 0;//閸掑棔闊╅崚棰佺窗鐠囷拷
	private static final int SceneTimeline = 1; //閸掑棔闊╅崚鐗堟箙閸欏婀�
	
	public static String ReqWxLogin = "ReqWxLogin";
	public static String ReqWxShareImg = "ReqWxShareImg";
	public static String ReqWxShareTxt = "ReqWxShareTxt";
	public static String ReqWxShareUrl = "ReqWxShareUrl";
	public static String ReqWXPay = "ReqWXPay";
	
	
	
	  @Override
	 public void onCreate(Bundle savedInstanceState)
	 {
	        super.onCreate(savedInstanceState);
	        sContext = this;
    	    Log.d(Tag,"onCreate");
	        
			Intent intent = getIntent();

	    	api = WXAPIFactory.createWXAPI(this,APP_ID, true);
		    api.registerApp(APP_ID);
	        api.handleIntent(intent, this);
	        
			if (intent.hasExtra(ReqWxLogin)) 
			{
	        	reqLogin();
			}
			else if(intent.hasExtra(ReqWxShareImg))
			{
				String ImgPath = intent.getStringExtra("ImgPath");
				int nType = intent.getIntExtra("ShareType",0);
				 reqShareImg(ImgPath,nType);
			}
			else if(intent.hasExtra(ReqWxShareTxt))
			{
				String ShareText = intent.getStringExtra("ShareText");
				int nType = intent.getIntExtra("ShareType",0);
				reqShareTxt(ShareText,nType);
			}
			else if(intent.hasExtra(ReqWxShareUrl))
			{
				String ShareUrl = intent.getStringExtra("ShareUrl");
				String ShareTitle = intent.getStringExtra("ShareTitle");
				String ShareDesc = intent.getStringExtra("ShareDesc");
				int nType = intent.getIntExtra("ShareType",0);
				reqShareUrl(ShareUrl,ShareTitle,ShareDesc,nType);
			}
			finish();
	 }
	  
	  @Override
	  protected void onNewIntent(Intent intent) 
	  {
			super.onNewIntent(intent);
			
			setIntent(intent);
	        api.handleIntent(intent, this);
	  }
 
    public void reqLogin()
    {
    	SendAuth.Req req = new SendAuth.Req();
    	req.scope = "snsapi_userinfo";
    	req.state = "carjob_wx_login";
    	api.sendReq(req);
	    Log.d(Tag,"reqLogin");
    } 
    public void reqShareImg(String ImgPath,int nType)
    {
		File file = new File(ImgPath);
		if (!file.exists()) 
		{
		    Log.d(Tag,"reqShare file not exists:"+ImgPath);
		    return;
		}

		Bitmap bmp = BitmapFactory.decodeFile(ImgPath);
		WXImageObject imgObj = new WXImageObject(bmp);
		
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = imgObj;
		
		Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, 128, 72, true);
		bmp.recycle();
		msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("img");
		req.message = msg;
		if(nType==SceneTimeline )
		{
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}
		else if(nType==SceneSession )
		{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		api.sendReq(req);
	    Log.d(Tag,"reqShare Ok:"+ImgPath);
    }
    public void reqShareTxt(String text,int nType)
    {
		// 初始化一个WXTextObject对象
		WXTextObject textObj = new WXTextObject();
		textObj.text = text;

		// 用WXTextObject对象初始化一个WXMediaMessage对象
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		// 发送文本类型的消息时，title字段不起作用
		// msg.title = "Will be ignored";
		msg.description = text;

		// 构造一个Req
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("text"); // transaction字段用于唯一标识一个请求
		req.message = msg;
		if(nType==SceneTimeline )
		{
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}
		else if(nType==SceneSession )
		{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		// 调用api接口发送数据到微信
		api.sendReq(req);
		

	    Log.d(Tag,"reqShareTxt Ok:"+text);
    }
    
    public void reqShareUrl(String url, String title,String desc,int nType)
    {
    	// 初始化一个WXTextObject对象
		WXWebpageObject textObj = new WXWebpageObject();
		textObj.webpageUrl = url;

		// 用WXTextObject对象初始化一个WXMediaMessage对象
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		// 发送文本类型的消息时，title字段不起作用
		msg.title = title;
		msg.description = desc;

		// 构造一个Req
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("webpage"); // transaction字段用于唯一标识一个请求
		req.message = msg;
		if(nType==SceneTimeline )
		{
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}
		else if(nType==SceneSession )
		{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		// 调用api接口发送数据到微信
		api.sendReq(req);
		

	    Log.d(Tag,"reqShareUrl Ok:"+url);
    }

    public void reqShareTxtCB(String text,int nType)
    {
    	 // send appdata with no attachment
    	WXAppExtendObject appdata = new WXAppExtendObject("lallalallallal", "filePath");
    	
    	boolean bValue =  appdata.checkArgs();
    	if (!bValue)
    	{
    	    Log.d(Tag,"reqShareTxtCB Error:"+text);
    	}
    	
    	WXMediaMessage msg = new WXMediaMessage();
    	msg.title ="11";
    	msg.description = "22";
    	msg.mediaObject = appdata;
    	
    	SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("appdata");
		req.message = msg;
		
		if(nType==SceneTimeline )
		{
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}
		else if(nType==SceneSession )
		{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		// 调用api接口发送数据到微信
		api.sendReq(req);

	    Log.d(Tag,"reqShareTxtCB Ok:"+text);
    }
    
	// 瀵邦噣鏁撻懘姘殩閹风兘鏁撻弬銈嗗闁跨喐鏋婚幏鐑芥晸鐟楃喎鍩岀喊澶嬪闁跨喐鏋婚幏鐑芥晸閺傘倖瀚规惔鏃堟晸閺傘倖瀚归弮鍫曟晸閺傘倖瀚归柨鐔告灮閹烽攱鐨柨鐔告灮閹风兘鏁撻弬銈嗗濮婂懘鏁撻弬銈嗗闁跨噦鎷�
	@Override
	public void onReq(BaseReq req) 
	{
		switch (req.getType()) 
		{
		case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX: 
			Log.d(Tag,"onReq:COMMAND_GETMESSAGE_FROM_WX");
			break;
		case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX:
			Log.d(Tag,"onReq:COMMAND_SHOWMESSAGE_FROM_WX");
			break;
		default:
			break;
		}

	    Log.d(Tag,"onReq:"+req.getType());
	}

	// 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹峰嘲绨查柨鐔虹叓閸戙倖瀚归柨鐔煎徍绾板瀚瑰顕�鏁撻懘姘鳖暜閹风兘鏁撻弬銈嗗闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹风兘鏁撻弬銈嗗鎼存棃鏁撻弬銈嗗闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔稿焻绾板瀚归柨鐔告灮閹风兘鏁撻惌顐㈠殩閹风兘鏁撻弬銈嗗
	@Override
	public void onResp(BaseResp resp) {
		int result = 0;

	    Log.d(Tag,"errCode:"+resp.errCode);
	    
		switch (resp.errCode) {
		case BaseResp.ErrCode.ERR_OK:
		    String code = ((SendAuth.Resp) resp).code;
		    String Url =APP_ID+":"+AppSecret+":"+code+":authorization_code";
		    Log.d(Tag,Url);
		    Native.WxLoginGetAccessToken(Url);
			break;
		case BaseResp.ErrCode.ERR_USER_CANCEL:
		    Native.WxLoginGetAccessToken("ERR_USER_CANCEL");
			break;
		case BaseResp.ErrCode.ERR_AUTH_DENIED:
		    Native.WxLoginGetAccessToken("ERR_AUTH_DENIED");
			break;
		default:
		    Native.WxLoginGetAccessToken("REE_Unknow");
			break;
		}
	}
	
	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
}