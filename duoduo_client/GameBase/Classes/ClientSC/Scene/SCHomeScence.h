#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLoginMission.h"

class SCHomeScence
	:public cocos2d::Node
	,public IServerListDataSink
	,public FvSingleton<SCHomeScence> 
{
public:
	SCHomeScence();
	~SCHomeScence();
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

	virtual void onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr );

public:
	void ShowEffect();
	void EnterScence();
	void upPlayerInfo();
	void upGameSever();
public:
	void init_Button();
	void Button_GameRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BackHome(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ShowMatchSignup(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRoom_Quick(cocos2d::Ref*,WidgetUserInfo*);

	void Button_DuiHuan(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ShangCheng(cocos2d::Ref*,WidgetUserInfo*);
private:
};