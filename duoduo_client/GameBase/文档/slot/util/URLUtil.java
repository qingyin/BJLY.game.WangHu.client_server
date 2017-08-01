package com.xunbao.slot.util;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLEncoder;

public class URLUtil {

	public static String sendGet(String url,String param) throws Exception {
		String result = "";
		String urlName = url + "?" + param;

		BufferedReader in = null;
		try {
			
			URL U = new URL(urlName);
			URLConnection connection = U.openConnection();
			connection.connect();
			
			in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
			String line;
			while ((line = in.readLine())!= null) {
				result += line;
			}
		}
		catch(Exception e) {
			throw e;
		} finally {
			if(in != null) {
				try {
					in.close();
				} catch(Exception e1) {
				}
			}
		}
		return result;
	}
	
	public static String sendPost(String url,String param) throws Exception
	{
		String result="";
		
		PrintWriter out = null;
		BufferedReader in = null;
		try {
			URL httpurl = new URL(url);
			HttpURLConnection httpConn = (HttpURLConnection)httpurl.openConnection();       
			httpConn.setDoOutput(true);
			httpConn.setDoInput(true);
			out = new PrintWriter(httpConn.getOutputStream());
			out.print(param);
			out.flush();
			out.close();
			in = new BufferedReader(new InputStreamReader(httpConn.getInputStream()));
			String line;
			while ((line = in.readLine())!= null) {
				result += line; 
			}
		}
		catch(Exception e) {
			throw e;
		}
		finally {
			if(out != null) {
				try {
					out.flush();
					out.close();
				} catch(Exception e1) {
				}
			}
			if(in != null) {
				try {
					in.close();
				} catch(Exception e1) {
				}
			}
		}
		return result;
	}
	
	public static void main(String[] args) {
		try {
			String param = "DESTADDR=10655867823&MSG="+URLEncoder.encode("A#8a3","UTF-8")+"&MOBILE=13005565005&LINKID=60680146&MTDATE="+URLEncoder.encode("2012-06-14 15:49:10", "UTF-8");
			String result = sendGet("http://localhost:8080/sms_nomo_adapter/fee",param);
			System.out.println(result);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
