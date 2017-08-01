#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLoginMission.h"
#include "GameLib/GameNet/MCWebReq.h"
#include "GameLib/Game/Game/MissionWeiXin.h"


class SCLogonScence
	:public cocos2d::Node
	,public IGPLoginMissionSink
	,public IWeiXinMissionSink
	,public FvSingleton<SCLogonScence> 
{
public:
	 static std::string WX_APPID;
	 static std::string WX_APPSECRET;
public:
	SCLogonScence();
	~SCLogonScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void HideAll();
public:
	virtual void onGPLoginSuccess();
	virtual void onGPLoginComplete();
	virtual void onGPLoginFailure(unsigned int iErrorCode,const char* szDescription);
	virtual void onGPError(int err);
public:
	void Button_HomeBack(cocos2d::Ref*,WidgetUserInfo*);
	void Button_LogonAccount(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Visitor(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BindPhoneAction(cocos2d::Ref*,WidgetUserInfo*);

	void Button_GerPhoneAccept(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BindPhone(cocos2d::Ref*,WidgetUserInfo*);
public:
	virtual void onWxLoginSuccess(WxUserInfo kWxUserInfo );
	virtual void onWxLoginFail(std::string kError);
public:
	void Req_PhoneBinding();
	void Rsp_PhoneBinding(rapidjson::Document* pDoc);
	std::string setPhoneCheakCode();
private:
	CGPLoginMission m_kLoginMission;
	std::string m_kPssword;
	std::string m_kPhoneCheakCode;
	std::string m_kPhoneNumber;
};