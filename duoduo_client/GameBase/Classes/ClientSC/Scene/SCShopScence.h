#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPShopInfoMission.h"

class SCShopScence
	:public cocos2d::Node
	,public CGShopInfoMissionSink
	,public FvSingleton<SCShopScence> 
{
public:
	SCShopScence();
	~SCShopScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void upPlayerInfo();
public:
	virtual void onGPShopInfoListResult(CMD_GP_BackShopInfo* pNetInfo);
public:
	void Button_SCShopItem(cocos2d::Ref*,WidgetUserInfo*);
private:

	CGPShopInfoMission		m_kShopInfoMission;
};