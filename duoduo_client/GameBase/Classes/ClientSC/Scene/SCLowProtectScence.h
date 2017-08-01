#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLowProtectMission.h"

class SCLowProtectScence
	:public cocos2d::Node
	,public IGPLowProtectMissionLink
	,public FvSingleton<SCLowProtectScence> 
{
public:
	SCLowProtectScence();
	~SCLowProtectScence();
public:
	virtual bool init();
public:
	void LogonSucess();
	void upPlayerInfo();
public:
	virtual void onLowProtectQueryInfoResult(SCORE lScoreCondition, SCORE lScoreAmount, byte cbTakeTimes);
	virtual void onLowProtectDoneResult(bool bSuccessed, SCORE lGameScore, const char* szDescription);
private:
	byte m_cbTakeTimes;
	SCORE m_lScoreCondition;
	CGPLowProtectMission m_kLowProtectMission;
};