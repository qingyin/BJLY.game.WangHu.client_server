#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include "Game/Script/TimeManager.h"
#include "Game/Script/TimeNode.h"

class YaCell;
class YaChip;
struct UserBetInfo
{
	BYTE          cbBetArea;										//下注区域
	QYLONG        lBetScore;										//下注分数
	float         kPosX;											//筹码位置
	float         kPosY;											//筹码位置
};
typedef std::vector<YaChip*> ChipList;

typedef std::map<word,ChipList> BetChipMap;

class LPGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<LPGameScence> 
{
public:
	const static int KIND_ID = 404;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int MAX_CELL_NUM      =50;	
	const static int MAX_HISTORY	    = 13;
	
public:
	LPGameScence();
	~LPGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();
	void initPlayerNode();
public:
	void EnterScence();
	void defaultState();	
public:
	void showClock(int iTime,bool bShow = true);
	void showClockTimeCallBack(const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime);

	
public:
	void initButton();
	void BJLButton_Zhu(cocos2d::Ref*,WidgetUserInfo*);
	void LPButton_History(cocos2d::Ref*,WidgetUserInfo*);
	void LPButton_CloseHistory(cocos2d::Ref*,WidgetUserInfo*);
	void LPButton_LastBet(cocos2d::Ref*,WidgetUserInfo*);
	void LPButton_ResetAll(cocos2d::Ref*,WidgetUserInfo*);
	void LPButton_Reset(cocos2d::Ref*,WidgetUserInfo*);
	void LPButton_Rule(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnBetScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
public:
	virtual void upSelfPlayerInfo();
public:
	void OnSubGameStart(const void * pBuffer, word wDataSize);
	void OnSubGameEnd(const void * pBuffer, word wDataSize);
	void OnSubUserBet(const void * pBuffer, word wDataSize);
	void OnSubUserCancleBet(const void* pBuffer,word wDataSize);
public:
	void initNet();
public:
	void Command_PlaceBet(int iArea,int iBetScore,float x,float y);
	void Command_CancleBet(int nType);
public:
	void initTouch();
	void initCell();
	void showCell(std::vector<int>& kIndexList,bool bAni);
	void showCheck(bool bShow);

	bool checkCellShadow(const cocos2d::Vec2& kPos,const cocos2d::Size& kSize);
	int checkTypeByYaCell();
public:
	void showCellByNumList( std::vector<int>& kNumList);
	int CheckYa_1(const std::vector<YaCell*>& pCellList );
	int CheckYa_2(const std::vector<YaCell*>& pCellList );
	int CheckYa_3(const std::vector<YaCell*>& pCellList );
	int CheckYa_6(const std::vector<YaCell*>& pCellList );
	int CheckYa_4(const std::vector<YaCell*>& pCellList );
	void CheckEndPos(const cocos2d::Vec2& pos);
	void addChipInList( YaChip*& pChip );
	void commpletYa(cocos2d::Vec2 pos);

	void wheel_run(int nEndNum);
	void runEnd();
	void playTimeOver1();
	void playTimeOver2();

	void cleanYaChips(ChipList& kChipsList);
	void cleanYaChips(BetChipMap& kChipMap);
	void blinkCell(int nEndNum);
	void endChipsAni(int nEndNum);
	void endChipAniOther(int nEndNum);
	void chip_move(Node* pChip,const cocos2d::Vec2& pos,int nIndex);

	void saveEndNum(int nEndNum);
	void updateHistotry();

	void userCancleBet(word wChairID,int nCancleType);
	void recycleYaChips(ChipList& kChipsList,bool isSelf);
	void recycleYaChip(YaChip* pChip,bool isSelf,int nIndex = 0);

	void betLastOnce();
	bool setWinScore(LONGLONG lWinScore);
	bool setUserScore(LONGLONG lScore);
public:
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch,cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
protected:
	bool                  m_bDrag;
	int					  m_nChipsType;
	YaChip*               m_pCurrentChip;
	YaChip*               m_pCurrentChipTemp;
	ChipList				 m_pYaChipList;

	BetChipMap			 m_kBetChipMap;     //玩家下注筹码

	std::vector<YaCell*>  m_pYaCellList;     //下注格子
	std::vector<YaCell*>  m_pTouchCellList;

	int                   m_pCurrentYaType;
	int                   m_nGameState;
	int                   m_nEndNum;
	LONGLONG              m_lWinScore;
	LONGLONG              m_lTotalScore;

	std::list<int>		m_kHistoryList;

	std::vector<UserBetInfo>  m_kLastBetList;//上次押注数据
	std::vector<UserBetInfo>  m_kLastBetListTemp;//上次押注数据
};