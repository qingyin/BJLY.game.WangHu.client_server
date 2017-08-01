#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPBeginnerMission.h"

class SCBeginnerScence
	:public cocos2d::Node
	,public IGPBeginnerMissionSink
	,public FvSingleton<SCBeginnerScence> 
{
public:
	SCBeginnerScence();
	~SCBeginnerScence();
public:
	virtual bool init();
public:
	void HideAll();
	void LogonSucess();
public:
	virtual void onBeginnerQueryInfoResult(CMD_GP_BeginnerInfo* pNetInfo);
	virtual void onBeginnerDoneResult(CMD_GP_BeginnerResult* pNetInfo);
public:
	void Button_BeginnerScence(cocos2d::Ref*,WidgetUserInfo*);
	void BegiinerButton_Award(cocos2d::Ref*,WidgetUserInfo*);
private:
	CGPBeginnerMission m_kBeginnerMission;
};