#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPIndividualMission.h"

class SCPlayerInfoScence
	:public cocos2d::Node
	,public IGPIndividualMissionSink
	,public FvSingleton<SCPlayerInfoScence> 
{
public:
	SCPlayerInfoScence();
	~SCPlayerInfoScence();
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
	void Button_ChangePlayerInfo(cocos2d::Ref*,WidgetUserInfo*);
public:
	void ChangePlayerName();
private:
	int		m_iGender;
	CGPIndividualMission m_kIndividualMission;
};