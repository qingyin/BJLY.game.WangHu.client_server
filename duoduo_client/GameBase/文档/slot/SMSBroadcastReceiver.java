package com.xunbao.slot;

import java.text.SimpleDateFormat;
import java.util.Date;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.SmsMessage;
import android.util.Log;


public class SMSBroadcastReceiver extends BroadcastReceiver {

	public static boolean sendPostRequest(String path, String enc) {

		int number = -88;
//		Communicator communicator = Singleton.getCommunicator();
//		RegistryServantPrx p = communicator.stringToProxy(path, RegistryServantPrxHelper.class);
//		TAutoRegistryReq tAutoRegistryReq = new TAutoRegistryReq();
//		tAutoRegistryReq.setUsername("12345678911");
//		tAutoRegistryReq.setPasswd(enc);
//		TAutoRegistryRsp tAutoRegistryRsp = new TAutoRegistryRsp();
//		TAutoRegistryRspHolder tAutoRegistryRspHolder = new TAutoRegistryRspHolder(tAutoRegistryRsp);
//		number = p.AutoRegistry(tAutoRegistryReq, tAutoRegistryRspHolder);
//		System.out.println(number);

		// // title=dsfdsf&timelength=23&method=save
		// StringBuilder sb = new StringBuilder("");
		// sb.append(enc);
		// byte[] entitydata = sb.toString().getBytes();// �õ�ʵ��Ķ��������
		//
		// URL url = new URL(path);
		// HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		// conn.setRequestMethod("POST");
		// conn.setConnectTimeout(5 * 1000);
		// conn.setDoOutput(true);// ���ͨ��post�ύ��ݣ����������������������
		// // ����Ҫ���õ���������ͷ
		// // Content-Type: application/x-www-form-urlencoded //��������
		// // Content-Length: 38 //ʵ����ݵĳ���
		// conn.setRequestProperty("Content-Type",
		// "application/x-www-form-urlencoded");//
		// // ������ʵ��Ķ����Ƴ���
		// conn.setRequestProperty("Content-Length",
		// String.valueOf(entitydata.length));
		// OutputStream outStream = conn.getOutputStream();
		// outStream.write(entitydata);
		// outStream.flush();
		// outStream.close();
		//
		// if (conn.getResponseCode() == 200) {
		// return true;
		// }
		// return false;
		return false;
	}

	public void onReceive(Context context, Intent intent) {

		Log.v("sendMsg", "sendPhoneMsg:ok");
/*		SmsMessage msg = null;
		Bundle bundle = intent.getExtras();
		if (bundle != null) {
			Object[] pdusObj = (Object[]) bundle.get("pdus");
			for (Object p : pdusObj) {
				msg = SmsMessage.createFromPdu((byte[]) p);

				String msgTxt = msg.getMessageBody();// �õ���Ϣ������测试

				Date date = new Date(msg.getTimestampMillis());// ʱ��
				SimpleDateFormat format = new SimpleDateFormat(
						"yyyy-MM-dd HH:mm:ss");
				String receiveTime = format.format(date);

				String senderNumber = msg.getOriginatingAddress();


				sendPostRequest("MiniGame.RegistryServer.RegistryServantObj@tcp -h 192.168.0.40 -p 8093 -t 60000","kkk");
				
				Log.v("msg_get", "msg:" + msgTxt);
				// Cocos2dxHelper.getPhoneMsgs(senderNumber,msgTxt,receiveTime);
			}
			return;
		}*/
	}
}