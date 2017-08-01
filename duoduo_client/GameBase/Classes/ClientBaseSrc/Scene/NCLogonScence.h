#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/MissionWeiXin.h"
#include "Platform/PFKernel/CGPLoginMission.h"

class NCLogonScence
	:public cocos2d::Node
	,public IGPLoginMissionSink
	,public IWeiXinMissionSink
	,public FvSingleton<NCLogonScence> 
{
public:
	NCLogonScence();
	~NCLogonScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void RegisterAccount();
public:
	virtual void onGPLoginSuccess();
	virtual void onGPLoginComplete();
	virtual void onGPLoginFailure(unsigned int iErrorCode,const char* szDescription);
	virtual void onGPError(int err);
public:
	virtual void onWxLoginSuccess(WxUserInfo kWxUserInfo );
	virtual void onWxLoginFail(std::string kError);
public:
	void Button_WeiXinLogon(cocos2d::Ref*,WidgetUserInfo*);
	void Button_UserXieYiCheak(cocos2d::Ref*,WidgetUserInfo*);
private:
	CGPLoginMission m_kLoginMission;
	std::string m_kPssword;

	WxUserInfo m_kWeiXinUserInfo;
};