#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLoginMission.h"

class DBLogonScence
	:public cocos2d::Node
	,public IGPLoginMissionSink
	,public FvSingleton<DBLogonScence> 
{
public:
	DBLogonScence();
	~DBLogonScence();
public:
	virtual bool init();
public:
	virtual void onGPLoginSuccess();
	virtual void onGPLoginComplete();
	virtual void onGPLoginFailure(unsigned int iErrorCode,const char* szDescription);
	virtual void onGPError(int err);
public:
	void EnterScence();
	void Button_Login(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Visitor(cocos2d::Ref*,WidgetUserInfo*);
private:
	std::string m_kPssword;
	CGPLoginMission m_kLoginMission;
};