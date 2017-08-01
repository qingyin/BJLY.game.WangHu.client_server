#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLoginMission.h"

class DBHomeScence
	:public cocos2d::Node
	,public IServerListDataSink
	,public FvSingleton<DBHomeScence> 
{
public:
	DBHomeScence();
	~DBHomeScence();
public:
	virtual bool init();
public:
	//完成通知
	virtual void OnGameItemFinish();
	//完成通知
	virtual void OnGameKindFinish(uint16 wKindID);
	//更新通知
	virtual void OnGameItemUpdateFinish();

	//更新通知
public:
	//插入通知
	virtual void OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual void OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual void OnGameItemDelete(CGameListItem * pGameListItem);
public:
	void EnterScence();
	void upPlayerInfo();
	void upGameSever();
public:
	void init_Button();
	void Button_EnterGame(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BackHome(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Shop(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BJL(cocos2d::Ref*,WidgetUserInfo*);
	void Button_CAR(cocos2d::Ref*,WidgetUserInfo*);
	void Button_LP(cocos2d::Ref*,WidgetUserInfo*);

	void Button_BJLHall(cocos2d::Ref*,WidgetUserInfo*);

	void Button_HallReturn(cocos2d::Ref*,WidgetUserInfo*);
};