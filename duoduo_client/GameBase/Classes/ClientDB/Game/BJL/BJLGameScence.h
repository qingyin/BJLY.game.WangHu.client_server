#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include "BJLPlayer.h"


struct BetChip
{
	int nChairID;
	cocos2d::Node* pChip;
};
struct BJLUserBetInfo
{
	int nArea;
	int nScore;
};

class BJLGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<BJLGameScence> 
{
public:
	const static int KIND_ID = 122;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);				//程序版本
	//索引定义
	const static int INDEX_PLAYER		=		0;									//闲家索引
	const static int INDEX_BANKER		=		1;									//庄家索引
	const static int MAX_PLAYER_NUM    =       10;									//坐下玩家个数
	const static int MAX_HISTORY_NUM   =       60;
	const static int HISTORY_ROW_NUM   =       6;	

	const static int GAME_PLAYER   =       100;
	const static int AREA_MAX      =       8;
	const static int SERVER_ROOM_CHUJI   = 1;
	const static int SERVER_ROOM_ZHONGJI = 2;
	const static int SERVER_ROOM_GAOJI   = 3;
public:
	BJLGameScence();
	~BJLGameScence();
public:
	virtual bool init();
	void initPlayerNode();
public:
	virtual bool IsInGame();
	void EnterScence(int EnterScore);
	void setUserRight(dword dwUserRight);
	void defaultState();	
	void setInGameShow(bool bShow);
	void upButtonState();
	void upBetAreaState(bool bEnbale);
	void updateBetButtonInfo(int EnterScore);
	void clearBetChips();
public:
	void showClock(int iTime,bool bShow = true);
	void showClockTimeCallBack(const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime);
	void showClockTimeCallBack(Node* pRoot,const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime);
	void endClockTimeCallBack(const std::string& kName);
	void endClockTimeCallBack(Node* pRoot,const std::string& kName);

	int getAreaScore(int iIdex);
	void MoveGold(bool bNeedMove,int iArea,int iBetScore,Vec2 posFrom,int wChairID);
	void setTimeDesc(int nState);
	void statusTip(int nState);

	void setShowCardsNum(bool bShow,int nArea,int nNum = 0);
	void setCardImge(cocos2d::Node* pCardNode,int nValue,int nColor);
	void setCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3]);
	void sendCardBack(int nArea,int nCardIndex,bool bHaveAni = true);
	void showCardBack(int nArea,int nCardIndex,bool bShow);
	void resultAni();

	// 中途进桌
	int getChipIndexByScore(int nScore);
	void setAreaScore(int nArea,int nAllBet,int nSelfBet,bool bRest = false,bool bAdd = false);
	void setAreaBetMax(word wXianChairID,word wZhuangChairID,longlong wXianBet,longlong wZhuangBet);
	void showAreaBetMax(int nArea,bool bShow);
	void moveAreaIconToPlayer(int nArea);

	void OnOpenCard(int nOpenIndex,int nChairID);
	void openActionByCardIndex(int nArea,int nCardIndex,const std::string& kActionName);
	void openActionByOpenIndex(int nOpenIndex,const std::string& kActionName);
	void openCardTimeOver(int nOpenIndex,int nChairID);
	void showPointByOpenEnd(int nOpenIndex);
	int  getAreaPoint(int nArea,int nCardNum);//获取牌点数 nCardNum = 1，2或者3  前几张的点数
	void set3CardPoint(int nArea,int nPoint);

	//记录
	void initHistory();
	void addHistoryWinArea(int nAreaWin);
	void refreshHistory();

	void BJL_TurnCard(cocos2d::Ref*,WidgetUserInfo*);
	void BJL_SubCard(cocos2d::Ref*,WidgetUserInfo*);
	void touchOpenCardCallBack(int nIndex,int nTouchType,cocos2d::Vec2 touchPos);
	bool autoOpenCard(int nOpenIndex);

	void turnCard1End();
	void turnCard2End();

	void setZhuang(int nChairID);
public:
	void initButton();
	void BJLButton_Zhu(cocos2d::Ref*,WidgetUserInfo*);
	void BJHButton_Bet(cocos2d::Ref*,WidgetUserInfo*);
	void BJL_OpenCard(cocos2d::Ref*,WidgetUserInfo*);
	void BJHButton_Control(cocos2d::Ref*,WidgetUserInfo*);

	void BJLButton_ZhuangList(cocos2d::Ref*,WidgetUserInfo*);
	void BJLButton_Shangzhuang(cocos2d::Ref*,WidgetUserInfo*);
	void BJLButton_QuXiaoZhuang(cocos2d::Ref*,WidgetUserInfo*);

	void BJLButton_CancleBet(cocos2d::Ref*,WidgetUserInfo*);
	void BJLButton_LastBet(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
public:
	virtual void upSelfPlayerInfo();
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//系统滚动消息
	virtual bool OnGFTableMessage(const char* szMessage);
public:
	void OnSubGameStart(const void * pBuffer, word wDataSize);
	void OnSubGameFree(const void * pBuffer, word wDataSize);
	void OnSubPlaceBet(const void * pBuffer, word wDataSize);
	void OnSubGameEnd(const void * pBuffer, word wDataSize);
	void OnSubGameOpenCard(const void * pBuffer, word wDataSize);
	void OnSubTuchTurnCard(const void * pBuffer, word wDataSize);
	void OnSubGameRecord(const void * pBuffer, word wDataSize);
	void OnSubPlaceBetFail(const void * pBuffer, word wDataSize);
	void OnSubLeftCout(const void * pBuffer, word wDataSize);
	void OnSubBankerList(const void * pBuffer, word wDataSize);
	void OnSubApplyBanker(const void * pBuffer, word wDataSize);
	void OnSubCancleBanker(const void * pBuffer, word wDataSize);
	void OnSubChangeBanker(const void * pBuffer, word wDataSize);
	void OnSubCancleBet(const void * pBuffer, word wDataSize);
public:
	void initNet();
public:
	void Command_PlaceBet(int iArea,int iBetScore);
	void Command_ControlArea(int iArea);
protected:
	int								m_iActSelectGoldButton;
	LONGLONG*						m_lPlayBet;				//玩家下注
	SCORE                           m_nWinScore;
	int                             m_nWinArea;			   //赢区域
	SCORE                           m_nAllWinScore[GAME_PLAYER];
	std::list<int>                  m_nHistoryList;
	int                            m_nNowOpenIndex;
	int                            m_nOpenChairID;
	cocos2d::Vec2                   m_pTuchPosTemp;

	std::vector<BetChip>             m_pBetChipList;
	std::vector<BJLUserBetInfo>  m_kLastBetList;		//上次押注数据
	std::vector<BJLUserBetInfo>  m_kLastBetListTemp;//上次押注数据
};