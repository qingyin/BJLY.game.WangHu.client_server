#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPExchangeMission.h"
#include "GameLib/GameNet/MCWebReq.h"

class HNExchangeScence
	:public cocos2d::Node
	,public CGExchangeMissionSink
	,public FvSingleton<HNExchangeScence> 
{
public:
	HNExchangeScence();
	~HNExchangeScence();
public:
	virtual bool init();
public:
	virtual void onGPExchangeHuaFeiListResult(CMD_GP_BackExchangeHuaFei* pNetInfo);
	virtual void onGPExchangeDoneResult(bool bSucess,const char* pChar);
public:
	void HNButton_ShowExchange(cocos2d::Ref*,WidgetUserInfo*);
	void HNButton_ExchangeItem(cocos2d::Ref*,WidgetUserInfo*);
private:
	CGPExchangeMission m_kExchangeHuaFeiMission;

	CMD_GP_BackExchangeHuaFei m_kExchangeNetInfo;
};