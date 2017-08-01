
#include "BJLPlayer.h"

BJLPlayer::BJLPlayer( IClientUserItem * pIClientUserItem)
	:GamePlayer(pIClientUserItem)
	,m_nSeatID(-1)
	,m_pSeatNode(NULL)
	,m_pGameNode(NULL)
{
}

BJLPlayer::~BJLPlayer()
{

}

int  BJLPlayer::getSeatID()
{
	return m_nSeatID;
}
void BJLPlayer::seatDown(int nID,Node* pGameNode)
{
	m_nSeatID = nID;
	setScenceNode(pGameNode);
	m_pSeatNode = WidgetFun::getChildWidget(pGameNode,utility::toString("BJLPlayer",m_nSeatID));
	ASSERT(m_pSeatNode);

	PlayerEnter();
}
void BJLPlayer::setScenceNode( Node* pGameNode )
{
	m_pGameNode = pGameNode;
}
void BJLPlayer::PlayerEnter()
{
	if(m_pSeatNode)
	{
		m_pSeatNode->setVisible(true);
		WidgetFun::setVisible(m_pSeatNode,"BoxLine",false);
		WidgetFun::setVisible(m_pSeatNode,"LookingCardIcon",false);

		setPlayerInfo();
	}
}

void BJLPlayer::PlayerLeave()
{
	if(m_pSeatNode)
	{
		m_pSeatNode->setVisible(false);
	}
}

void BJLPlayer::upPlayerInfo()
{
	setPlayerInfo();
}

void BJLPlayer::upPlayerState()
{

}

void BJLPlayer::setPlayerInfo()
{
	if (m_pSeatNode==NULL || m_nSeatID < 0 )
	{
		return;
	}
	tagUserInfo* pUserInfo =  GetUserInfo();
	if (!pUserInfo)
	{
		return;
	}

	std::string kNick = GetNickName();
	SCORE  gold = GetUserScore();
	if (gold < 0)
	{
		gold = 0;
	}

	WidgetFun::setText(m_pSeatNode,"NickName",utility::a_u8(kNick));
	WidgetFun::setText(m_pSeatNode,"UserGold",utility::NumToStringMyriabit(gold));

	if (pUserInfo==NULL)
	{
		return;
	}
	if (pUserInfo->dwCustomID == 0 )
	{
		setSysFace();
	}
	else
	{
		setCustomFace();
	}
}

void BJLPlayer::setSysFace()
{
	WidgetFun::setImagic(m_pSeatNode,"HeadImg","Head/Head0.png");
}

void BJLPlayer::setCustomFace()
{

}
cocos2d::Vec2 BJLPlayer::getSeatPos()
{
	if (m_pSeatNode==NULL || m_nSeatID < 0 )
	{
//		无座玩家
		return Vec2();
	}
	else
	{
		return m_pSeatNode->getPosition();
	}
}
cocos2d::Vec2 BJLPlayer::getAreaIconPos()
{
	if (m_pSeatNode==NULL || m_nSeatID < 0 )
	{
//		无座玩家
		return Vec2();
	}
	else
	{
		Vec2 pos =  m_pSeatNode->getPosition();
		return Vec2(pos.x-50,pos.y-50);
	}
}

void BJLPlayer::setWinScore( SCORE nScore )
{
	if (m_pSeatNode==NULL || m_nSeatID<0) return;

	if (nScore>0)
	{
		WidgetFun::setText(m_pSeatNode,"UserWinGold",utility::toString("+",nScore));
	}
	else
	{
		WidgetFun::setText(m_pSeatNode,"UserWinGold",nScore);
	}
	WidgetFun::runWidgetAction(m_pSeatNode,"ResultActionNode","Start");
}

void BJLPlayer::LookingCard(bool bLooking)
{
	if (m_pSeatNode==NULL || m_nSeatID<0) return;

	WidgetFun::setVisible(m_pSeatNode,"BoxLine",bLooking);
	WidgetFun::setVisible(m_pSeatNode,"LookingCardIcon",bLooking);
}

void BJLPlayer::setZhuangIcon(bool bShow)
{
	if (m_pSeatNode==NULL || m_nSeatID<0) return;

	WidgetFun::setVisible(m_pSeatNode,"BJLZhuangIcon",bShow);
}


