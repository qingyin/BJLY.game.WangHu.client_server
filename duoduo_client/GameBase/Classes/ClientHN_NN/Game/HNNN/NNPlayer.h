
#pragma once

#include "Game/Game/GamePlayer.h"
#include "GameLib/Game/GameLib.h"
#include "cocos2d.h"
#include "CMD_Ox.h"


//ÓÎÏ·Âß¼­
class NNPlayer:public GamePlayer
{
public:
	static const int MAX_CARD = 5;
	static const int POSY_DOWN = 0;
	static const int POSY_UP = 10;

public:

	NNPlayer(int iIdex,cocos2d::Node* pSeatNode);
	~NNPlayer();

	void init();
	int  getIdex();
	void defaultState();
	void setGameRecord(bool bRecord);

	void setZhuang();
	void showAddGold(int iGold);
	void showStatusImagic(std::string kKey);
	cocos2d::Node* getPlayerNode();

	void showClock(int iTime);
	void HideClock();
public:
	void showBackCard();
	void showStartCard(BYTE* cbCardData);
	void setHandCard(BYTE* cbCardData,bool bShow);
	void showAllCard();
	void showNiuCard(BYTE cbNiu);
	void upCardTexture(BYTE uCard,cocos2d::Node* pNode);
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
public:
	void showJiaoZhuang(bool bShow);
	void setJiaoZhuangFen(int nFen);
	void sendCardAni(int nValue);
	cocos2d::Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
	static void clickCard(cocos2d::Node* pCardNode);
private:
	int						m_iIdex;
	cocos2d::Node*			m_pSeatNode;
	bool					m_bGameRecord;

	BYTE					m_cbCardData[MAX_COUNT];
};

