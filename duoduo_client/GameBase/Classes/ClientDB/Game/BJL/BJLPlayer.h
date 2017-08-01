
#pragma once

#include "Game/Game/GamePlayer.h"
#include "cocos2d.h"
#include "GameLib/Game/GameLib.h"
USING_NS_CC;

class BJLPlayer:public GamePlayer
{
public:
	BJLPlayer(IClientUserItem * pIClientUserItem);
	~BJLPlayer();
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
public:
	int  getSeatID();
	void seatDown(int nID,Node* pGameNode);

	void setScenceNode(Node* pGameNode);
	void setPlayerInfo();
	void setSysFace();
	void setCustomFace();
	Vec2 getSeatPos();
	Vec2 getAreaIconPos();
	void setWinScore(SCORE nScore);
	void LookingCard(bool bLooking);
	void setZhuangIcon(bool bShow);
private:
	int				m_nSeatID;
	Node*			m_pSeatNode;
	Node*			m_pGameNode;
};

