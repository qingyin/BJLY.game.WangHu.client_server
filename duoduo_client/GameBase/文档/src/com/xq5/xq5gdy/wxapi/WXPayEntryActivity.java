package com.xq5.xq5gdy.wxapi;


import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;

import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.xq5.xq5gdy.R;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler{
	
	//private static final String TAG = "test";
	
    private IWXAPI api;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       //setContentView(R.layout.pay_result);
      //  Log.e("test","wxpayentryActivity");
    	api = WXAPIFactory.createWXAPI(this, WXEntryActivity.APP_ID);
        api.handleIntent(getIntent(), this);
        
        finish();
    }

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
        api.handleIntent(intent, this);
        
        //Log.e("test", "onNewIntent");
	}

	@Override
	public void onReq(BaseReq req) {
		//Log.e("test", "onReq");
		finish();
	}

	@Override
	public void onResp(BaseResp resp) {
		//Log.d(TAG, "onPayFinish, errCode = " + resp.errCode);
		//Log.e("test", "onResp");
		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle(R.string.app_tip);
			builder.setMessage(getString(R.string.pay_result_callback_msg, String.valueOf(resp.errCode)));
			builder.show();
		
		}
		finish();
	}
}