#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGMatchMission.h"
#include "Kernel/kernel/server/IServerMatchSink.h"

class SCMatchScence
	:public cocos2d::Node
	,public ICGMatchMissionSink
	,public IServerMatchSink
	,public FvSingleton<SCMatchScence> 
{
public:
	SCMatchScence();
	~SCMatchScence();
public:
	void sendPayMatchCost(cocos2d::Ref*,WidgetUserInfo*);
	void hideAll();
	void reqMatch(int iServerID);
	void ShowSignupMatch(int iServerID);
public:
	virtual bool init();
public:
	virtual void onSignUpResoult(tagGameMatch&	pGameMatchInfo,bool bSucess,bool bSignUp,const char* pStr );
	virtual void onMatchAwardList(CMD_GR_MatchAwardList* pList);
public:
	virtual void OnSocketSubMatchFee(CMD_GR_Match_Fee* pNetInfo);
	virtual void OnSocketSubMatchNum(CMD_GR_Match_Num* pNetInfo) ;
	virtual void OnSocketSubMatchInfo(CMD_GR_Match_Info* pNetInfo);
	virtual void OnSocketSubMatchWaitTip(bool bShow,CMD_GR_Match_Wait_Tip* pNetInfo);
	virtual void OnSocketSubMatchResult(CMD_GR_MatchResult* pNetInfo);
	virtual void OnSocketSubMatchStatus(byte pNetInfo);
	virtual void OnSocketSubMatchGoldUpdate(CMD_GR_MatchGoldUpdate* pNetInfo);
	virtual void OnSocketSubMatchEliminate();
	virtual void OnSocketSubMatchJoinResolt(bool bSucess);
private:
	void Button_ExitImmediateMatch(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ExitWaiteMatch(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ExitMatchResoult(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ActSignupMatch(cocos2d::Ref*,WidgetUserInfo*);
private:
	int m_iActServerID;
	CGMatchMission m_kMactchMission;
};