#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"

class CarGameScence
	:public GameBase
	,public FvSingleton<CarGameScence> 
{
public:
	const static int KIND_ID = 108;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int MAX_CAR_NUM = 24;
	const static int UNIT_BET[4];
	const static int S_AREA_COUNT = 8;
public:
	CarGameScence();
	~CarGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();
	void defaultInit();
public:
	void EnterScence();
	void updateAction(float dt);
private:
	void initData();
	void hideAllCarItem();
	void showCarItem(int nIndex,bool bAni = true);
	void blinkItem(int nIndex);
	void carLogoAni();
	void showClockTime(int nStatus,int nSecond);
	void showClockTimeCallBack(int nStatus,int nSecond,const std::function<void()>& pCallBack,float fCheakTime);
	void setClockDesc(int nStatus);
	void setSelfBet(int nIndex,SCORE nScore);
	void setAllBet(int nIndex,SCORE nScore);
	void setUnitBet(SCORE nScore);
	void setUserScore(SCORE nScore);
	void setWinScore(SCORE nScore);

	void setUserNickeName(const std::string& username);
	void setUserKeYong(SCORE nScore);
	void setUserResoult(SCORE nScore);

	void addRecordIndex(int nIndex);
	void setGameHistory();
	void oldItemMove(bool bMove);
	void newItemMove(int nNewItemIndex,bool bMoveByX);

	void updateBetBtnStatus();
	void setBetBtnEnble(bool bEnble);
	void clearBetInfo();
	void atuoBet();
	void daoShuAni();
private:
	void initButton();
	void CarBtn_Bet1(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Bet2(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Bet3(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Bet4(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Bet5(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Bet6(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Bet7(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Bet8(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Switch(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_ReBet(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_AutoBet(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void CarBtn_Return(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
public:
	void Command_UserBet(int nIndex,SCORE nScore);
private:
	virtual void upSelfPlayerInfo();
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
public:
	void initNet();
	void onSubGameFree(const void* pBuffer,word wDataSize);
	void onSubGameStart(const void* pBuffer,word wDataSize);
	void onSubGameJetton(const void* pBuffer,word wDataSize);
	void onSubGameEnd(const void* pBuffer,word wDataSize);
	void onSubGameRecord(const void* pBuffer,word wDataSize);
	void onGameStatusFree(const void* pBuffer,word wDataSize);
	void onGameStatusPlay(const void* pBuffer,word wDataSize);
private:
	void run_start();
	void run_end();
private:
	std::vector<cocos2d::Node*> m_pCarNodeList;
	std::vector<cocos2d::Node*> m_pCarRecordList;
	std::list<int>				m_nRecordIndexList;
	int                         m_nStartIndex;
	int                         m_nEndIndex;
	SCORE                       m_arrySelfBetScore[S_AREA_COUNT+1];		//自己下注分数
	SCORE                       m_arrySelfBetScore_Temp[S_AREA_COUNT+1];//自己下注分数
	SCORE                       m_arryAllBetScore[S_AREA_COUNT+1];		//所有下注分数
	SCORE                       m_nUnitBet;								//单注

	int                         m_nGameStatus;                          //游戏状态

	SCORE						m_nUserScore;
	SCORE                       m_nWinScore;
	SCORE						m_nKeYongScore;
};