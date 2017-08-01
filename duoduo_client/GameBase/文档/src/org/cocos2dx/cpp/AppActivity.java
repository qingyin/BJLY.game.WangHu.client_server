/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import aj.ajention.tools.AJAPI;
import aj.ajention.tools.AJDefine;
import aj.ajention.tools.AJLog;
import aj.ajention.tools.UiTools;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Vibrator;
import android.provider.MediaStore;
import android.telephony.TelephonyManager;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.webkit.WebView;
import cn.jpush.android.api.JPushInterface;

import com.baidu.mobstat.StatService;
import com.xq5.xq5gdy.AliPay.AliPayFun;
import com.xq5.xq5gdy.UPPay.UPPay;
import com.xq5.xq5gdy.wxapi.WXEntryActivity;

public class AppActivity extends Cocos2dxActivity {

	public static final int ACTIVE_TYPE_PHOTO = 1;// 鎷嶇収
	public static final int ACTIVE_CROP_PHOTO = 2;// 鐩稿唽
	public static final int CROP_PHOTO = 3;
	public static final int ACTIVE_WX_RES = 4;// 微信返回消息
	public static final int ACTIVE_GET_HEADIMAGE = 5;

	public final int PHOTO_REQUEST_GALLERY = 1000;
	public final int PHOTO_REQUEST_CUT = 1001;
	public final int PHOTO_REQUEST_CAREMA = 1002;

	private String fileName = "tempHeadImage.jpg";
	private Uri imageUri;
	public static AppActivity m_apkInstance = null;

	private TelephonyManager m_telephonyManager;

	private QYFun mQyFun;
	private AliPayFun m_aliPayFun;
	private UPPay mUppPay;

	private UiTools mUiTools;

	private ConnectListen mConnectListen;
	public String tag = "test";

	private Vibrator mVibrator;

	// ------------------------------------------

	// Handler handler = new Handler() {
	// public void handleMessage(android.os.Message msg) {
	// switch (msg.what) {
	// case ACTIVE_WX_RES: {
	// WXisNotInstall();
	// }
	// break;
	// case ACTIVE_GET_HEADIMAGE: {
	// String teString = (String) msg.obj;
	// AJLog.d(tag, "handle =" + teString);
	// headImagePath(teString);
	// }
	// break;
	// }
	//
	// }
	// };

	@Override
	protected void onResume() {
		AJLog.d(tag, "onResume");
		super.onResume();
		StatService.onResume(this);
		JPushInterface.onResume(this);
	}

	@Override
	protected void onPause() {
		super.onPause();
		AJLog.d(tag, "onPause");
		StatService.onPause(this);
		JPushInterface.onPause(this);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		AJLog.d(tag, "keyCode =" + keyCode);
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			webViewOnKeyBack(keyCode);
			return true;
		default:
			break;
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public void onDestroy() {
		AJLog.d(tag, "onDestroy");
		super.onDestroy();
		closeConnectListen();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		AJLog.d(tag, "onCreate");
		super.onCreate(savedInstanceState);

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		m_apkInstance = this;
		m_telephonyManager = (TelephonyManager) this
				.getSystemService(TELEPHONY_SERVICE);

		mVibrator = (Vibrator) this.getSystemService(VIBRATOR_SERVICE);
		initSdk();
		
		JPushInterface.setDebugMode(true);
        JPushInterface.init(this);
	}

	private void initSdk() {
		mQyFun = new QYFun(this);
		mQyFun.startBaidDuTongJi();
		m_aliPayFun = new AliPayFun(this);
		mUppPay = new UPPay(this);
	}

	public static Object getApkInstance() {
		AJLog.d("test", "getApkInstance");
		return m_apkInstance;
	}

	public static AppActivity getInstance() {
		return m_apkInstance;
	}

	public void openConnectListen() {
		if (mConnectListen != null)
			return;
		AJLog.d(tag, "openConnectListen");
		IntentFilter filter = new IntentFilter(
				ConnectivityManager.CONNECTIVITY_ACTION);
		mConnectListen = new ConnectListen();
		this.registerReceiver(mConnectListen, filter);
	}

	public void closeConnectListen() {
		if (mConnectListen != null) {
			AJLog.d(tag, "closeConnectListen");
			this.unregisterReceiver(mConnectListen);
		}
	}

	private void crop(Uri uri) {
		AJLog.d(tag, "int crop()");
		// 裁剪图片意图
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, "image/*");
		intent.putExtra("crop", "false");
		// 裁剪框的比例，1：1
		intent.putExtra("aspectX", 1);
		intent.putExtra("aspectY", 1);
		// 裁剪后输出图片的尺寸大小
		intent.putExtra("outputX", 256);
		intent.putExtra("outputY", 256);

		intent.putExtra("outputFormat", "JPG");// 图片格式
		intent.putExtra("noFaceDetection", true);// 取消人脸识别
		intent.putExtra("return-data", true);
		// 开启一个带有返回值的Activity，请求码为PHOTO_REQUEST_CUT
		startActivityForResult(intent, PHOTO_REQUEST_CUT);
	}

	public String getVersionName() {
		String versionName = "";
		try {
			PackageInfo info = this.getPackageManager().getPackageInfo(
					this.getPackageName(), 0);
			versionName = info.versionName;
		} catch (Exception e) {
			e.printStackTrace();
		}
		AJLog.d(tag, "getVersionName=" + versionName);
		return versionName;
	}

	public String getProvidersName() {
		return aj.ajention.tools.AJGetPhoneInfo.getProvidersName(this);
	}

	public String getOsVerSionName() {
		String versionString = android.os.Build.VERSION.RELEASE;
		AJLog.v(tag, "getOsVerSionName()=" + versionString);
		return versionString;
	}

	public String getSource() {
		String source = "";// getString(R.string.app_source);
		try {
			InputStreamReader inputReader = new InputStreamReader(
					getResources().getAssets().open("androidSource"));
			BufferedReader bufReader = new BufferedReader(inputReader);
			source = bufReader.readLine();
			inputReader.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		AJLog.d(tag, "source =" + source);
		return source;
	}

	public float getBatterLevel() {
		return AJAPI.getBatterLevel(this);
	}

	public void updataApk(String url, int iSzie) {
		// mQyFun.startDownFile(url, iSzie);
		AJAPI.downloadFile(this, url);
	}

	public String getDeviceName() {
		return android.os.Build.MODEL;
	}

	public String getGamePackageName() {
		return this.getPackageName();
	}

	public void setVibrator(int time) {
		mVibrator.vibrate(time);
	}

	public void callPhone(String phone) {
		
		AJLog.d(tag,"callphone() :phone="+phone);
		
		//String phString = "tel://"+phone;
		//ACTION_DIAL ACTION_CALL
		Intent intent = new Intent(Intent.ACTION_DIAL,
				Uri.parse(phone));
		startActivity(intent);
	}

	public void getHeadImageFromCamera() {

		// // 判断存储卡是否可以用，可用进行存储
		File tempFile = new File(
				AJDefine.getFolderPath(AJDefine.gameHeadImageFolder), fileName);
		try {
			if (tempFile.exists())
				tempFile.delete();
			tempFile.createNewFile();
		} catch (Exception e) {
			// TODO: handle exception
		}
		// 从文件中创建uri
		Intent intent = new Intent("android.media.action.IMAGE_CAPTURE");
		imageUri = Uri.fromFile(tempFile);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
		startActivityForResult(intent, PHOTO_REQUEST_CAREMA);
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		return glSurfaceView;
	}

	public String getDeviceID() {
		return m_telephonyManager.getDeviceId();
	}

	public void setAliPay(String info) {
		m_aliPayFun.aliPay(info);
	}

	public void setYinlianPay(String info) {
		mUppPay.setUPPay(info);
	}

	public void setWeixnPay(String info) {
		Intent intent = new Intent(AppActivity.this, WXEntryActivity.class);
		intent.putExtra(WXEntryActivity.ResWXPay, info);
		startActivity(intent);
	}

	public String getUserAgent() {
		WebView webview = new WebView(this);
		webview.layout(0, 0, 0, 0);
		String userAgent = webview.getSettings().getUserAgentString();
		webview = null;
		return userAgent;
	}

	public void setYibaoPay(String info) {
		AJLog.d(tag, "setYinlianPay = " + info);
		Intent intent = new Intent();
		intent.setAction("android.intent.action.VIEW");
		intent.setData(Uri.parse(info));
		startActivity(intent);
	}

	public void resWeixinWebpage(String data) {
		AJLog.d(tag, "resWeixinWebpage0 data=" + data);
		Intent intent = new Intent(AppActivity.this, WXEntryActivity.class);
		AJLog.d(tag, "resWeixinWebpage1");
		intent.putExtra(WXEntryActivity.ResWebpage, data);
		AJLog.d(tag, "resWeixinWebpage2");
		startActivityForResult(intent, ACTIVE_WX_RES);
		AJLog.d(tag, "resWeixinWebpage3");
	}

	public void resWeixinAppInfo(String data) {
		AJLog.d(tag, "resWeixinAppInfo1 data=" + data);
		Intent intent = new Intent(AppActivity.this, WXEntryActivity.class);
		intent.putExtra(WXEntryActivity.ResAppData, data);
		startActivityForResult(intent, ACTIVE_WX_RES);
		AJLog.d(tag, "resWeixinAppInfo2");
	}

	public void getPhotoFile(int tag) {
		Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
		intent.setType("image/*");// 鐩哥墖绫诲瀷
		startActivityForResult(intent, tag);
	}

	public void getHeadImage(int tag) {
		if (1 == tag) {
			getPhotoFile(PHOTO_REQUEST_GALLERY);
		} else if (2 == tag) {
			getHeadImageFromCamera();
		}
	}
	
	public void shareImage(final String filePath) {
		new Thread(new Runnable() {
			@Override
			public void run() {
				Intent intent = new Intent("android.intent.action.SEND");
				intent.setType("image/*");
				intent.putExtra(Intent.EXTRA_SUBJECT, "分享");
				intent.putExtra(Intent.EXTRA_TEXT, "星期五干瞪眼");
				intent.putExtra(Intent.EXTRA_STREAM,
						Uri.parse("file:" + filePath));
				AJLog.i(tag, "filepath=" + filePath);
				intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				AppActivity.this.startActivity(Intent.createChooser(intent,
						"干瞪眼图片分享"));
			}
		}).start();
	}

	public void openApkFile(String path){
		File file = new File(path);
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.fromFile(file),
				"application/vnd.android.package-archive");
		startActivity(intent);
		android.os.Process.killProcess(android.os.Process.myPid());
		System.exit(0);
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == ACTIVE_TYPE_PHOTO) {
			if (data == null || "".equals(data))
				return;
			Uri uri = data.getData();
			String[] proj = { MediaStore.Images.Media.DATA };
			Cursor actualimagecursor = managedQuery(uri, proj, null, null, null);
			int actual_image_column_index = actualimagecursor
					.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
			actualimagecursor.moveToFirst();
			String img_path = actualimagecursor
					.getString(actual_image_column_index);
			AJLog.d(tag, "img_path = " + img_path);
			File iconFile = new File(img_path);
			// //iconFile.get
			String filePath = Environment.getExternalStorageDirectory() + "/"
					+ "gandengyanFaceback.jpg";
			Bitmap photo = null;
			try {
				photo = BitmapFactory
						.decodeStream(new FileInputStream(iconFile));
			} catch (FileNotFoundException e3) {
				e3.printStackTrace();
			}
			photo = Bitmap.createBitmap(photo);
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			photo.compress(Bitmap.CompressFormat.JPEG, 50, baos);
			AJLog.d(tag, "QY Activity7");
			try {
				FileOutputStream fos = new FileOutputStream(filePath);
				fos.write(baos.toByteArray());
				fos.flush();
				fos.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			AJLog.d(tag, "QY Activity8");
			nativeFaceBakcImage(filePath);
		}
		if (requestCode == PHOTO_REQUEST_GALLERY) {
			// 从相册返回的数据
			if (data != null) {
				// 得到图片的全路径
				Uri uri = data.getData();
				crop(uri);
			}
		}
		if (requestCode == PHOTO_REQUEST_CAREMA) {
			if (resultCode != RESULT_OK)
				AJLog.d(tag, "PHOTO_REQUEST_CAREMA is not ok");
			// 从相机返回的数据
			crop(imageUri);
		}
		if (requestCode == PHOTO_REQUEST_CUT) {
			// 从剪切图片返回的数据
			if (data == null)
				return;
			Bitmap bitmap = data.getParcelableExtra("data");
			ByteArrayOutputStream baos = new ByteArrayOutputStream();

			bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);

			byte[] byteData = baos.toByteArray();

			// String md5 = MD5.getMessageDigest(byteData);

			String fileName = "tempHeadImage.jpg";// MD5.getMessageDigest(byteData)+".png";

			AJLog.d(tag, "md5 size =" + fileName.length());
			AJLog.d(tag, "md5 str=" + fileName);

			String filePath = AJDefine
					.getFolderPath(AJDefine.gameHeadImageFolder) + fileName;
			AJLog.d(tag, "filePath=" + filePath);
			try {
				FileOutputStream fos = new FileOutputStream(filePath);
				fos.write(byteData);
				fos.flush();
				fos.close();
			} catch (IOException e) {
				AJLog.d(tag, "catch!: if (requestCode == PHOTO_REQUEST_CUT)");
				e.printStackTrace();
			}

			AJLog.d(tag, "ACTIVE_GET_HEADIMAGE filePath=" + filePath);
			if (AJAPI.isMainLooper()) {
				AJLog.d(tag, "isMainLooper");
				headImagePath(filePath);
			}
		}

	}

	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis())
				: type + System.currentTimeMillis();
	}

	private Bitmap comp(Bitmap image) {

		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		image.compress(Bitmap.CompressFormat.JPEG, 100, baos);
		if (baos.toByteArray().length / 1024 > 1024) {// 判断如果图片大于1M,进行压缩避免在生成图片（BitmapFactory.decodeStream）时溢出
			baos.reset();// 重置baos即清空baos
			image.compress(Bitmap.CompressFormat.JPEG, 50, baos);// 这里压缩50%，把压缩后的数据存放到baos中
		}
		ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());
		BitmapFactory.Options newOpts = new BitmapFactory.Options();
		// 开始读入图片，此时把options.inJustDecodeBounds 设回true了
		newOpts.inJustDecodeBounds = true;
		Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);
		newOpts.inJustDecodeBounds = false;
		int w = newOpts.outWidth;
		int h = newOpts.outHeight;
		// 现在主流手机比较多是800*480分辨率，所以高和宽我们设置为
		float hh = 180f;// 这里设置高度为800f
		float ww = 180f;// 这里设置宽度为480f
		// 缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可
		int be = 1;// be=1表示不缩放
		if (w > h && w > ww) {// 如果宽度大的话根据宽度固定大小缩放
			be = (int) (newOpts.outWidth / ww);
		} else if (w < h && h > hh) {// 如果高度高的话根据宽度固定大小缩放
			be = (int) (newOpts.outHeight / hh);
		}
		if (be <= 0)
			be = 1;
		newOpts.inSampleSize = be;// 设置缩放比例
		// 重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了
		isBm = new ByteArrayInputStream(baos.toByteArray());
		bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);
		return compressImage(bitmap);// 压缩好比例大小后再进行质量压缩
	}

	private Bitmap compressImage(Bitmap image) {

		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		image.compress(Bitmap.CompressFormat.JPEG, 50, baos);// 质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中
		int options = 100;
		while (baos.toByteArray().length / 1024 > 100) { // 循环判断如果压缩后图片是否大于100kb,大于继续压缩
			baos.reset();// 重置baos即清空baos
			image.compress(Bitmap.CompressFormat.JPEG, options, baos);// 这里压缩options%，把压缩后的数据存放到baos中
			options -= 10;// 每次都减少10
		}
		ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());// 把压缩后的数据baos存放到ByteArrayInputStream中
		Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);// 把ByteArrayInputStream数据生成图片
		return bitmap;
	}

	public native void nativeFaceBakcImage(final String filePath);

	public native void wixinApiResp(final int code);

	public native void webViewOnKeyBack(int code);

	public native void WXisNotInstall();

	public native void headImagePath(final String filePath);

	public native void ListenConnectStatus(int status);
}
