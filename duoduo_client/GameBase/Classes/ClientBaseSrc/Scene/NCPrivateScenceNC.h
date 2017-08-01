#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/HNPrivateScenceBase.h"
#include "Kernel/kernel/server/IServerPrivateSink.h"

class NCPrivateScenceNC
	:public HNPrivateScenceBase
	,public FvSingleton<NCPrivateScenceNC> 
{
public:
	NCPrivateScenceNC();
	~NCPrivateScenceNC();
public:
	void hideAll();
	void setPlayerCoutIdex(int iIdex);
	void setGameRuleIdex(int iIdex);
public:
	virtual bool init();
public:

	void Button_Show_Create_Public(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo*);
	
	void Button_CreateRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_JoinPublic(cocos2d::Ref*,WidgetUserInfo*);
	void Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*);
	
	void Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonGameRule(cocos2d::Ref*,WidgetUserInfo*);
	
private:
	int m_iPlayCoutIdex;
	dword m_dwPlayRule;
};