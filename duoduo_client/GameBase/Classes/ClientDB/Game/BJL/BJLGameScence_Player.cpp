#include "BJLGameScence.h"
#include "Game/GameLib.h"

GamePlayer* BJLGameScence::CreatePlayer( IClientUserItem * pIClientUserItem )
{
	GamePlayer* pPlayer = getPoolPlayer(pIClientUserItem);
	if (pPlayer)
	{
		return pPlayer;
	}
	return new BJLPlayer(pIClientUserItem);
}
void BJLGameScence::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	utility::toString("BJLGameScence::OnEventUserEnter");
	GameBase::OnEventUserEnter(pIClientUserItem,bLookonUser);

	for (int i=0;i<(int)m_kPlayers.size();i++)
	{
		BJLPlayer* player = (BJLPlayer*)m_kPlayers[i];

		if ( player->getSeatID() < 0 && i < MAX_PLAYER_NUM )
		{
			player->seatDown(i,this);
		}
	}

}
void BJLGameScence::upSelfPlayerInfo()
{
	WidgetFun::setText(this,"BJLSelfGold",getSelfGamePlayer()->GetUserScore());
}

void BJLGameScence::initPlayerNode()
{
	for (int i=0;i<MAX_PLAYER_NUM;i++)
	{
		Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BJLPlayer",i));
		pNode->setVisible(false);
	}
}
void BJLGameScence::setZhuang(int nChairID)
{
	for (int i=0;i<m_kPlayers.size();i++)
	{
		((BJLPlayer*)m_kPlayers[i])->setZhuangIcon(false);
	}
	BJLPlayer* pBanker = (BJLPlayer*)getPlayerByChairID(nChairID);
	if (pBanker)
	{
		pBanker->setZhuangIcon(true);
	}

}
