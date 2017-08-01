/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

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
package com.xunbao.slot;

import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cocos2dx.lib.CRC;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.content.Intent;
import android.content.Intent.ShortcutIconResource;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.preference.PreferenceManager;
import android.provider.MediaStore;
import android.telephony.SmsManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.xunbao.slot.util.KkZip;
import com.xunbao.slot.util.URLUtil;

public class stare extends Cocos2dxActivity {

	ProgressDialog pd;
	private int downLoadFileSize;
	InputStream is = null;
	FileOutputStream fos = null;
	private int size;
	String filename = "";
	private static int installFlag = 1;
	public static final int NONE = 0;
	public static final int PHOTOHRAPH = 1;// 拍照
	public static final int PHOTOZOOM = 2; // 缩放
	public static final int PHOTORESOULT = 3;// 结果
	public static final int BROWSER = 4;
	public static final String IMAGE_UNSPECIFIED = "image/*";
	private static String tempPath = "";
	private static int photoType = 0;
	private static Uri uri;
	private boolean isComplete = false;
	private static String version = "";
	PowerManager powerManager = null;
	WakeLock wakeLock = null;
	public static final String APP_ID = "wxc1050af059d672b6";
	private static final int THUMB_SIZE = 150;
	private static final String SDCARD_ROOT = Environment
			.getExternalStorageDirectory().getAbsolutePath();
	private static final int TIMELINE_SUPPORTED_VERSION = 0x21020001;
//	private SDKManager sdkManager;
	private Dialog resDialog;
	public static Map<String, String> resourceMap = new HashMap<String, String>();
	private String ipAddr = "";
	private int isDownGameResource;
	private String srcUrl ;
	private String srcPath ;
	private String resId;	
	private String resOldId;
	private String downPath;
	

	private static String SMS_SEND_ACTIOIN = "SMS_SEND_ACTIOIN";
	private static String SMS_DELIVERED_ACTION = "SMS_DELIVERED_ACTION";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		tempPath = Cocos2dxHelper.getAbsolutePathOnExternalStorage(
				this.getApplicationInfo(), "temp/");
		this.powerManager = (PowerManager) this
				.getSystemService(Context.POWER_SERVICE);
		this.wakeLock = this.powerManager.newWakeLock(
				PowerManager.FULL_WAKE_LOCK, "My Lock");
		version = android.os.Build.VERSION.SDK;
		checkShortCut();
//		MobclickAgent.setDebugMode(true);
		System.out.println(getDeviceInfo(this));
	}

	static {
		System.loadLibrary("game");
	}

	private Handler payHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			/*switch (msg.what) {
			case SDKManager.WHAT_PAY_CALLBACK_DEFAULT:
				String json = (String) msg.obj;
				System.out.println("json --> " + json);
				try {
					JSONObject pay = new JSONObject(json);
					int resultCode = pay.getInt("resultCode");
					String desc = pay.getString("desc");
					double money = pay.getDouble("money");
					System.out.println("resultCode = " + resultCode);
					System.out.println("desc = " + desc);
					System.out.println("money = " + money);
				} catch (JSONException e) {
					e.printStackTrace();
				}
				break;

			case SDKManager.WHAT_ACTIVATE_CALLBACK_DEFAULT:
				String json1 = (String) msg.obj;
				System.out.println("json --> " + json1);
				try {
					JSONObject pay = new JSONObject(json1);
					int resultCode = pay.getInt("resultCode");
					String desc = pay.getString("desc");
					double money = pay.getDouble("money");
					System.out.println("resultCode = " + resultCode);
					System.out.println("desc = " + desc);
					System.out.println("money = " + money);
				} catch (JSONException e) {
					e.printStackTrace();
				}
				break;
			case SDKManager.WHAT_BUY_CALLBACK_DEFAULT:
				String json2 = (String) msg.obj;
				System.out.println("json --> " + json2);
				try {
					JSONObject pay = new JSONObject(json2);
					int resultCode = pay.getInt("resultCode");
					String desc = pay.getString("desc");
					double money = pay.getDouble("money");
					System.out.println("resultCode = " + resultCode);
					System.out.println("desc = " + desc);
					System.out.println("money = " + money);
					if (resultCode == 0) {
						Cocos2dxHelper.ExitBuy(0, "buy success");
					} else if (resultCode == -1) {
						Cocos2dxHelper.ExitBuy(0, "buy fail");
					} else if (resultCode == -2) {
						Cocos2dxHelper.ExitBuy(0, "buy cancel");
					}
				} catch (JSONException e) {
					e.printStackTrace();
				}
				break;
			default:
				break;
			}*/
		}
	};

	private Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			if (!Thread.currentThread().isInterrupted()) {
				switch (msg.what) {
				case 1:
					pd.setProgress(downLoadFileSize / 1024);
					break;
				case 2:
					if(isDownGameResource==1) {	
						try {
							File file = new File(downPath + "/"
									+ resOldId);
							file.mkdirs();
							KkZip.UnZipFolder(downPath+"/"+ resOldId+"/"+resId+".zip", downPath+"/"+ resOldId+"/");
							File zipFile = new File(downPath+"/"+ resOldId+"/"+resId+".zip");
							if(zipFile.exists()) {
								zipFile.delete();
							}
							File newFile = new File(downPath+"/"+resId);
							file.renameTo(newFile);										
							Cocos2dxHelper.ExitCheckResources(1,resId);
							
						} catch (Exception e) {		
							Log.e("landlord", "error: "
									+ e.getMessage(),
									e);
						}
						pd.dismiss();						
					} else {
						if (installFlag == 1) {
							Toast.makeText(stare.this, "文件下载完成", 1).show();
							String absPath = Environment
									.getExternalStorageDirectory()
									+ "/Android/data/"
									+ getApplicationInfo().packageName + "/files/";
							openFile(new File(absPath + filename));
						}
					}					
					break;
				case 3:
					Toast.makeText(stare.this, "请检查wifi网络是否开启!", 1).show();
					break;
				case -1:
					Toast.makeText(stare.this, "网络异常，文件下载失败!", 1).show();
					// Cocos2dxHelper.ExitUpdate(0);
					break;
				case 4:
					AlertDialog.Builder builder = new Builder(stare.this);
					final int kSize = msg.getData().getInt("size");
					String info = msg.getData().getString("info");
					int isUpdate = msg.getData().getInt("isUpdate");
					final String url = msg.getData().getString("url");
					builder.setMessage(info+"请在wifi环境下下载...");
					builder.setTitle("更新提示");
					builder.setPositiveButton("确定",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									dialog.dismiss();
									pd = new ProgressDialog(stare.this);
									pd.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
									pd.setTitle("提示");
									if (checkNetWorkIsAvailable()) {
										pd.setMessage("游戏文件下载中，请等待");
									} else {
										pd.setMessage("您当前未使用wif网络,游戏文件下载中，请等待");
									}
									pd.setProgress(100);
									pd.setIndeterminate(false);
									pd.setCancelable(false);
									pd.setMax(kSize);
									pd.show();
									pd.setOnKeyListener(new OnKeyListener() {
										@Override
										public boolean onKey(
												DialogInterface dialog,
												int keyCode, KeyEvent event) {
											if (keyCode == KeyEvent.KEYCODE_BACK) {
												installFlag = 0;
												try {
													if (fos != null) {
														fos.close();
													}
													if (is != null) {
														is.close();
													}
												} catch (Exception ex) {
													Log.e("landlord", "error: "
															+ ex.getMessage(),
															ex);
												}
												Toast.makeText(stare.this,
														"文件下载已取消", 1).show();
												pd.dismiss();
											}
											return false;
										}
									});
									new Thread() {
										@Override
										public void run() {
											String absPath = Environment
													.getExternalStorageDirectory()
													+ "/Android/data/"
													+ getApplicationInfo().packageName
													+ "/files/";
											File file = new File(absPath);
											file.mkdirs();
											try {
												downFile(url, absPath);
											} catch (IOException e) {
												e.printStackTrace();
											}
										}
									}.start();
								}
							});
					if (isUpdate == 0) {
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
					break;
				case 5:
					String waresid = msg.getData().getString("waresid");
					String exorderno = msg.getData().getString("exorderno");
					int price = msg.getData().getInt("price");
					String spId = msg.getData().getString("spId");
					String productName = msg.getData().getString("productName");

			/*		sdkManager.buy(exorderno, payHandler,
							SDKManager.WHAT_BUY_CALLBACK_DEFAULT, price,
							productName, SDKManager.TYPE_GAME);*/
					break;
				case 6:
					int brightness = msg.getData().getInt("number");
					if (brightness > 0 && brightness < 255) {
						try {

							WindowManager.LayoutParams lp = getWindow()
									.getAttributes();
							lp.screenBrightness = Float.valueOf(brightness)
									* (1f / 255f);
							getWindow().setAttributes(lp);
						} catch (Exception e) {
							Log.e("landlord", "set brightness error");
						}
					}
					break;
				case 7:
					new AlertDialog.Builder(stare.this)
							.setTitle("设置头像...")
							.setNegativeButton("取消",
									new DialogInterface.OnClickListener() {
										public void onClick(
												DialogInterface dialog,
												int which) {
											photoType = NONE;
											dialog.dismiss();
										}
									})
							.setNeutralButton("相册",
									new DialogInterface.OnClickListener() {
										public void onClick(
												DialogInterface dialog,
												int which) {
											dialog.dismiss();
											Intent intent = new Intent(
													Intent.ACTION_GET_CONTENT,
													null);
											intent.setDataAndType(
													MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
													IMAGE_UNSPECIFIED);
											photoType = PHOTOZOOM;
											startActivityForResult(intent,
													PHOTOZOOM);
										}
									})
							.setPositiveButton("拍照",
									new DialogInterface.OnClickListener() {
										public void onClick(
												DialogInterface dialog,
												int whichButton) {
											dialog.dismiss();
											Intent intent = new Intent(
													MediaStore.ACTION_IMAGE_CAPTURE);
											File tempDir = new File(tempPath);
											tempDir.mkdirs();
											intent.putExtra(
													MediaStore.EXTRA_OUTPUT,
													Uri.fromFile(new File(
															tempPath
																	+ "kkavatartemp.jpg")));
											intent.putExtra(
													MediaStore.EXTRA_SCREEN_ORIENTATION,
													ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
											photoType = PHOTOHRAPH;
											startActivityForResult(intent,
													PHOTOHRAPH);

										}
									}).show();
					break;
				case 8:
					if (photoType == PHOTOHRAPH || photoType == PHOTOZOOM) {
						if (uri != null) {
							startPhotoZoom(uri);
						}
					} else {
						Cocos2dxHelper.ExitSetIconSecond("");
					}
					break;
				case 9:
					//a/b/c
					srcUrl = msg.getData().getString("url");//下载URL
					size = msg.getData().getInt("size");//文件大小
					int isShow = msg.getData().getInt("isShow");//
					srcPath = msg.getData().getString("srcPath");//下载资源路径
					resId = srcPath.substring(srcPath.lastIndexOf("/")+1);
					resOldId  = resId+System.currentTimeMillis();
					downPath = srcPath.substring(0,srcPath.lastIndexOf("/"));
					if(isShow==1) {
						isDownGameResource=1;
						pd = new ProgressDialog(stare.this);
						pd.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
						pd.setTitle("提示");
						if (checkNetWorkIsAvailable()) {
							pd.setMessage("游戏文件下载中，请等待");
						} else {
							pd.setMessage("您当前未使用wif网络,游戏文件下载中，请等待");
						}						
						pd.setProgress(100);
						pd.setIndeterminate(false);
						pd.setCancelable(true);
						pd.setMax(size/1024);
						pd.show();
						pd.setOnKeyListener(new OnKeyListener() {
							@Override
							public boolean onKey(
									DialogInterface dialog,
									int keyCode, KeyEvent event) {
								if (keyCode == KeyEvent.KEYCODE_BACK) {
									installFlag = 0;
									try {
										if (fos != null) {
											fos.close();
										}
										if (is != null) {
											is.close();
										}
									} catch (Exception ex) {
										Log.e("landlord", "error: "
												+ ex.getMessage(),
												ex);
									}
									Toast.makeText(stare.this,
											"文件下载已取消", 1).show();
									pd.dismiss();
									Cocos2dxHelper.ExitCheckResources(0,resId);

								}
								return false;
							}
						});
						new Thread() {
							@Override
							public void run() {								
								try {
									//下载URL及下载路径
									File file = new File(downPath);
									file.mkdirs();
									file = new File(downPath+"/"+resOldId);
									file.mkdirs();
									downFile(srcUrl, downPath+"/"+resOldId+"/");
								} catch (IOException e) {
									e.printStackTrace();
									pd.dismiss();
									Cocos2dxHelper.ExitCheckResources(0,resId);
								}
							}
						}.start();
						
					} else {
						if(resourceMap.get(srcPath)==null) {
							resourceMap.put(srcPath,resId);
							boolean sdCardExist = Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
							if (!sdCardExist) {
								android.os.Process.killProcess(android.os.Process.myPid());
								System.exit(0);
							} else {
								File file = new File(downPath);
								file.mkdirs();
								new Thread() {
									@Override
									public void run() {
										File file = new File(downPath+"/"+resOldId);
										file.mkdirs();
										try {
											downSourceFile(srcUrl, downPath+"/"+resOldId,resId);
											KkZip.UnZipFolder(downPath+"/"+ resOldId+"/"+resId+".zip", downPath+"/"+ resOldId+"/");
											File zipFile = new File(downPath+"/"+ resOldId+"/"+resId+".zip");
											if(zipFile.exists()) {
												zipFile.delete();
											}
											File newFile = new File(downPath+"/"+resId);
											file.renameTo(newFile);										
											Cocos2dxHelper.ExitCheckResources(1,resId);
										} catch (Exception e) {
											Log.e("tag", "error: " + e.getMessage(), e);
											//finish();
											//System.exit(0);
										}
									}
								}.start();
							}
						}
					}					
					break;
				case 10:
					if (isComplete == false) {
						pd = new ProgressDialog(stare.this);
						pd.setProgressStyle(ProgressDialog.STYLE_SPINNER);
						pd.setTitle("提示");
						pd.setMessage("游戏资源文件加载中，请等待");
						pd.setIndeterminate(false);
						pd.setCancelable(false);
						pd.show();
						pd.setOnKeyListener(new OnKeyListener() {
							@Override
							public boolean onKey(DialogInterface dialog,
									int keyCode, KeyEvent event) {
								if (keyCode == KeyEvent.KEYCODE_BACK) {
									installFlag = 0;
									try {
										if (fos != null) {
											fos.close();
										}
										if (is != null) {
											is.close();
										}
									} catch (Exception ex) {
										Log.e("tag",
												"error: " + ex.getMessage(), ex);
									}
									Toast.makeText(stare.this, "文件下载已取消", 1)
											.show();
									pd.dismiss();
								}
								finish();
								System.exit(0);
								return false;
							}
						});
					} else {
						pd.dismiss();
					}
					break;
				case 11:
					// browser;
				/*	String browserUrl = msg.getData().getString("text");
					Intent intent = new Intent(stare.this,
							BrowserActivity.class);
					Bundle bundle = new Bundle();
					bundle.putString("url", browserUrl);
					intent.putExtras(bundle);
					startActivityForResult(intent, BROWSER);*/
					/**
					if(isAvilible(landlord.this, "com.ifeng.news2")){ 
						String s = "http://api.3g.ifeng.com/ipadtestdoc?imgwidth=100&aid=%s&channel=&chid=&android=1";
						String type = "0";
		                if(type=="0"){ //   当数据中articletype等于"0"时
		                    Intent intent = new Intent();
		                    ComponentName cn = new ComponentName("com.ifeng.news2", "com.ifeng.news2.activity.DetailActivity");        
		                    intent.setComponent(cn);
		                    intent.putExtra("extra.com.ifeng.news2.id", String.format(s, "79167560")); // 此处为数据中的id字段
		                    intent.setAction("action.com.ifeng.news2.push");
		                    startActivity(intent);
		                }else if(type=="1"){ //   当数据中articletype等于"1"时
		                    Intent intent = new Intent();
		                    ComponentName cn = new ComponentName("com.ifeng.news2", "com.ifeng.news2.activity.SlideActivity");         
		                    intent.setComponent(cn);
		                    intent.putExtra("extra.com.ifeng.news.url", String.format(s, "79167560"));// 此处为数据中的id字段
		                    intent.putExtra("extra.com.ifeng.news.position", 0);
		                    intent.setAction("action.com.ifeng.news2.from_head_image");
		                    startActivity(intent);
		                }
					} else {
						
					}
					*/
					break;
				case 12:
					break;
				case 13:
					/*browserUrl = msg.getData().getString("text");
					Intent browserintent = new Intent();
					browserintent.setAction("android.intent.action.VIEW");
					Uri content_url = Uri.parse(browserUrl);
					browserintent.setData(content_url);
					startActivity(browserintent);
					finish();*/
					break;
				case 14:
					/*String isBottom=msg.getData().getString("text");
					LayoutInflater inflater = LayoutInflater.from(stare.this);
					View v = inflater.inflate(R.layout.dialogview, null);
					LinearLayout layout = (LinearLayout) v.findViewById(R.id.dialog_view);
					// main.xml中的ImageView
					ImageView spaceshipImage = (ImageView) v.findViewById(R.id.img);
					// 加载动画
					Animation hyperspaceJumpAnimation = AnimationUtils.loadAnimation(stare.this,R.anim.animation);
					// 使用ImageView显示动画
					spaceshipImage.startAnimation(hyperspaceJumpAnimation);
					resDialog = new Dialog(stare.this,R.style.FullHeightDialog);
					resDialog.setCancelable(false);				
					resDialog.setContentView(layout);
					resDialog.show();	
					if("B".equals(isBottom)) {						
						resDialog.getWindow().setGravity(Gravity.BOTTOM);
					}	*/
					break;
				case 15:
					//resDialog.dismiss();
					break;
				case 18:
					if (isNetworkConnected(stare.this)
							&& isNetCanGetData(stare.this)) {
						Cocos2dxHelper.endDetectionNetwork(1,"http://"+ipAddr+":9001");
					} else {
						Dialog alertDialog = new AlertDialog.Builder(
								stare.this)
								.setTitle("提示")
								.setMessage("您的网络连接异常，请检查网络后重试!")
								.setIcon(R.drawable.icon)
								.setPositiveButton("确定",
										new DialogInterface.OnClickListener() {
											@Override
											public void onClick(
													DialogInterface dialog,
													int which) {
												dialog.dismiss();
												android.os.Process
														.killProcess(android.os.Process
																.myPid());
												System.exit(0);
											}
										}).create();
						alertDialog.show();
					}
					break;
				case 20:
					String event = msg.getData().getString("title");
					String params = msg.getData().getString("text");
					HashMap<String, String> paramsMap = new HashMap<String, String>();
					paramsMap.put(params, params);
					
	/*				if("start".equals(event)) {
						if("login".equals(params)) {
							MobclickAgent.onEventBegin(stare.this,event);
						} else {
							MobclickAgent.onEventEnd(stare.this,event);
						}
					} else {
						MobclickAgent.onEvent(stare.this,event,params);
					}*/
					break;
				case 21:
					finish();
					new Thread(new Runnable() {
						public void run() {
							try {
								Thread.sleep(1000);
							} catch (InterruptedException e) {
								
							}
							android.os.Process.killProcess(android.os.Process.myPid());
							System.exit(0);
						}
					}).start();
					break;
				case 100:

					final String kPhoneNum = msg.getData().getString("title");
					final String kText = msg.getData().getString("text");
					PendingIntent pi = PendingIntent.getActivity(stare.this, 0, new Intent(stare.this, stare.class), 0);
			        SmsManager sms = SmsManager.getDefault();
			        sms.sendTextMessage(kPhoneNum, null, kText, pi, null);
			        break;
				case 101:
					{
						final String kTitle = msg.getData().getString("title");
						final String kTxt = msg.getData().getString("text");
						Dialog kDialog = new AlertDialog.Builder(stare.this)
								.setTitle(kTitle)
								.setMessage(kTxt)
								.setIcon(android.R.drawable.ic_dialog_info)
								.setPositiveButton("确  定",null).create();
						kDialog.show();
					}
				 	break;
				}
			}
			super.handleMessage(msg);
		}
	};

	@Override
	public void downUpdate(final String url, final String info, final int size,
			final int isUpdate) {
		Log.v("url","url:"+url);
		Log.v("kSize","kSize0:"+size);
		new Thread() {
			@Override
			public void run() {
				sendMsg(4, url, size, info, isUpdate);
			}
		}.start();
	}

	private void sendMsg(int flag, String url, int size, String info,
			int isUpdate) {
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putInt("size", size);
		bundle.putString("url", url);
		bundle.putInt("isUpdate", isUpdate);
		bundle.putString("info", info);
		msg.setData(bundle);
		handler.sendMessage(msg);
	}
	

	private void sendMsg(int flag, String exorderno, String productName,
			String waresid, int price, String spId, String notifyUrl,String userId) {
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putString("exorderno", exorderno);
		bundle.putString("waresid", waresid);
		bundle.putString("productName", productName);
		bundle.putInt("price", price);
		bundle.putString("spId", spId);
		bundle.putString("notifyUrl", notifyUrl);
		bundle.putString("userId", userId);
		msg.setData(bundle);
		handler.sendMessage(msg);
	}

	private void sendMsg(int flag, String url, final String path, final int size) {
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putInt("size", size);
		bundle.putString("url", url);
		bundle.putString("srcPath", path);
		msg.setData(bundle);
		handler.sendMessage(msg);
	}
	

	private void sendMsg(int flag, String url, String srcPath, int size,
			int isShow) {
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putInt("size", size);
		bundle.putInt("isShow", isShow);
		bundle.putString("url", url);
		bundle.putString("srcPath", srcPath);
		msg.setData(bundle);
		handler.sendMessage(msg);		
	}

	private void sendMsg(int flag, int number) {
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putInt("number", number);
		msg.setData(bundle);
		handler.sendMessage(msg);
	}

	private void sendMsg(int flag) {
		Message msg = new Message();
		msg.what = flag;
		handler.sendMessage(msg);
	}

	private void sendMsg(int flag, String text) {
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putString("text", text);
		msg.setData(bundle);
		handler.sendMessage(msg);
	}

	private void sendMsg(int flag, String title, String text) {
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putString("title", title);
		bundle.putString("text", text);
		msg.setData(bundle);
		handler.sendMessage(msg);
	}

	// 下载文件
	public void downFile(String url, String path) throws IOException {
		filename = url.substring(url.lastIndexOf("/") + 1);
		// 获取文件名
		URL myURL = new URL(url);
		URLConnection conn = myURL.openConnection();
		conn.connect();
		is = conn.getInputStream();
		final File file = new File(path + filename);
		if (is == null)
			throw new RuntimeException("stream is null");
		fos = new FileOutputStream(file);
		// 把数据存入路径+文件名
		byte buf[] = new byte[512];
		downLoadFileSize = 0;
		sendMsg(0);
		do {
			int numread = is.read(buf);
			if (numread == -1) {
				break;
			}
			fos.write(buf, 0, numread);
			downLoadFileSize += numread;
			sendMsg(1);// 更新进度条
		} while (true);
		sendMsg(2);// 通知下载完成
		try {
			fos.close();
			is.close();
		} catch (Exception ex) {
			Log.e("tag", "error: " + ex.getMessage(), ex);
		}
	}
	
	// 下载文件
	public void downGameSourceFile(String url, String path,String srcName) throws IOException {
		URL myURL = new URL(url);
		URLConnection conn = myURL.openConnection();
		conn.connect();
		is = conn.getInputStream();
		final File file = new File(path + "/" + srcName+".zip");
		if (is == null)
			throw new RuntimeException("stream is null");
		fos = new FileOutputStream(file);
		// 把数据存入路径+文件名
		byte buf[] = new byte[512];
		downLoadFileSize = 0;
		sendMsg(0);
		do {
			int numread = is.read(buf);
			if (numread == -1) {
				break;
			}
			fos.write(buf, 0, numread);
		} while (true);
		sendMsg(2);// 通知下载完成
		try {
			fos.close();
			is.close();
		} catch (Exception ex) {
			Log.e("landlord", "error: " + ex.getMessage(), ex);
		}
	}

	// 打开下载文件安装
	private void openFile(File file) {
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.fromFile(file),
				"application/vnd.android.package-archive");
		startActivity(intent);
		this.finish();
	}

	// 检查wifi
	public boolean checkNetWorkIsAvailable() {
		ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo netInfo = cm.getActiveNetworkInfo();
		if (null == netInfo || !netInfo.isAvailable()) {
			return false;
		} else {
			return true;
		}
	}

	@Override
	public void setBrightness(final int brightness) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(6, brightness);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}

	@Override
	public void startPay(final String exorderno, final String waresid,
			final int price, final String productName, final String spId,
			final String notifyUrl,final String userId) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(5, exorderno, productName, waresid, price, spId,
							notifyUrl,userId);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}
	
	@Override
	public void exitApp() {

		sendMsg(21);
	}
	
	@Override
	public void startEvent(final String event, final String params) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(20, event, params);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}
	

	@Override
	public void startDetectionNetwork() {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(18);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}
	
	@Override
	public void startFeedBack() {
		System.out.println("startFeedBack test ok");
	}
	
	@Override
	public void getLoadingNotice() {
		Cocos2dxHelper.exitGetLoadingNotice("这是一个Android版本的登录公告测试文字");
	}
	
	@Override
	public void startThirdLogin() {
		
	}	

	@Override
	public void startLoadRes(final String text) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(14, text);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
		
	}
	
	@Override
	public void endLoadRes() {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(15);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
		
	}

	@Override
	public void startBrowser(final String url) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(11, url);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}

	@Override
	public void startBrowserSys(final String url) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(13, url);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}

	@Override
	public void startShare(final String imagePath,final String title) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(12, title,imagePath);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}

	@Override
	public void checkResource(final String url, final String srcPath,
			final int size,final int isShow) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(9, url, srcPath, size,isShow);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}

	

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
		// 如果是直接从相册获取
		case 1:
			File tempDir = new File(tempPath);
			tempDir.mkdirs();
			File temp = new File(tempPath + "kkavatartemp.jpg");
			uri = Uri.fromFile(temp);
			if (Integer.parseInt(version) < 10) {
				smallVersionSetIcon(temp);
			} else {
				Cocos2dxHelper.ExitSetIconFirst("");
			}
			break;
		// 如果是调用相机拍照时
		case 2:
			if (data != null) {
				uri = data.getData();
				if (Integer.parseInt(version) < 10) {
					// 上传文件
					Uri uri = data.getData();
					String[] proj = { MediaStore.Images.Media.DATA };
					Cursor actualimagecursor = managedQuery(uri, proj, null,
							null, null);
					int actual_image_column_index = actualimagecursor
							.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
					actualimagecursor.moveToFirst();
					String img_path = actualimagecursor
							.getString(actual_image_column_index);
					File icon = new File(img_path);
					smallVersionSetIcon(icon);
				} else {
					Cocos2dxHelper.ExitSetIconFirst("");
				}
			}
			break;
		// 取得裁剪后的图片
		case 3:
			/**
			 * 非空判断大家一定要验证，如果不验证的话， 在剪裁之后如果发现不满意，要重新裁剪，丢弃
			 * 当前功能时，会报NullException，小马只 在这个地方加下，大家可以根据不同情况在合适的 地方做判断处理类似情况
			 * 
			 */
			if (data != null) {
				// setPicToView(data);
				Bundle extras = data.getExtras();
				if (extras != null) {
					Bitmap photo = extras.getParcelable("data");
					ByteArrayOutputStream stream = new ByteArrayOutputStream();
					photo = Bitmap.createScaledBitmap(photo, 150, 150, false);
					photo.compress(Bitmap.CompressFormat.PNG, 50, stream);
					photo.recycle();
					byte[] b = stream.toByteArray();
					try {
						stream.close();
					} catch (IOException e2) {
						e2.printStackTrace();
					}
					String newName = CRC.getCrc(b) + "";// + ".png";
					// 将图片流以字符串形式存储下来
					if (uploadFile(b, newName)) {
						// String tp = new String(Base64Coder.encodeLines(b));
						/*
						 * 这个地方大家可以写下给服务器上传图片的实现，直接把tp直接上传就可以了，
						 * 服务器处理的方法是服务器那边的事了，吼吼
						 */
						// getAbsolutePathOnExternalStorage(this.getApplicationInfo(),
						// "images");
						
						String filePath = Cocos2dxHelper
								.getAbsolutePathOnExternalStorage(
										this.getApplicationInfo(),
										"iconplayer/");
						
						File iconPath = new File(filePath);
						iconPath.mkdirs();
						BufferedOutputStream bos = null;
						FileOutputStream fos = null;
						File file = null;
						try {
							File dir = new File(filePath);
							file = new File(filePath + "/" + newName);
							fos = new FileOutputStream(file);
							bos = new BufferedOutputStream(fos);
							bos.write(b);
						} catch (Exception e) {
							System.out.println("save file:" + e);
						} finally {
							if (bos != null) {
								try {
									bos.close();
								} catch (IOException e1) {
									e1.printStackTrace();
								}
							}
							if (fos != null) {
								try {
									fos.close();
								} catch (IOException e1) {
									e1.printStackTrace();
								}
							}

						}
						Cocos2dxHelper.ExitSetIconSecond(newName);//
					} else {
						Cocos2dxHelper.ExitSetIconSecond("");//
					}
				} else {
					Cocos2dxHelper.ExitSetIconSecond("");
				}
			}
			break;
		case 4:
			Cocos2dxHelper.ExitBrowser(1, "browser exit");
			break;
		default:
			Cocos2dxHelper.ExitSetIconSecond("");
			break;

		}
		super.onActivityResult(requestCode, resultCode, data);
	}

	/* 上传文件至Server的方法 */
	private boolean uploadFile(byte[] tp, String newName) {
		String end = "\r\n";
		String twoHyphens = "--";
		String boundary = "*****";
		try {
			String actionUrl = getString(R.string.action_url);
			// String actionUrl = "";
			URL url = new URL(actionUrl);
			HttpURLConnection con = (HttpURLConnection) url.openConnection();
			/* 允许Input、Output，不使用Cache */
			con.setDoInput(true);
			con.setDoOutput(true);
			con.setUseCaches(false);
			/* 设置传送的method=POST */
			con.setRequestMethod("POST");
			/* setRequestProperty */
			con.setRequestProperty("Connection", "Keep-Alive");
			con.setRequestProperty("Charset", "UTF-8");
			con.setRequestProperty("Content-Type",
					"multipart/form-data;boundary=" + boundary);
			/* 设置DataOutputStream */
			DataOutputStream ds = new DataOutputStream(con.getOutputStream());
			ds.writeBytes(twoHyphens + boundary + end);

			ds.writeBytes("Content-Disposition: form-data; "
					+ "name=\"upload\";filename=\"" + newName + "\"" + end);
			ds.writeBytes(end);
			ds.write(tp);
			ds.writeBytes(end);
			ds.writeBytes(twoHyphens + boundary + twoHyphens + end);
			/* close streams */

			ds.flush();
			/* 关闭DataOutputStream */
			ds.close();
			/* 取得Response内容 */
			InputStream is = con.getInputStream();
			int ch;
			StringBuffer b = new StringBuffer();
			while ((ch = is.read()) != -1) {
				b.append((char) ch);
			}

			return true;
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
			return false;
		}
	}

	/**
	 * 裁剪图片方法实现
	 * 
	 * @param uri
	 */
	public void startPhotoZoom(Uri uri) {
		/*
		 * 至于下面这个Intent的ACTION是怎么知道的，大家可以看下自己路径下的如下网页
		 * yourself_sdk_path/docs/reference/android/content/Intent.html
		 * 直接在里面Ctrl+F搜：CROP ，之前小马没仔细看过，其实安卓系统早已经有自带图片裁剪功能, 是直接调本地库的，小马不懂C C++
		 * 这个不做详细了解去了，有轮子就用轮子，不再研究轮子是怎么 制做的了...吼吼
		 */
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, "image/*");
		// 下面这个crop=true是设置在开启的Intent中设置显示的VIEW可裁剪
		intent.putExtra("crop", "true");
		// aspectX aspectY 是宽高的比例
		intent.putExtra("aspectX", 1);
		intent.putExtra("aspectY", 1);
		// outputX outputY 是裁剪图片宽高
		intent.putExtra("outputX", 150);
		intent.putExtra("outputY", 150);
		intent.putExtra("return-data", true);
		startActivityForResult(intent, 3);
	}

	@Override
	public void SetIconDialogFirst(final String title, final String text) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(7, title, text);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "set icon dialog first Error");
		}
	}

	@Override
	public void SetIconDialogSecond(final String title, final String text) {
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(8, title, text);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "set icon dialog second Error");
		}
	}

	public void smallVersionSetIcon(File temp) {
		// 上传文件
		Bitmap photo = null;
		try {
			photo = BitmapFactory.decodeStream(new FileInputStream(temp));
		} catch (FileNotFoundException e3) {
			e3.printStackTrace();
		}
		ByteArrayOutputStream stream = new ByteArrayOutputStream();
		photo = Bitmap.createScaledBitmap(photo, 150, 150, false);
		photo.compress(Bitmap.CompressFormat.PNG, 50, stream);
		photo.recycle();
		byte[] b = stream.toByteArray();
		try {
			stream.close();
		} catch (IOException e2) {
			e2.printStackTrace();
		}
		String newName = CRC.getCrc(b) + "";// + ".png";
		// 将图片流以字符串形式存储下来
		if (uploadFile(b, newName)) {
			// String tp = new String(Base64Coder.encodeLines(b));
			/*
			 * 这个地方大家可以写下给服务器上传图片的实现，直接把tp直接上传就可以了， 服务器处理的方法是服务器那边的事了，吼吼
			 */
			// getAbsolutePathOnExternalStorage(this.getApplicationInfo(),
			// "images");
			String filePath = Cocos2dxHelper.getAbsolutePathOnExternalStorage(
					this.getApplicationInfo(), "iconplayer/");
			BufferedOutputStream bos = null;
			FileOutputStream fos = null;
			File file = null;
			try {
				File dir = new File(filePath);
				dir.mkdirs();
				file = new File(filePath + "/" + newName);
				fos = new FileOutputStream(file);
				bos = new BufferedOutputStream(fos);
				bos.write(b);
			} catch (Exception e) {
				System.out.println("save file:" + e);
			} finally {
				if (bos != null) {
					try {
						bos.close();
					} catch (IOException e1) {
						Log.e("landlord", "error: " + e1.getMessage(), e1);
					}
				}
				if (fos != null) {
					try {
						fos.close();
					} catch (IOException e1) {
						Log.e("landlord", "error: " + e1.getMessage(), e1);
					}
				}

			}
			Cocos2dxHelper.ExitSetIconSecond(newName);//
		} else {
			Cocos2dxHelper.ExitSetIconSecond("");//
		}
	}

	@Override
	protected void onResume() {
		 if(getRequestedOrientation()!=ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
			  setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
			 }
		super.onResume();
		this.wakeLock.acquire();
	//	MobclickAgent.onResume(this);
	}

	@Override
	protected void onPause() {
		super.onPause();
		this.wakeLock.release();
	//	MobclickAgent.onPause(this);
	}

	/**
	 * <br>
	 * 为程序创建桌面快捷方式 </br>
	 */
	private void addShortcut() {
		if (IsShortCutExist()) {
			return;
		}
		Intent shortcut = new Intent(
				"com.android.launcher.action.INSTALL_SHORTCUT");
		// 快捷方式的名称
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_NAME,
				getString(R.string.app_name));
		shortcut.putExtra("duplicate", false); // 不允许重复创建

		Intent shortcutIntent = new Intent(Intent.ACTION_MAIN);
		shortcutIntent.setClassName(this, this.getClass().getName());
		shortcutIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
		// 快捷方式的图标
		ShortcutIconResource iconRes = Intent.ShortcutIconResource.fromContext(
				this, R.drawable.icon);
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, iconRes);
		sendBroadcast(shortcut);
	}

	/**
	 * <br>
	 * 快捷方式是否存在</br>
	 * 
	 * @return
	 */
	public boolean IsShortCutExist() {
		boolean isInstallShortcut = false;
		final ContentResolver cr = this.getContentResolver();
		final String AUTHORITY = "com.android.launcher2.settings";
		final Uri CONTENT_URI = Uri.parse("content://" + AUTHORITY
				+ "/favorites?notify=true");
		Cursor c = cr.query(CONTENT_URI,
				new String[] { "title", "iconResource" }, "title=?",
				new String[] { getString(R.string.app_name) }, null);
		if (c != null && c.getCount() > 0) {
			isInstallShortcut = true;
		}
		return isInstallShortcut;
	}

	private void delShortcut() {
		Intent shortcut = new Intent(
				"com.android.launcher.action.UNINSTALL_SHORTCUT");
		// 快捷方式的名称
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_NAME,
				getString(R.string.app_name));
		Intent shortcutIntent = new Intent(Intent.ACTION_MAIN);
		shortcutIntent.setClassName(this, this.getClass().getName());
		shortcutIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
		sendBroadcast(shortcut);
	}

	public void checkShortCut() {
		SharedPreferences sp = PreferenceManager
				.getDefaultSharedPreferences(this);
		// 是否在桌面上添加了快捷方式
		int oldVersionCode = sp.getInt("VersionCode", 0);
		int versionCode = getVersionCode();
		Editor editor = sp.edit();
		editor.putInt("VersionCode", versionCode);
		editor.commit();
		if (versionCode > oldVersionCode) {
			delShortcut();
			addShortcut();
		}
	}

	// 下载文件
	public void downSourceFile(String url, String path,String srcName) throws IOException {
		URL myURL = new URL(url);
		URLConnection conn = myURL.openConnection();
		conn.connect();
		InputStream is = conn.getInputStream();
		final File file = new File(path + "/" + srcName+".zip");
		if (is == null)
			throw new RuntimeException("stream is null");
		FileOutputStream fos = new FileOutputStream(file);
		// 把数据存入路径+文件名
		byte buf[] = new byte[512];
		do {
			int numread = is.read(buf);
			if (numread == -1) {
				break;
			}
			fos.write(buf, 0, numread);
		} while (true);
		try {
			fos.close();
			is.close();
		} catch (Exception ex) {
			Log.e("landlord", "error: " + ex.getMessage(), ex);
		}
	}

	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis())
				: type + System.currentTimeMillis();
	}
	

	public boolean isNetworkConnected(Context context) {
		if (context != null) {
			ConnectivityManager mConnectivityManager = (ConnectivityManager) context
					.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo mNetworkInfo = mConnectivityManager
					.getActiveNetworkInfo();
			if (mNetworkInfo != null) {
				return mNetworkInfo.isAvailable() && mNetworkInfo.isConnected();
			}
		}
		return false;
	}

	public boolean isNetCanGetData(Context context) {
		boolean flag = false;
		if (context != null) {
			String actionUrl = getString(R.string.check_url);
			try {
				String result = URLUtil.sendGet(actionUrl, "");
				if (result!=null&&result.contains("true")) {
					flag = true;					
					ipAddr = result.substring(result.indexOf(":")+1);
					System.out.println(ipAddr);
				} else {
					flag = false;
				}
			} catch (Exception e) {
				Log.e("landlord", e.toString());
				flag = false;
			}
		}
		return flag;
	}
	
	private boolean isAvilible(Context context, String packageName){ 
        final PackageManager packageManager = context.getPackageManager();//获取packagemanager 
        List<PackageInfo> pinfo = packageManager.getInstalledPackages(0);//获取所有已安装程序的包信息 
        List<String> pName = new ArrayList<String>();//用于存储所有已安装程序的包名 
       //从pinfo中将包名字逐一取出，压入pName list中 
            if(pinfo != null){ 
            for(int i = 0; i < pinfo.size(); i++){ 
                String pn = pinfo.get(i).packageName; 
                pName.add(pn); 
            } 
        } 
        return pName.contains(packageName);//判断pName中是否有目标程序的包名，有TRUE，没有FALSE 
  } 
	
	public static String getDeviceInfo(Context context) {
	    try{
	      org.json.JSONObject json = new org.json.JSONObject();
	      android.telephony.TelephonyManager tm = (android.telephony.TelephonyManager) context
	          .getSystemService(Context.TELEPHONY_SERVICE);
	  
	      String device_id = tm.getDeviceId();
	      
	      android.net.wifi.WifiManager wifi = (android.net.wifi.WifiManager) context.getSystemService(Context.WIFI_SERVICE);
	          
	      String mac = wifi.getConnectionInfo().getMacAddress();
	      json.put("mac", mac);
	      
	      if( TextUtils.isEmpty(device_id) ){
	        device_id = mac;
	      }
	      
	      if( TextUtils.isEmpty(device_id) ){
	        device_id = android.provider.Settings.Secure.getString(context.getContentResolver(),android.provider.Settings.Secure.ANDROID_ID);
	      }
	      
	      json.put("device_id", device_id);
	      
	      return json.toString();
	    }catch(Exception e){
	      e.printStackTrace();
	    }
	  return null;
	}
	public  void SendPhoneMsg(final String kPhoneNum,final String kText)
	{
		Log.v("sendMsg", "sendPhoneMsg:"+kPhoneNum+"##"+kText);
			SmsManager smsManager = SmsManager.getDefault();  
			PendingIntent sentIntent = PendingIntent.getBroadcast(this, 0, new Intent(), 0);
			List<String> divideContents = smsManager.divideMessage(kText);
			for (String text : divideContents) {    
			    smsManager.sendTextMessage(kPhoneNum, null, text, sentIntent, null);     
			}  
	}
	public  void ShowAndroidDlg( final String kTitle,final String kText )
	{
		try {
			new Thread() {
				@Override
				public void run() {
					sendMsg(101, kTitle,kText);
				}
			}.start();
		} catch (Exception e) {
			Log.e("landlord", "error: " + e.getMessage(), e);
		}
	}
   
}
