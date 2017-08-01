package com.xq5.xq5gdy.AliPay;

import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.cocos2dx.cpp.AppActivity;

import android.os.Handler;
import android.os.Message;
import android.widget.Toast;

import com.alipay.android.app.sdk.AliPay;
import aj.ajention.tools.AJLog;
public class AliPayFun {

	private static final int RQF_PAY = 1;
	private static final int RQF_LOGIN = 2;

	private static AppActivity m_appInstance ;

	public AliPayFun(AppActivity _node){
		m_appInstance = _node;
	}
	
	Handler mHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			Result result = new Result((String) msg.obj);

			AJLog.i("test","getStatus = "+result.getResultStatusValue());
			
			Toast.makeText(m_appInstance, result.getResultStatusValue(),Toast.LENGTH_LONG).show();
			switch (msg.what) {
			case RQF_PAY: {
				AJLog.e("test", "RQF_pay");
				String reString = result.getresultStatus();
				if (reString.equals("9000")) {
					AJLog.e("test", "pay_ok");
				}
	
			}break;
			case RQF_LOGIN: {
				AJLog.e("test", "RQF_Login");
				Toast.makeText(m_appInstance, result.getResult(),
						Toast.LENGTH_SHORT).show();

			}
				break;
			default:
				AJLog.e("test", "Rqf_error");
				break;
			}
		};
	};

	public void aliPay(String info) {

		AJLog.e("test", "_infor = " + info);
		try {
			//String info = getNewOrderInfo(_infor);

			String sign = Rsa.sign(info, Keys.PRIVATE);

			sign = URLEncoder.encode(sign);

			info += "&sign=\"" + sign + "\"&" + getSignType();
			AJLog.i("test", "start pay");
			
			final String orderInfo = info;
			AJLog.i("test", "orderInfo = " + orderInfo);
			new Thread() {
				public void run() {
					AJLog.i("test", "orderInfo = 1" );
					AliPay alipay = new AliPay(m_appInstance, mHandler);
					// 閻犱礁澧介悿鍡樼▔閻戞鐓撶紒鐘洪哺鑶╃�顔奸獜缁辨繃绋夊鍫晭缂傚喚鍣ｇ划顖滄媼閵堝嫯绀嬬紒鎯с仒缁楀倿鎮�
					// alipay.setSandBox(true);
					String result = alipay.pay(orderInfo);

					
					AJLog.e("test","res = "+result);
					Message msg = new Message();
					msg.what = RQF_PAY;
					msg.obj = result;
					mHandler.sendMessage(msg);
				}
			}.start();
		} catch (Exception ex) {
			ex.printStackTrace();
			AJLog.e("test","aliPay_error");
			Toast.makeText(m_appInstance, "pay_faild", Toast.LENGTH_SHORT)
					.show();
		}

	}

//	private String getNewOrderInfo(String test) {
//		AJLog.e("test", "getNewOrderInfo");
//		StringBuilder sb = new StringBuilder();
//
//		sb.append("partner=\"");
//		sb.append(Keys.DEFAULT_PARTNER);
//
//		// 閻犱降鍨瑰畷鐔访规担瑙勫瘻闁告瑱鎷�//		sb.append("\"&out_trade_no=\"");
//		sb.append(test);
//
//		// 闁哥喎妫楅幖褔宕ュ鍥嗭拷
//		sb.append("\"&subject=\"");
//		sb.append("test1");
//
//		sb.append("\"&body=\"");
//		sb.append("闁哄嫮鍠愬﹢鈩冪閺冿拷妫為柣妤�焸閸ｅ墽鏁敓浠嬪礂閸愨晜娈�);
//
//		// 濞寸娀鏀遍悧鍛婄閵堝棙顬栭梺鍙夊灴椤わ拷
//		sb.append("\"&total_fee=\"");
//		sb.append("0.01");
//
//		// 闁猴拷鍨划顖滐拷濠靛洦绠涢柛鏂猴拷濞呮帗绋夌拠鎻捫楅梺顐ｆ皑閻擄繝宕崱妯虹厱
//		// 缂傚啯鍨归悵顖炴煂鐏炴儳鐦归悗瑙勬皑濞堟垶銇勯悽鍛婃〃http閻犱警鍨扮欢鐐哄Υ閿燂拷		// 闂傚浄鎷烽—鍖睷L encode闁靛棴鎷�//		sb.append("\"&notify_url=\"");
//		// 缂傚啯鍨靛鍐閿熺瓔娓介柛瀣摋RL缂傚倹鐗滈悥锟�//		sb.append(URLEncoder
//				.encode("http://api.xq5.com/alipay_mobile/callback/"));
//
//		sb.append("\"&service=\"mobile.securitypay.pay");
//		
//		sb.append("\"&_input_charset=\"UTF-8");
//		
//		sb.append("\"&return_url=\"");
//		sb.append(URLEncoder.encode("http://m.alipay.com"));
//
//		sb.append("\"&payment_type=\"1");
//
//		sb.append("\"&seller_id=\"");
//		//sb.append(Keys.DEFAULT_SELLER);
//
//		// 濠碘�鍊归悘濉籬ow_url闁稿﹪妫跨拹鐔虹矚閻氬绀夐柛娆樺灟缁楀瀵奸敓锟�	// sb.append("\"&show_url=\"");
//		sb.append("\"&it_b_pay=\"1m");
//		sb.append("\"");
//
//		String reString = new String(sb);
//		AJLog.i("test", "re = " + reString);
//		return reString;
//	}

	private String getOutTradeNo() {
		AJLog.e("test", "getOutTradeNo");

		SimpleDateFormat format = new SimpleDateFormat("MMddHHmmss");
		Date date = new Date();
		String key = format.format(date);

		java.util.Random r = new java.util.Random();
		key += r.nextInt();
		key = key.substring(0, 15);
		AJLog.d("test", "outTradeNo: " + key);
		return key;
	}

	private String getSignType() {
		AJLog.e("test", "getSignType");
		return "sign_type=\"RSA\"";
	}
}
