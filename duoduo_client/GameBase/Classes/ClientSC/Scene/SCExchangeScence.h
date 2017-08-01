#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPExchangeMission.h"
#include "GameLib/GameNet/MCWebReq.h"

class SCExchangeScence
	:public cocos2d::Node
	,public CGExchangeMissionSink
	,public FvSingleton<SCExchangeScence> 
{
public:
	SCExchangeScence();
	~SCExchangeScence();
public:
	virtual bool init();
public:
	void EnterScence();
public:
	virtual void onGPExchangeHuaFeiListResult(CMD_GP_BackExchangeHuaFei* pNetInfo);
public:
	void Button_SCExchangeItem(cocos2d::Ref*,WidgetUserInfo*);
public:
	void Req_Exchange(int nExchangeIndex);
	void Rsp_Exchange(rapidjson::Document* pDoc);
private:
	CGPExchangeMission m_kExchangeHuaFeiMission;

	CMD_GP_BackExchangeHuaFei m_kExchangeNetInfo;
	
};