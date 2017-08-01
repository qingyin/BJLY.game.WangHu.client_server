#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPublicNoticMission.h"

class DBPublicNoticeScence
	:public cocos2d::Node
	,public CGPublicNoticMissionSink
	,public FvSingleton<DBPublicNoticeScence> 
{
public:
	DBPublicNoticeScence();
	~DBPublicNoticeScence();
public:
	virtual bool init();
public:
	void LogonSucess();
public:
	virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
private:
	CGPublicNoticMission m_kPublicNoticMission;
};