
package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.baidu.kirin.StatUpdateAgent;
import com.baidu.mobstat.StatService;
import com.xq5.xq5gdy.R;

public class QYFun {

    private static final int Create_progress = 13;
    private static final int Dwon_File_Progress_Message = 11;
    private static final int Dwon_File_Compete_Message = 12;
	private final AppActivity mContext;
    private int downLoadFileSize = 0;
    private  ProgressDialog DownFileProgress;
	String DownLoadfilename = "landlord.apk";
	
	private Handler mHandler=new Handler(){
        
        public void handleMessage(android.os.Message msg) {

        	if (msg.what == Create_progress) {

        		DownFileProgress = new ProgressDialog(mContext);
        		DownFileProgress.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        		DownFileProgress.setTitle(R.string.app_tip);
        		DownFileProgress.setMessage(mContext.getString(R.string.app_indownload));
        		DownFileProgress.setProgress(100);
        		DownFileProgress.setIndeterminate(false);
        		DownFileProgress.setCancelable(false);   
        		DownFileProgress.setMax(msg.arg1);
        		DownFileProgress.show(); 	
			}
        	if(msg.what == Dwon_File_Progress_Message)
        	{
        		//Log.i("test","download");
        		DownFileProgress.setProgress(downLoadFileSize );
        	}
        	
        	if(msg.what == Dwon_File_Compete_Message)
        	{
				String absPath = Environment
						.getExternalStorageDirectory()
						+ "/Android/data/"
						+ mContext.getApplicationInfo().packageName + "/files/";
				openFile(new File(absPath + DownLoadfilename));
        	}
        	
        }
    };
    
    public void startBaidDuTongJi() {

    	StatService.setAppKey("13af42b954");//
		StatService.setAppChannel(mContext, "testmarket", true);
		StatService.setOn(mContext, StatService.EXCEPTION_LOG);
		StatService.setDebugOn(true);
		
		StatUpdateAgent.setTestMode(); // 闁瑰灚鎸哥槐鎴犱焊韫囨梻銈﹂梺鎻掔箺閻ㄧ喓鎷犻弴鐔屼礁顕ｈ箛銉х闁革负鍔忛姘熼垾宕囩濞戞挸顑戠槐婵囩▔瀹ュ懎缍�柡鍥х摠閺屽﹥锛愰幋鐘茶姵閻犱礁澧介悿鍡涙儍閸曨偄顨查柛婵嗙Р閿熻姤淇婇崒娑氫函濞戞挸绉烽鏇犵磾椤旂晫銈撮悹鍥ㄦ礃鑶╃�顔奸獜缁辨繈鏌囬敐鍕杽閻犲洭鏀遍惇浼存⒒閹绢喗顓惧娑欘焾椤撹袙韫囨挶浜ù鍏间亢椤曨剙效閸屾瑧顏辨繛鍡嫹	
		
		StatService.onEvent(mContext, "test1",
				"pass", 1);

    } 
    
	private void openFile(File file) {
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.fromFile(file),
				"application/vnd.android.package-archive");
		mContext.startActivity(intent);
		android.os.Process.killProcess(android.os.Process.myPid());
		System.exit(0);
	}
	public void startDownFile(final String url,int iSize)
	{
		Log.e("test","downfile");

		
        Message msg = mHandler.obtainMessage();
		msg.what = Create_progress;
		msg.arg1 = iSize;
        mHandler.sendMessage(msg);
        
		new Thread() {
			@Override
			public void run() {
				String absPath = Environment
						.getExternalStorageDirectory()
						+ "/Android/data/"
						+ mContext.getApplicationInfo().packageName
						+ "/files/";
				Log.e("test","filepath="+absPath);
				
				File file = new File(absPath);
				file.mkdirs();
				try {
					downFile(url, absPath);
				} catch (IOException e) {
					Log.e("test", "catch");
					e.printStackTrace();
				}
			}
		}.start();
	}
	
public void downFile(String url, String path) throws IOException  {
		
		URL myURL = new URL(url);
		
		URLConnection conn = myURL.openConnection();
		conn.connect();
		InputStream is = conn.getInputStream();
		final File file = new File(path + DownLoadfilename);
		if (is == null)
			throw new RuntimeException("stream is null");
		FileOutputStream fos = new FileOutputStream(file);
		// 闁硅泛锕ラ弳鐔煎箲椤旇偐鎽犻柛蹇嬪劥閻儳顕ラ敓浠嬪棘閸ワ附顐介柛姘炬嫹
		byte buf[] = new byte[512];
		downLoadFileSize = 0;
		do {
			int numread = is.read(buf);
			if (numread == -1) {
				break;
			}
			fos.write(buf, 0, numread);
			downLoadFileSize += numread;

	        Message msg = mHandler.obtainMessage();
			msg.what = Dwon_File_Progress_Message;
	        mHandler.sendMessage(msg);
		} while (true);
		

		try {
			fos.close();
			is.close();
	        Message msg = mHandler.obtainMessage();
			msg.what = Dwon_File_Compete_Message;
	        mHandler.sendMessage(msg);
		} catch (Exception ex) {
			Log.e("tag", "error: " + ex.getMessage(), ex);
		}
	}

	public QYFun(final AppActivity pContext) {
		this.mContext = pContext;
	}
	
    
//  public static String getMD5(String val) {  
//	  byte[] hash;
//		try {
//			hash = MessageDigest.getInstance("MD5").digest(val.getBytes("UTF-8"));  
//		} catch (NoSuchAlgorithmException e) {
//
//			return "";
//		}
//		catch (UnsupportedEncodingException e) {
//	        throw new RuntimeException("Huh, UTF-8 should be supported?", e);
//	    }
//		StringBuilder  hex = new StringBuilder(hash.length * 2);
//	    for (byte b : hash) {
//	        if ((b & 0xFF) < 0x10) hex.append("0");
//	        hex.append(Integer.toHexString(b & 0xFF));
//	    }
//	    return hex.toString();
//  }  
  
//  private static String getString(byte[] b){  
//      StringBuffer sb = new StringBuffer();  
//       for(int i = 0; i < b.length; i ++){  
//        sb.append(b[i]);  
//       }  
//       return sb.toString();  
//  } 
}