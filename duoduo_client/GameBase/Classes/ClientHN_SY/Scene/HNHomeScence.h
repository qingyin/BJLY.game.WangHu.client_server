#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLoginMission.h"
#include "GameLib/GameNet/MCWebReq.h"
#include "Platform/PFKernel/CGPublicNoticMission.h"
#include "Platform/PFKernel/CGPFaceMission.h"
#include "Platform/PFKernel/CGPIndividualMission.h"

class HNHomeScence
	:public cocos2d::Node
	,public IServerListDataSink
	,public CGPublicNoticMissionSink
	,public IGPIndividualMissionSink
	,public FvSingleton<HNHomeScence> 
{
public:
	HNHomeScence();
	~HNHomeScence();
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
	void EnterScence();
	void upPlayerInfo();
	void LogonSucess();
	void upGameSever();

	void upJoinNumTxt();
public:
	void init_Button();
	void Button_GameRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BackHome(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeHead(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeAddGold(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeExit(cocos2d::Ref*,WidgetUserInfo*);

	void Button_shareFriend(cocos2d::Ref*,WidgetUserInfo*);
	void Button_shareMoment(cocos2d::Ref*,WidgetUserInfo*);
public:
	void NextSpeaker();
	void showSysSpeakerTxt(std::string kTxt,std::string kColor);

	virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);

	virtual void onGPIndividualSuccess(int type, const char* szDescription);
	virtual void onGPIndividualFailure(int type, const char* szDescription);
private:
	std::string m_kActChannel;
	std::vector<std::string> m_kMsgList;
	CGPublicNoticMission m_kPublicNoticMission;
	CGPIndividualMission m_kIndividualMission;
	CGPFaceMission m_kFaceMission;
};