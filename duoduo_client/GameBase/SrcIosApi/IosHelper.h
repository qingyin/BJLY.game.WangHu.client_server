//
//  IosHelper.h
//  kkddz
//
//  Created by macbook110 on 13-6-4.
//
//

#ifndef __kkddz__IosHelper__
#define __kkddz__IosHelper__

#include <iostream>
using namespace std;

class  IosHelper {
    
    
public:
    /*INI,暂定为ios native interface*/
    // 震动
    static void VibratorIni(int vibratorTime);  //单位毫秒
    //获得wifi强度
    static int GetWifiSignalIni();    
    // 获得系统时间，返回整形值:小时*60+分钟
    static int GetTimeJNI();
    // 获得Imsi
    static const char *  GetImsiIni();
	
	static void callPhone(const char * pTxt);
    // 获得Imei
    static const char *  GetImeiIni();
    // 获得Os
    static const char *  GetOsIni();
    // 获得DeviceName
    static const char *  GetDeviceNameIni();
    // 设置屏幕亮度
	static void SetFlightIni( int flight );
    // 获得屏幕亮度
	static int GetFlightIni( );
    // 获得程序文件存储的绝对路径
    static const char *  GetKKLandlordAbsolutePathOnExternalStorage(const char * filename);
    // 调用相机或者相册中的图片设置玩家图像
    static void SetIconFirstIni(const char *title, const char *msg);
    static void SetIconSecondIni(const char *title, const char *msg);
    // 用户选择反馈上传图片
    static void uploadFeedBackImage();
    
    static void shareIos(const char * pathName);
    // 判断文件存储位置
    static int GetFileExistPlaceIni(const char *FineName);
    // 版本更新
    static void downUpdateIni(const char * filename , const int filesize, const char * des, const int isUpdate);
    // 支付模块
    static void startPayIni(int waresid, int price,const char *  exorderno);
    // 获得mac地址
    static const char * GetMacIni();
    // 获得来源
    static const char * GetProjectIdIni();
    static void startBrowserJni( const char * url);
    //分享到微信朋友
    static void shareWithWeixinFriend(const char * pTxt);
    //分享到微信朋友圈
    static void shareWithWeixinCircle(const char * pTxt);
    
    static void shareWithWeixinCircle(const char * pTxt,const char *FileName);
    static void sharedComplete();
	
    static void uploadHeadImage(int iType);
    static double getbatteryLevel();
    static void buy(std::string iIdex);
    
    static const char * GetOS();
    static const char * GetFrom();
    static const char * GetAppID();
	static const char * GetOsVersion();
	static const char * GetDeviceName();
	static const char * GetDeviceType();
};

#endif /* defined(__kkddz__IosHelper__) */
