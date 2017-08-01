#include "FZMJGameScence.h"
#include "Game/GameLib.h"

#include "CMD_FZMJ.h"
#include "FZMJPlayer.h"
#include "FZMJGameLogic.h"

void FZMJGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,FZMJGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,FZMJGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_OUT_CARD_CSGANG,this,FZMJGameScence::OnSubOutCardCSGang);
	addNetCBDefine(SUB_S_SEND_CARD,this,FZMJGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_SEND_CARD_CSGANG,this,FZMJGameScence::OnSubSendCard_CSGang);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY,this,FZMJGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT,this,FZMJGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END,this,FZMJGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE,this,FZMJGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU,this,FZMJGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE,this,FZMJGameScence::OnSubGangScore);
	addNetCBDefine(SUB_S_XIAO_HU,this,FZMJGameScence::OnSubXiaoHu);

	addNetCBDefine(SUB_S_MASTER_HANDCARD,this,FZMJGameScence::OnMasterHandCard);
	addNetCBDefine(SUB_S_MASTER_LEFTCARD,this,FZMJGameScence::OnMasterLeftCard);
	
}
//游戏开始
void FZMJGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
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

	int nIndex = 1;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setHandCard(pGameStart->cbCardData,MAX_COUNT-1);
		m_pPlayer[i]->startGame();
	}

	HNMJButtonAction_ShowCard(NULL,NULL);
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	if (pGameStart->cbXiaoHuTag == 1)
	{
		setGameState(HNMJ_STATE_XIAO_HU);
	}
	else
	{
		setGameState(HNMJ_STATE_PLAYING);
	}

	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_TING_CARD))
	{
		m_pLocal->showTingCard();
	}

}
//用户出牌
void FZMJGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
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

	FZMJPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->sendOutCard(pOutCard->cbOutCardData);
	if (pPlayer == m_pLocal && HaveOptionRule(GAME_OPTION_RULE_SHOW_TING_CARD))
	{
		m_pLocal->showTingCard();
	}

	defaultPlayerActionState();
}


//用户出牌
void FZMJGameScence::OnSubOutCardCSGang(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard_CSGang));
	if (wDataSize!=sizeof(CMD_S_OutCard_CSGang)) 
		return;

	//消息处理
	CMD_S_OutCard_CSGang * pOutCard=(CMD_S_OutCard_CSGang *)pBuffer;

	FZMJPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->showCSGangCard(pOutCard->cbOutCardData1,pOutCard->cbOutCardData2,true);
}

//发牌消息
void FZMJGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	WidgetFun::setVisible(this,"SelfActionNode",false);
	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask,pSendCard->cbCardData);
	FZMJPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 1;
		WidgetFun::setText(this,"LastCardCout",iLastCardCout);
		if (pPlayer == m_pLocal)
		{
			m_pLocal->setHasCSGang();
		}
	}
	setGameState(HNMJ_STATE_PLAYING);
}

void FZMJGameScence::OnSubSendCard_CSGang( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard_CSGang));
	if (wDataSize!=sizeof(CMD_S_SendCard_CSGang)) 
		return;
	WidgetFun::setVisible(this,"SelfActionNode",false);

	//变量定义
	CMD_S_SendCard_CSGang * pSendCard=(CMD_S_SendCard_CSGang *)pBuffer;

	FZMJPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	pPlayer->showCSGangCard(pSendCard->cbCardData1,pSendCard->cbCardData2,false);
	if (pSendCard->cbActionMask & WIK_CHI_HU )
	{
		setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask);
	}
	else if(pPlayer == m_pLocal)
	{
		m_kCSGangSendCard1 = pSendCard->cbCardData1;
		m_kCSGangSendCard2 = pSendCard->cbCardData2;
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(FZMJGameScence::AutoOutCSGangCard,this),3);
	}
	int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 2;
	WidgetFun::setText(this,"LastCardCout",iLastCardCout);
	setGameState(HNMJ_STATE_NULL);
}
void FZMJGameScence::AutoOutCSGangCard()
{
	CMD_C_OutCard_CSGang OutCard;
	OutCard.cbCardData1 =m_kCSGangSendCard1;
	OutCard.cbCardData2 =m_kCSGangSendCard2;
	SendSocketData(SUB_C_OUT_CARD_CSGANG,&OutCard,sizeof(OutCard));
}
//操作提示
void FZMJGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	setCurrentPlayer(m_pLocal->GetChairID(),pOperateNotify->cbActionMask,pOperateNotify->cbActionCard);
}
//操作结果
void FZMJGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return ;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	FZMJPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);

	FZMJPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer &&(pOperateResult->cbOperateCode == WIK_PENG 
		|| pOperateResult->cbOperateCode == WIK_LEFT
		|| pOperateResult->cbOperateCode == WIK_CENTER
		|| pOperateResult->cbOperateCode == WIK_RIGHT
		|| (pOperateResult->cbOperateCode == WIK_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )
		|| (pOperateResult->cbOperateCode == WIK_CS_GANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )
		|| (pOperateResult->cbOperateCode == WIK_BU_ZHANG && pOperateResult->wOperateUser != pOperateResult->wProvideUser )))
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}
	if (pOperateResult->cbOperateCode == WIK_CS_GANG && pPlayer == m_pLocal)
	{
		m_pLocal->setHasCSGang(true);//长沙杠，
	}

	setCurrentPlayer(pOperateResult->wOperateUser,WIK_NULL);
	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//游戏结束
void FZMJGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	WidgetFun::setVisible(this,"TuoGuanStateNode",false);
	showJieSuanInfo(pGameEnd);
	setGameState(HNMJ_STATE_NULL);
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		int iChirID = m_pPlayer[i]->GetChairID();
		m_pPlayer[i]->showHandCard(pGameEnd->WeaveItemArray[iChirID],pGameEnd->cbWeaveCount[iChirID],
			pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
		m_pPlayer[i]->showCard(pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);	
	}
	defaultMaster();
	removeWaringSound();

}
//用户托管
void FZMJGameScence::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) 
		return;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	FZMJPlayer* pPlayer = getPlayerByChairID(pTrustee->wChairID);
	pPlayer->setTuoGuan(pTrustee->bTrustee);
	if (pPlayer == m_pLocal)
	{
		WidgetFun::setVisible(this,"TuoGuanStateNode",pTrustee->bTrustee);
	}
}
//
void FZMJGameScence::OnSubUserChiHu( const void *pBuffer,WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_ChiHu));
	if (wDataSize!=sizeof(CMD_S_ChiHu))
		return;

	//消息处理
	CMD_S_ChiHu * pChiHu=(CMD_S_ChiHu *)pBuffer;

	FZMJPlayer* pPlayer = getPlayerByChairID(pChiHu->wChiHuUser);
//	pPlayer->showEffect("Hu");
	pPlayer->setChiHuCard(pChiHu->cbChiHuCard);
	pPlayer->addNewInCard(pChiHu->cbChiHuCard);
	if (pChiHu->wChiHuUser != pChiHu->wProviderUser)
	{
		FZMJPlayer* pDestPlayer = getPlayerByChairID(pChiHu->wProviderUser);
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->removeHandOutCard(pChiHu->cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pDestPlayer->runAniDianPao();
	}
	else
	{
		pPlayer->showHuPai(true);
		pPlayer->showStatusImagic("ZiMo");
		pPlayer->runAniZiMo();
	}
}
//
void FZMJGameScence::OnSubGangScore( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_GangScore));
	if (wDataSize!=sizeof(CMD_S_GangScore)) 
		return;

	//消息处理
	CMD_S_GangScore * pGangScore=(CMD_S_GangScore *)pBuffer;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		FZMJPlayer* pPlayer = getPlayerByChairID(i);
		//pPlayer->showAddGold(pGangScore->lGangScore[i]);
	}
}
void FZMJGameScence::OnSubXiaoHu(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_XiaoHu));
	if (wDataSize!=sizeof(CMD_S_XiaoHu)) 
		return;

	//消息处理
	CMD_S_XiaoHu * pXiaoHu=(CMD_S_XiaoHu *)pBuffer;

	FZMJPlayer* pPlayer = getPlayerByChairID(pXiaoHu->wXiaoHuUser);
	if (pPlayer)
	{
		pPlayer->showXiaoHu(pXiaoHu->dwXiaoCode);
		pPlayer->showCard(pXiaoHu->cbCardData,pXiaoHu->cbCardCount);

		TimeManager::Instance().addCerterTimeCB1<int>(CC_CALLBACK_1(FZMJGameScence::OnXiaoHuEnd,this),this,3.f,pXiaoHu->wXiaoHuUser);
	}
}

bool FZMJGameScence::OnXiaoHuEnd( int nChairID )
{
	FZMJPlayer* pPlayer = getPlayerByChairID(nChairID);
	if (pPlayer)
	{
		pPlayer->showHandCard();
	}
	return false;
}

