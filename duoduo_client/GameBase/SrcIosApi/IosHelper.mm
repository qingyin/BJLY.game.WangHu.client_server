//
//  IosHelper.cpp
//  kkddz
//
//  Created by macbook110 on 13-6-4.
//
//

#include "IosHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <UIKit/UIDevice.h>
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#import "QPImagePickerController.h"
#import "SSKeychain.h"
#import "WXApi.h"
#import "WXApiObject.h"
#import "IAPAPI.h"
#endif//

void IosHelper::shareWithWeixinCircle(const char * pTxt){
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = txt;
    message.description = txt;
    message.messageExt = txt;
    message.messageAction = txt;
    
    [message setThumbImage:[UIImage imageNamed:@"weixin_share_icon.png"]];
    
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = @"http://www.xq5.com";
    
    message.mediaObject = ext;
    
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message; 
    req.scene = WXSceneTimeline;
    
    [WXApi sendReq:req];
}
void IosHelper::shareWithWeixinCircle(const char * pTxt,const char *FileName)
{
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    NSString *filePath = [NSString stringWithCString:FileName encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    
    message.title = txt;
    message.description = txt;
    [message setThumbImage:[UIImage imageNamed:@"weixin_share_icon.png"]];
    
    WXImageObject *ext = [WXImageObject object];
    ext.imageData = [NSData dataWithContentsOfFile:filePath];
    
    
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneTimeline;
    
    [WXApi sendReq:req];
}
void IosHelper::shareWithWeixinFriend(const char * pTxt){
    
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    WXAppExtendObject *AppData = [WXAppExtendObject object];
    AppData.extInfo = txt;
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = txt;
    message.description = txt;
    [message setThumbImage:[UIImage imageNamed:@"weixin_share_icon.png"]];
    
    
    message.mediaObject = AppData;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneSession;
    
    [WXApi sendReq:req];
}

void IosHelper::VibratorIni(int vibratorTime)  //单位毫秒
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}
void IosHelper::callPhone(const char * pTxt)  
{
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
	UIWebView *callPhoneWebVw = [[UIWebView alloc] init];  
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:txt]];  
    [callPhoneWebVw loadRequest:request]; 
}
 
//获得wifi强度
int IosHelper::GetWifiSignalIni()
{
    return 1;
}
// 获得系统时间，返回整形值:小时*60+分钟
int IosHelper::GetTimeJNI()
{
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"MM-dd-HH-mm-ss"];
    NSString*locationString=[formatter stringFromDate: [NSDate date]];
    NSArray*timeArray=[locationString componentsSeparatedByString:@"-"];
    float value_h=  [ [timeArray objectAtIndex:2]floatValue];
    float value_m= [ [timeArray objectAtIndex:3]floatValue];
    //    float value_All=value_D*24*60*60+value_h*60*60+value_m*60+value_s;
    int value_All=value_h*60+value_m;
    [formatter release];
    return value_All;
}

// 获得Imsi
const char *  IosHelper::GetImsiIni()
{
//    NSString *appName=[[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)kCFBundleNameKey];
    
    NSString *strApplicationUUID = [SSKeychain passwordForService:@"gdy_offline" account:@"incoding"];
    if (strApplicationUUID == nil)
    {
        if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
        {
           // strApplicationUUID  = [[UIDevice currentDevice] uniqueIdentifier];
            //[SSKeychain setPassword:strApplicationUUID forService:@"gdy_offline" account:@"incoding"];
           // ;
            srand((unsigned int)time(0));
            int iValue = rand()%1000000000;
            strApplicationUUID  = [NSString stringWithFormat:@"%d",iValue];
            [SSKeychain setPassword:strApplicationUUID forService:@"niuniu" account:@"incoding"];
        }
        else
        {
            strApplicationUUID  = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
            [SSKeychain setPassword:strApplicationUUID forService:@"gdy_offline" account:@"incoding"];
        }
    }
    
    return [strApplicationUUID UTF8String];
}

// 获得Imei
const char *  IosHelper::GetImeiIni()
{
    return "";
}
// 获得Os
const char *  IosHelper::GetOsIni()
{
return "Ios";
}
// 获得DeviceName
const char *  IosHelper::GetDeviceNameIni()
{
    NSString *username = [[UIDevice currentDevice] name];
    return [username cStringUsingEncoding:NSUTF8StringEncoding];
}
// 设置屏幕亮度
void IosHelper::SetFlightIni( int flight )
{
    [[UIScreen mainScreen] setBrightness: flight];
}
// 获得屏幕亮度
int IosHelper::GetFlightIni( )
{
    return [[UIScreen mainScreen] brightness];
}
// 获得程序文件存储的绝对路径
const char *  IosHelper::GetKKLandlordAbsolutePathOnExternalStorage(const char * filename) 
{
    NSLog(@"111111filename=%s",filename);
    NSString *_filename = [NSString stringWithFormat:@"%s",filename];
//    NSString *fillPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:_filename];
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *fillPath = [documentsDirectory stringByAppendingPathComponent:_filename];
    if (![[NSFileManager defaultManager] fileExistsAtPath:fillPath]) {
        NSArray *paths= NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [[paths objectAtIndex:0] stringByAppendingPathComponent:_filename];
        [[NSFileManager defaultManager] createDirectoryAtPath:documentsDirectory withIntermediateDirectories:YES attributes:nil error:nil];
    }
    fillPath = [fillPath stringByAppendingString:@"\/"];
    NSLog(@"11111[fillPath cStringUsingEncoding:NSUTF8StringEncoding]=%s",[fillPath cStringUsingEncoding:NSUTF8StringEncoding]);
    return [fillPath cStringUsingEncoding:NSUTF8StringEncoding];
    
}
// 调用相机或者相册中的图片设置玩家图像
void IosHelper::SetIconFirstIni(const char *title, const char *msg)
{
    //ImagePickerController *imagepickercontroller = [[ImagePickerController alloc] init];
    
   // [imagepickercontroller chooseImage];
    
}
void IosHelper::SetIconSecondIni(const char *title, const char *msg)
{}
// 判断文件存储位置
int IosHelper::GetFileExistPlaceIni(const char *FineName)
{

    return 0;
}
// 版本更新
void IosHelper::downUpdateIni(const char * filename , const int filesize, const char * des, const int isUpdate)
{}
// 支付模块
void IosHelper::startPayIni(int waresid, int price,const char *  exorderno)
{}
// 获得mac地址
const char * IosHelper::GetMacIni()
{
    int                 mgmtInfoBase[6];
    char                *msgBuffer = NULL;
    size_t              length;
    unsigned char       macAddress[6];
    struct if_msghdr    *interfaceMsgStruct;
    struct sockaddr_dl  *socketStruct;
    NSString            *errorFlag = NULL;
    
    // Setup the management Information Base (mib)
    mgmtInfoBase[0] = CTL_NET;        // Request network subsystem
    mgmtInfoBase[1] = AF_ROUTE;       // Routing table info
    mgmtInfoBase[2] = 0;
    mgmtInfoBase[3] = AF_LINK;        // Request link layer information
    mgmtInfoBase[4] = NET_RT_IFLIST;  // Request all configured interfaces
    
    // With all configured interfaces requested, get handle index
    if ((mgmtInfoBase[5] = if_nametoindex("en0")) == 0)
        errorFlag = @"if_nametoindex failure";
    else
    {
        // Get the size of the data available (store in len)
        if (sysctl(mgmtInfoBase, 6, NULL, &length, NULL, 0) < 0)
            errorFlag = @"sysctl mgmtInfoBase failure";
        else
        {
            // Alloc memory based on above call
            msgBuffer =(char*) malloc(length) ;
            if ((msgBuffer) == NULL)
                errorFlag = @"buffer allocation failure";
            else
            {
                // Get system information, store in buffer
                if (sysctl(mgmtInfoBase, 6, msgBuffer, &length, NULL, 0) < 0)
                    errorFlag = @"sysctl msgBuffer failure";
            }
        }
    }
    
    // Befor going any further...
    if (errorFlag != NULL)
    {
        //  DLog(@"Error: %@", errorFlag);
        return [errorFlag cStringUsingEncoding:NSUTF8StringEncoding];
//        return errorFlag;
    }
    
    // Map msgbuffer to interface message structure
    interfaceMsgStruct = (struct if_msghdr *) msgBuffer;
    
    // Map to link-level socket structure
    socketStruct = (struct sockaddr_dl *) (interfaceMsgStruct + 1);
    
    // Copy link layer address data in socket structure to an array
    memcpy(&macAddress, socketStruct->sdl_data + socketStruct->sdl_nlen, 6);
    
    // Read from char array into a string object, into traditional Mac address format
    NSString *macAddressString = [NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X",
                                  macAddress[0], macAddress[1], macAddress[2],
                                  macAddress[3], macAddress[4], macAddress[5]];
    // DLog(@"Mac Address: %@", macAddressString);
    
    // Release the buffer memory
    free(msgBuffer);
    NSLog(@"macAddressString=%@",macAddressString);
//    return macAddressString;
    return [macAddressString cStringUsingEncoding:NSUTF8StringEncoding];

}
// 获得来源
const char * IosHelper::GetProjectIdIni()
{
    return "";
}
void IosHelper::startBrowserJni( const char * url)
{
    NSString *nsFineName = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:nsFineName]];
}

void IosHelper::uploadFeedBackImage(){
    [QPImagePickerController selectImage];
    
}
void IosHelper::shareIos(const char * pathName)
{
    NSString *nsFineName = [NSString stringWithCString:pathName encoding:NSUTF8StringEncoding];
    NSURL *url = [NSURL fileURLWithPath:nsFineName];
    NSArray *objectsToShare = @[url];
    
    
    UIActivityViewController *controller = [[UIActivityViewController alloc] initWithActivityItems:objectsToShare applicationActivities:nil];
    
    UIWindow *window = [UIApplication sharedApplication].keyWindow;
    UIViewController *rootViewController = window.rootViewController;
    [rootViewController presentViewController:controller animated:YES completion:nil];
    
}
void IosHelper::uploadHeadImage(int iType)
{
    if (iType == 1) {
        [QPImagePickerController upHeadImagic2];
    }
    else
    {
        
        [QPImagePickerController upHeadImagic1];
    }
}

double IosHelper::getbatteryLevel()
{
     [[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];
    return [UIDevice currentDevice].batteryLevel;
}
void IosHelper::buy(std::string iIdex)
{
    NSString *_filename = [NSString stringWithFormat:@"%s",iIdex.c_str()];
    IAPAPI *iapapi = [IAPAPI getShareIAPAPI];
    [iapapi buy:_filename];
}
const char * IosHelper::GetOS()
{
    UIDevice *device_=[[UIDevice alloc] init];
    return [device_.model cStringUsingEncoding:NSUTF8StringEncoding];
}
const char * IosHelper::GetFrom()
{
    return IosHelper::GetFrom();
}
const char * IosHelper::GetAppID()
{
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleIdentifierKey];
    return [version cStringUsingEncoding:NSUTF8StringEncoding];
}
 const char * IosHelper::GetOsVersion()
{
    UIDevice *device_=[[UIDevice alloc] init];
    return [device_.systemVersion cStringUsingEncoding:NSUTF8StringEncoding];
}
 const char * IosHelper::GetDeviceName()
{
    UIDevice *device_=[[UIDevice alloc] init];
    return [device_.name cStringUsingEncoding:NSUTF8StringEncoding];
}
 const char * IosHelper::GetDeviceType()
{
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char*)malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];    free(machine);
    return [platform cStringUsingEncoding:NSUTF8StringEncoding];
}
