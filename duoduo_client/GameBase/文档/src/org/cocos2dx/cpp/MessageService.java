package org.cocos2dx.cpp;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class MessageService extends Service{

	@Override
	public IBinder onBind(Intent intent){
		
		return null;
	}
	@Override
	public void onCreate(){
		super.onCreate();
	}
	@Override
	public int onStartCommand(Intent intent,int flags,int startId){
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Log.e("test", "service is running");
			}
		}).start();
		return super.onStartCommand(intent, flags, startId);
		
	}
	
	@Override
	public void onDestroy(){
		super.onDestroy();
	}
}
