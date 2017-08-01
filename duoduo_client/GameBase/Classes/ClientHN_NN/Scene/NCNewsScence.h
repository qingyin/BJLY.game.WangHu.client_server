#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPublicNoticMission.h"

class NCNewsScence
	:public cocos2d::Node
	,public CGPublicNoticMissionSink
	,public FvSingleton<NCNewsScence> 
{
public:
	NCNewsScence();
	~NCNewsScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();
	void showNews(int iNews);

public:
	void Button_XinWen(cocos2d::Ref*,WidgetUserInfo*);
	void Button_NewsShow(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo);

public:
	virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
private:
	CGPublicNoticMission m_kPublicNoticMission;
};