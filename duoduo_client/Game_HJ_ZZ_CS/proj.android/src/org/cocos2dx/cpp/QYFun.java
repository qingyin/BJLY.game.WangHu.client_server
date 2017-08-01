
package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.os.Bundle;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.content.Intent;
import android.media.MediaPlayer;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

public class QYFun {

    private static final int Create_progress = 13;
    private static final int Dwon_File_Progress_Message = 11;
    private static final int Dwon_File_Compete_Message = 12;
    private static final int Show_Update = 14;
    
	private  Context mContext = null;
    private int downLoadFileSize = 0;
    private  ProgressDialog DownFileProgress;
	String DownLoadfilename = "landlord.apk";

	private static int installFlag = 1;
	
	private static QYFun pInstance = null;
	private Handler mHandler = null;
	

    private MediaPlayer mPlayer = null;  
    private MediaRecorder mRecorder = null;  
    
    //语音文件保存路径  
    private String SoundFileName = null;
	
	public static QYFun getInstance()
	{
		if(pInstance==null)
		{
			pInstance = new QYFun();
		}
		return pInstance;
	}
	
	public void setContext( Context pContext) {
		this.mContext = pContext;
		initHandler(this.mContext);
	}

	public void initHandler( Context pContext)
	{
			mHandler= new Handler(pContext.getMainLooper()){
	        
	        public void handleMessage(android.os.Message msg) {

	        	if (msg.what == Create_progress) {

	        		DownFileProgress = new ProgressDialog(mContext);
	        		DownFileProgress.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
	        		DownFileProgress.setTitle("提示");
	        		DownFileProgress.setMessage("游戏文件下载中，请等待");
	        		DownFileProgress.setProgress(100);
	        		DownFileProgress.setIndeterminate(false);
	        		DownFileProgress.setCancelable(false);   
	        		DownFileProgress.setMax(msg.arg1);
	        		DownFileProgress.show(); 	
				}
	        	if(msg.what == Dwon_File_Progress_Message)
	        	{
	        		//Log.i("test","download");
	        		DownFileProgress.setProgress(downLoadFileSize/1024 );
	        	}
	        	
	        	if(msg.what == Dwon_File_Compete_Message && installFlag == 1 )
	        	{
					String absPath = Environment
							.getExternalStorageDirectory()
							+ "/Android/data/"
							+ mContext.getApplicationInfo().packageName + "/files/";
					openFile(new File(absPath + DownLoadfilename));
	        	}
	        	if(msg.what == Show_Update )
	        	{
	        		final int kSize = msg.getData().getInt("size");
					String info = msg.getData().getString("info");
					int isUpdate = msg.getData().getInt("isUpdate");
					final String url = msg.getData().getString("url");
	        		
	        		AlertDialog.Builder builder = new Builder(mContext);
	        		builder.setMessage(info+"请在wifi环境下下载...");
	        		builder.setTitle("更新提示");
	        		
	        		builder.setPositiveButton("确定",
	        				new DialogInterface.OnClickListener() {
	        					@Override
	        					public void onClick(DialogInterface dialog,int which) 
	        					{
	        						dialog.dismiss();
	        						startDownFile(url,kSize);
	        					}	
	        				});
	        		
	        		if (isUpdate == 0)
	        		{
	        			builder.setNegativeButton("取消",
	        					new DialogInterface.OnClickListener() {
	        						@Override
	        						public void onClick(DialogInterface dialog,
	        								int which) {
	        							dialog.dismiss();
	        						}
	        					});
	        		}
	        		
	        		builder.create().show();
	        		
	        	}
	        }
	    };
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
	
		DownLoadfilename = url.substring(url.lastIndexOf("/") + 1);
		
		URL myURL = new URL(url);
		
		URLConnection conn = myURL.openConnection();
		conn.connect();
		InputStream is = conn.getInputStream();
		final File file = new File(path + DownLoadfilename);
		if (is == null)
			throw new RuntimeException("stream is null");
		FileOutputStream fos = new FileOutputStream(file);
		// 闂佺娉涢敃銉╁汲閻旂厧绠叉い鏃囧亹閹界娀鏌涜箛瀣姤闁活厽鍎抽銉╂晸娴犲妫橀柛銉檮椤愪粙鏌涘鐐
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


	public void showVersionUpdate(final String url,String info,final int iSize,int isUpdate)
	{
		Log.d("tag", "showVersionUpdate" );
		
		Message msg = new Message();
		msg.what = Show_Update;
		Bundle bundle = new Bundle();
		bundle.putInt("size", iSize);
		bundle.putString("url", url);
		bundle.putInt("isUpdate", isUpdate);
		bundle.putString("info", info);
		msg.setData(bundle);
		mHandler.sendMessage(msg);
	}
}