package com.pkqp.game.duoduo;


import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import com.pkqp.game.duoduo.wxapi.*;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

public class Native extends Cocos2dxHelper
{
	public static native void WxLoginGetAccessToken(String kUrl);
	
	 public static void LoginWX()
	  {
		 Intent intent = new Intent(Cocos2dxActivity.getContext(), WXEntryActivity.class);
		intent.putExtra(WXEntryActivity.ReqWxLogin,"wxlogin");
		 Cocos2dxActivity.getContext().startActivity(intent);
	  }
	 public static void ShareWX()
     {
     } 
	 public static void showWebView(String kUrl)
     {
		 Log.d("showWebView",kUrl);
		 Uri uri = Uri.parse(kUrl);
		 Intent it = new Intent(Intent.ACTION_VIEW, uri);    
		 Cocos2dxActivity.getContext().startActivity(it);
     }
}
