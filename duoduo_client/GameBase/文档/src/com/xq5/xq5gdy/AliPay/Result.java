package com.xq5.xq5gdy.AliPay;

import java.util.HashMap;
import java.util.Map;

import org.cocos2dx.cpp.AppActivity;
import org.json.JSONObject;

import android.app.Activity;
import android.util.Log;

import com.xq5.xq5gdy.R;

final class Keys {

//	//閸氬牅缍旈煬顐″敜閼板崨d閿涘奔浜�088瀵拷銇旈惃锟�娴ｅ秶鍑介弫鏉跨摟
//	//public static final String DEFAULT_PARTNER = "2088611023340901";
//
//	//閺�埖顑欓弨顖欑帛鐎规繆澶勯崣锟�	public static final String DEFAULT_SELLER = "xq5@tonnn.com";
//
//	//閸熷棙鍩涚粔渚�寽閿涘矁鍤滈崝鈺冩晸閹达拷
	public static final String PRIVATE = "MIICXAIBAAKBgQC8kWNizl8SiC4vN0gRCgfwg3wBU/Ajs1CLkmQVxERS0Ord3m3oOZV1CKqGD9L3L4bOOndCbuSB2icX4SF6+4N4EsUFslJSuHdwYYks3XCwwkcUPCTgim7ls55ujRTYAi950z/Lenrmdmo1qF4Z4XCVcGUUG4gtaZkilzSyPW9rywIDAQABAoGADEb/DJDZOdCqCSL1Sz62857r9P4tFXX4WY4HfC9SL34wvI9d4o/VPlB5Ic9n8fYjNcwytZ2YvSeiAfkDgAleLtdogDNoMz7LLIbF3VAS9jtCi+GIu/vaQa7V5q4d67Ek1g1pQrqg09MFZnxczZgSJIv6CAnfw7HhQ9BdhXs7wqECQQD1g/BWq9zZ3Mf/i7wUu0xCM4wT0ccZtajV+2IdW/apgWR4YkV8pjRUW8cmmD66yD5nGxVEV6MUY/6KZJEZ3Ns7AkEAxJ7hKuj0wo+ozlFtybZGMHnQvzxj2/sUmneUvqPVV+2qqx7KNn43Jtevfv7qCZNdVCDl1AWMsamriqm93IYIsQJAU+lPOGAu8GssE+QvWSqWqr2xb4iwMtJKrS/o3b3ztQVbUGXN+AdaNUbU7xTzw6Ty5wxznVMSksKGWch7a0p3xwJARjcm2hgPMFv1QJi7yEkbj7ww29pJHwnvNap2qA4B5kCtriZ0uDkioPBtb8DA+2Jw9dY5vRmuQNQCuqYbc2E6QQJBAO5ZSMvQyzxPl65WqLa9jLw05c+xI7sogCoUgxK6Cs6VN1vB/YP3K3qf6exNVGzNWfOK2yNkkstkiMk1VvLtPLg=";
//
//	//閺�垯绮�婵嗗彆闁斤拷
//	public static final String PUBLIC = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCnxj/9qwVfgoUh/y2W89L6BkRAFljhNhgPdyPuBV64bfQNN1PjbCzkIM6qRdKBoLPXmKKMiFYnkd6rAoprih3/PrQEB/VsW8OoM8fxn67UDYuyBTqA23MML9q1+ilIZwBC2AQ2UBVOrFXfFl75p6/B5KsiNG9zpgmLCUYuLkxpLQIDAQAB";


}
public class Result {
	
	private static final Map<String, String> sResultStatus;

	private String mResult;
	
	String resultStatus = null;
	String memo = null;
	String result = null;
	boolean isSignOk = false;

	public Result(String result) {
		Log.i("test","Result = "+result);
		this.mResult = result;
	}

	static {
		Activity temp= (Activity)AppActivity.getApkInstance();
		sResultStatus = new HashMap<String,String>();
		sResultStatus.put("9000", temp.getString(R.string.alipay_9000));
		sResultStatus.put("4000", temp.getString(R.string.alipay_4000));
		sResultStatus.put("4001", temp.getString(R.string.alipay_4001));
		sResultStatus.put("6001", temp.getString(R.string.alipay_6001));
		sResultStatus.put("6002", temp.getString(R.string.alipay_6002));
	}

	public  String getResult() {
		String src = mResult.replace("{", "");
		src = src.replace("}", "");
		
		Log.i("test","getResult() = "+ getContent(src, "memo=", ";result"));
		return getContent(src, "memo=", ";result");
	}
	public String getresultStatus(){
		String src = mResult.replace("{", "");
		src = src.replace("}", "");
		return getContent(src, "resultStatus=", ";memo");
	}
	public String getResultStatusValue()	{
		String rs = getresultStatus();
		String re;
		if (sResultStatus.containsKey(rs)) {
			re = sResultStatus.get(rs);
		} else {
			Activity temp= (Activity)AppActivity.getApkInstance();
			re = temp.getString(R.string.alipay_other);
		}
		return re;
	}
//	public  void parseResult() {
//		
//		try {
//			String src = mResult.replace("{", "");
//			src = src.replace("}", "");
//			String rs = getContent(src, "resultStatus=", ";memo");
//			if (sResultStatus.containsKey(rs)) {
//				resultStatus = sResultStatus.get(rs);
//			} else {
//				resultStatus = "鍏朵粬閿欒";
//			}
//			resultStatus += "(" + rs + ")";
//
//			memo = getContent(src, "memo=", ";result");
//			result = getContent(src, "result=", null);
//			isSignOk = checkSign(result);
//			
//			Log.i("test","resultStatus = " +resultStatus);
//			Log.i("test","memo = " +memo);
//			Log.i("test","result = "+result);
//			Log.i("test","isSignOk = "+isSignOk);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//	}

//	private  boolean checkSign(String result) {
//		boolean retVal = false;
//		try {
//			JSONObject json = string2JSON(result, "&");
//
//			int pos = result.indexOf("&sign_type=");
//			String signContent = result.substring(0, pos);
//
//			String signType = json.getString("sign_type");
//			signType = signType.replace("\"", "");
//
//			String sign = json.getString("sign");
//			sign = sign.replace("\"", "");
//
//			if (signType.equalsIgnoreCase("RSA")) {
//				retVal = Rsa.doCheck(signContent, sign, Keys.PUBLIC);
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//			Log.i("Result", "Exception =" + e);
//		}
//		Log.i("Result", "checkSign =" + retVal);
//		return retVal;
//	}

	public  JSONObject string2JSON(String src, String split) {
		JSONObject json = new JSONObject();

		try {
			String[] arr = src.split(split);
			for (int i = 0; i < arr.length; i++) {
				String[] arrKey = arr[i].split("=");
				json.put(arrKey[0], arr[i].substring(arrKey[0].length() + 1));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return json;
	}

	private  String getContent(String src, String startTag, String endTag) {
		String content = src;
		int start = src.indexOf(startTag);
		start += startTag.length();

		try {
			if (endTag != null) {
				int end = src.indexOf(endTag);
				content = src.substring(start, end);
			} else {
				content = src.substring(start);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return content;
	}
}
