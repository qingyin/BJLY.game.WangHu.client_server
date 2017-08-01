package org.cocos2dx.cpp;

import aj.ajention.tools.AJAPI;
import aj.ajention.tools.AJLog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo.State;

public class ConnectListen extends BroadcastReceiver {
	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		ConnectivityManager manager = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);

		int status = 0;
		State state = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE)
				.getState();
		if (State.CONNECTED == state) {
			status = 2;
		}
		state = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI)
				.getState();
		if (State.CONNECTED == state) {
			status = 1;
		}
		if (AJAPI.isMainLooper()) {
			AppActivity.getInstance().ListenConnectStatus(status);
		}
	} // 如果无网络连接activeInfo为null

}
