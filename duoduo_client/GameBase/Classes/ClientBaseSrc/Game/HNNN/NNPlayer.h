
#pragma once

#include "Game/Game/GamePlayer.h"
#include "GameLib/Game/GameLib.h"
#include "cocos2d.h"
#include "CMD_Ox.h"


//ÓÎÏ·Âß¼­
class NNPlayer:public GamePlayer
{
public:

	NNPlayer(int iIdex,cocos2d::Node* pSeatNode);
	~NNPlayer();

	void init();
	int  getIdex();
	void defaultState();
	void setGameRecord(bool bRecord);

	void setZhuang();
	void showStatusImagic(std::string kKey);
	cocos2d::Node* getPlayerNode();
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
public:
private:
	int						m_iIdex;
	cocos2d::Node*			m_pSeatNode;
	bool					m_bGameRecord;
};

