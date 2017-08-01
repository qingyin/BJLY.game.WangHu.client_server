#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Script/TimeNode.h"
#include "Platform/PFKernel/CGPRouletteMission.h"

class NCRouletteScence
	:public cocos2d::Node
	,public ICGPRouletteMissionSink
	,public TimeNode
	,public FvSingleton<NCRouletteScence> 
{
public:
	NCRouletteScence();
	~NCRouletteScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void upDataAction(float fTime);
	void hideAll();
	void StartRouletteIdex(int iIdex);

public:
	void Button_RouletteDone(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ShowRoulette(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo);

	void EndAction();
public:
	virtual void onRouletteUeserInfo(CMD_GP_BackRouletteUeserInfo* pNetInfo);
	virtual void onRouletteDone(CMD_GP_BackRouletteDone* pNetInfo);
private:
	bool				m_bHaveDone;
	CGPRouletteMission	m_kRouletteMission;
	CMD_GP_BackRouletteDone m_kRouletteDoneNetInfo;
};