#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPublicNoticMission.h"

class UserScript
	:public CGPublicNoticMissionSink
	,public FvSingleton<UserScript> 
{
public:
	UserScript();
	~UserScript();
public:
	void getUserDefine(std::string kKey);
	virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
private:
	CGPublicNoticMission m_kPublicNoticMission;
};