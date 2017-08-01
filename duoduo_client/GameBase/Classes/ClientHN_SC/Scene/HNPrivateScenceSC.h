#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/HNPrivateScenceBase.h"
#include "Kernel/kernel/server/IServerPrivateSink.h"

class HNPrivateScenceSC
	:public HNPrivateScenceBase
	,public FvSingleton<HNPrivateScenceSC> 
{
public:
	HNPrivateScenceSC();
	~HNPrivateScenceSC();
public:
	void hideAll();
	void setPlayerCoutIdex(int iIdex);
public:
	virtual bool init();
	bool BackKey();
public:

	void Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo*);
	
	void Button_ZZCreateRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_CSCreateRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_XZCreateRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*);

	void Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonGameRule(cocos2d::Ref*,WidgetUserInfo*);
public:
	void setGameRuleIdex(int iIdex);
private:
	int m_iPlayerCoutIdex;
	dword m_dwPlayRule;
};