#include "NCMJGameScence.h"
#include "Game/GameLib.h"

#include "CMD_NCMJ.h"
#include "NCMJPlayer.h"
#include "NCMJGameLogic.h"

void NCMJGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,NCMJGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,NCMJGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD,this,NCMJGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY,this,NCMJGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT,this,NCMJGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END,this,NCMJGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE,this,NCMJGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU,this,NCMJGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE,this,NCMJGameScence::OnSubGangScore);
}
//游戏开始
void NCMJGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	defaultState();

	m_iBankerUser=pGameStart->wBankerUser;
	m_iCurrentUser=pGameStart->wCurrentUser;
	m_iUserAction = pGameStart->cbUserAction;

	WidgetFun::setText(this,"LastCardCout",(int)pGameStart->cbLeftCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setHandCard(pGameStart->cbCardData,MAX_COUNT-1);
		m_pPlayer[i]->startGame();
	}

	NCMJButtonAction_ShowCard(NULL,NULL);
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	if (pGameStart->cbXiaoHuTag == 1)
	{
		setGameState(NCMJ_STATE_XIAO_HU);
	}
	else
	{
		setGameState(NCMJ_STATE_PLAYING);
	}

}
//用户出牌
void NCMJGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) 
		return;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setActOutCard(-1);
	}

	NCMJPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->sendOutCard(pOutCard->cbOutCardData);

	defaultPlayerActionState();
}
//发牌消息
void NCMJGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	WidgetFun::setVisible(this,"SelfActionNode",false);
	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask);
	WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString((int)pSendCard->cbCardData));
	NCMJPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 1;
		WidgetFun::setText(this,"LastCardCout",iLastCardCout);
	}
	setGameState(NCMJ_STATE_PLAYING);
}
//操作提示
void NCMJGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	setCurrentPlayer(m_pLocal->GetChairID(),pOperateNotify->cbActionMask);
	if (pOperateNotify->cbActionCard != 0)
	{
		WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString((int)pOperateNotify->cbActionCard));
	}
}
//操作结果
void NCMJGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return ;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	NCMJPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);

	NCMJPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer &&(pOperateResult->cbOperateCode == WIK_PENG 
		|| pOperateResult->cbOperateCode == WIK_LEFT
		|| pOperateResult->cbOperateCode == WIK_CENTER
		|| pOperateResult->cbOperateCode == WIK_RIGHT
		|| (pOperateResult->cbOperateCode == WIK_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )))
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}

	setCurrentPlayer(pOperateResult->wOperateUser,WIK_NULL);
	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//游戏结束
void NCMJGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	WidgetFun::setVisible(this,"FreeStateNode",true);

	showJieSuanInfo(pGameEnd);
	setGameState(NCMJ_STATE_NULL);
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		int iChirID = m_pPlayer[i]->GetChairID();
		m_pPlayer[i]->showCard(pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);	
	}
// 	WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,"Button_ResoultPlayer0"));
}
//用户托管
void NCMJGameScence::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) 
		return;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
}
//
void NCMJGameScence::OnSubUserChiHu( const void *pBuffer,WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_ChiHu));
	if (wDataSize!=sizeof(CMD_S_ChiHu))
		return;

	//消息处理
	CMD_S_ChiHu * pChiHu=(CMD_S_ChiHu *)pBuffer;

	NCMJPlayer* pPlayer = getPlayerByChairID(pChiHu->wChiHuUser);
	pPlayer->showAddGold(pChiHu->lGameScore);
	pPlayer->showEffect("Hu");
	pPlayer->setChiHuCard(pChiHu->cbChiHuCard);
	if (pChiHu->wChiHuUser != pChiHu->wProviderUser)
	{
		NCMJPlayer* pDestPlayer = getPlayerByChairID(pChiHu->wProviderUser);
		pPlayer->addNewInCard(pChiHu->cbChiHuCard);
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->showAddGold(-pChiHu->lGameScore);
		pDestPlayer->removeHandOutCard(pChiHu->cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pDestPlayer->runAniDianPao();
	}
	else
	{
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			if (i == pChiHu->wChiHuUser)
			{
				continue;
			}
			m_pPlayer[i]->showAddGold(-pChiHu->lGameScore/3);
		}
		pPlayer->showHuPai(true);
		pPlayer->showStatusImagic("ZiMo");
		pPlayer->runAniZiMo();
	}
}
//
void NCMJGameScence::OnSubGangScore( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_GangScore));
	if (wDataSize!=sizeof(CMD_S_GangScore)) 
		return;

	//消息处理
	CMD_S_GangScore * pGangScore=(CMD_S_GangScore *)pBuffer;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		NCMJPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->showAddGold(pGangScore->lGangScore[i]);
	}
}