#include "FZMJGameScence.h"
#include "Game/GameLib.h"
#include "FZMJPlayer.h"

FZMJPlayer* FZMJGameScence::getPlayerByChairID(int iChairID)
{
	if (!m_pLocal->getUserItem(false))
	{
		return m_pLocal;
	}
	int iIdex = (m_pLocal->GetChairID()-iChairID+MAX_PLAYER)%MAX_PLAYER;
	return m_pPlayer[iIdex];
}
GamePlayer* FZMJGameScence::CreatePlayer( IClientUserItem * pIClientUserItem )
{
	if (pIClientUserItem->GetUserID() == UserInfo::Instance().getUserID())
	{
		m_pLocal->setUserItem(pIClientUserItem);
		return m_pLocal;
	}
	else
	{
		GamePlayer* pPlayer = getPlayerByChairID(pIClientUserItem->GetChairID());
		pPlayer->setUserItem(pIClientUserItem);
		return pPlayer;
	}
	return NULL;
}
void FZMJGameScence::DeletePlayer(GamePlayer* pPlayer)
{
}
void FZMJGameScence::upSelfPlayerInfo()
{
	WidgetFun::getChildWidget(this,"TimeNodeChild")->setRotation(-90*(m_pLocal->GetChairID()));
	//WidgetFun::setText(this,"BJLSelfGold",getSelfGamePlayer()->GetUserScore());
}
