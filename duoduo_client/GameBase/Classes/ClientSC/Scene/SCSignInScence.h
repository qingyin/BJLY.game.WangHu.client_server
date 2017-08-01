#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPSignInMission.h"

class SCSignInScence
	:public cocos2d::Node
	,public IGPSignInMissionSink
	,public FvSingleton<SCSignInScence> 
{
public:
	SCSignInScence();
	~SCSignInScence();
public:
	virtual bool init();
public:
	void HideAll();
	void LogonSucess();
public:
	virtual void onSignInQueryInfoResult(CMD_GP_CheckInInfo* pNetInfo);
	virtual void onSignInDoneResult(CMD_GP_CheckInResult* pNetInfo);
public:
	void Button_SignInAction(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ItemClick(cocos2d::Ref*,WidgetUserInfo*);
private:
	CGPSignInMission m_kSignInMission;
};