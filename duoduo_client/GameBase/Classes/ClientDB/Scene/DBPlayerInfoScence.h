#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPIndividualMission.h"

class DBPlayerInfoScence
	:public cocos2d::Node
	,public IGPIndividualMissionSink
	,public FvSingleton<DBPlayerInfoScence> 
{
public:
	DBPlayerInfoScence();
	~DBPlayerInfoScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void upPlayerInfo();
public:
	virtual void onGPIndividualInfo(int type);
	virtual void onGPIndividualSuccess(int type, const char* szDescription);
	virtual void onGPIndividualFailure(int type, const char* szDescription);
public:
	void Button_PlayerInfoScence(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SetSpreader(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ChangeGender(cocos2d::Ref*,WidgetUserInfo*);
public:
	void ChangePlayerName();
private:
	CGPIndividualMission m_kIndividualMission;
};