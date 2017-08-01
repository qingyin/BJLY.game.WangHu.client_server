#include "XZDDGameScence.h"
#include "Game/GameLib.h"

#include "CMD_XZDD.h"
#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"

void XZDDGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,XZDDGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,XZDDGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD,this,XZDDGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY,this,XZDDGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT,this,XZDDGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END,this,XZDDGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE,this,XZDDGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU,this,XZDDGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE,this,XZDDGameScence::OnSubGangScore);

	addNetCBDefine(SUB_S_HUANPAI,this,XZDDGameScence::OnSubHuanPai);
	addNetCBDefine(SUB_S_XUANQUE_NOTICE,this,XZDDGameScence::OnSubXuanQueNotice);
	addNetCBDefine(SUB_S_XUANQUE,this,XZDDGameScence::OnSubXuanQue);
	addNetCBDefine(SUB_S_HUANPAI_CHENGDU,this,XZDDGameScence::OnSubHuanPaiChengDu);

	addNetCBDefine(SUB_S_HUANPAI_NOTICE,this,XZDDGameScence::OnSubHuanPaiNotice);
	
}
//游戏开始
void XZDDGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
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
	m_nLeftTime = pGameStart->nLeftTime;

	WidgetFun::setText(this,"LastCardCout",(int)pGameStart->cbLeftCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setHandCard(pGameStart->cbCardData,MAX_COUNT-1);
		m_pPlayer[i]->startGame();
	}
//	showSaiZi(pGameStart->lSiceCount);
	XZDDButtonAction_ShowCard(NULL,NULL);
	m_nGameState = XZDD_STATE_READY;
}
//用户出牌
void XZDDGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
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

	XZDDPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->sendOutCard(pOutCard->cbOutCardData);
	pPlayer->blackAllHandCard(true);

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}
//发牌消息
void XZDDGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) 
		return;

	WidgetFun::setVisible(this,"SelfActionNode",false); //发牌了 先隐藏操作按钮
	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	setCurrentPlayer(pSendCard->wCurrentUser,pSendCard->cbActionMask,pSendCard->cbCardData);
	XZDDPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this,"LastCardCout")) - 1;
		WidgetFun::setText(this,"LastCardCout",iLastCardCout);
	}
	if (pPlayer == m_pLocal&& !pSendCard->bHasHuPai)
	{
		m_pLocal->blackHandCardByQueColor();
	}
	else if(pPlayer == m_pLocal&& pSendCard->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
		if (pSendCard->cbActionMask ==WIK_NULL )
		{
			TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOutCard,this),3,pSendCard->cbCardData);
		}
		else
		{
			TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOperate,this),3.f,pSendCard->cbActionMask);
		}
	}
	else
	{
		m_pLocal->blackAllHandCard(true);
	}
	m_nGameState = XZDD_STATE_PLAYING;
}
//操作提示
void XZDDGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;
	m_wResumeUser =pOperateNotify->wResumeUser;
	setCurrentPlayer(m_pLocal->GetChairID(),pOperateNotify->cbActionMask,pOperateNotify->cbActionCard);
	if (pOperateNotify->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
		TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOperate,this),3.f,pOperateNotify->cbActionMask);
	}
}
//操作结果
void XZDDGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return ;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	XZDDPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);
	if (pPlayer == m_pLocal)
	{
		m_pLocal->blackHandCardByQueColor();
	}
	XZDDPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer)
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}

	setCurrentPlayer(pOperateResult->wOperateUser,WIK_NULL);
	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//游戏结束
void XZDDGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return;

	m_nGameState = XZDD_STATE_NULL;
	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;


	WidgetFun::setVisible(this,"FreeStateNode",true);
	if (!IsPrivateGame())
	{
		WidgetFun::setVisible(this,"GameResoult2",true);
		cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"GameResoult2");
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			int iChirID = m_pPlayer[i]->GetChairID();
			m_pPlayer[i]->showCard(pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
			WidgetFun::setButtonTxt(pRootNode,utility::toString("Button_ResoultPlayer",i),utility::a_u8(m_pPlayer[i]->GetNickName()));
			WidgetFun::setText(pRootNode,utility::toString("GoldTxt",i),pGameEnd->lGameScore[iChirID]);
			setGameResoultPlayerInfo(pGameEnd,m_pPlayer[i],WidgetFun::getChildWidget(pRootNode,utility::toString("ResoultPlayer",i)));
		}
		WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,"Button_ResoultPlayer0"));
	}
	else
	{
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			int iChirID = m_pPlayer[i]->GetChairID();
			m_pPlayer[i]->showCard(pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
		}
		showJieSuanInfo(pGameEnd);
	}
	GameManagerBase::InstanceBase().OnGameEnd(this,3);
}
//用户托管
void XZDDGameScence::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) 
		return;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
}
//
void XZDDGameScence::OnSubUserChiHu( const void *pBuffer,WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_ChiHu));
	if (wDataSize!=sizeof(CMD_S_ChiHu))
		return;

	//消息处理
	CMD_S_ChiHu * pChiHu=(CMD_S_ChiHu *)pBuffer;

	XZDDPlayer* pPlayer = getPlayerByChairID(pChiHu->wChiHuUser);
	pPlayer->showAddGold(pChiHu->lGameScore);
	pPlayer->showEffect("Hu");
	pPlayer->setChiHuCard(pChiHu->cbChiHuCard);
	if (pChiHu->wChiHuUser != pChiHu->wProviderUser)
	{
		XZDDPlayer* pDestPlayer = getPlayerByChairID(pChiHu->wProviderUser);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->removeHandOutCard(pChiHu->cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pDestPlayer->showAddGold(-pChiHu->lGameScore);

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
			m_pPlayer[i]->showAddGold(-pChiHu->lGameScore/(MAX_PLAYER-pChiHu->cbWinOrder));
		}
		pPlayer->showStatusImagic("ZiMo");
		pPlayer->runAniZiMo();
		pPlayer->removeHandCard(pChiHu->cbChiHuCard);
	}
	
	pPlayer->showHuPai(pPlayer != m_pLocal);
}
//
void XZDDGameScence::OnSubGangScore( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_GangScore));
	if (wDataSize!=sizeof(CMD_S_GangScore)) 
		return;

	//消息处理
	CMD_S_GangScore * pGangScore=(CMD_S_GangScore *)pBuffer;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->showAddGold(pGangScore->lGangScore[i]);
	}
}

void XZDDGameScence::OnSubHuanPai(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_HuanPai));
	if (wDataSize!=sizeof(CMD_S_HuanPai))return;

	CMD_S_HuanPai* pHuanPai = (CMD_S_HuanPai*)pBuffer;
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->setHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this,"ExchangeCardNode",false);
}
void XZDDGameScence::OnSubHuanPaiChengDu( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_HuanPaiChengDu));
	if (wDataSize!=sizeof(CMD_S_HuanPaiChengDu))return;

	CMD_S_HuanPaiChengDu* pHuanPai = (CMD_S_HuanPaiChengDu*)pBuffer;

	showSaiZi_HuanPai(pHuanPai->wSice);
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->saveRemoveHuanPai(pHuanPai->cbRemoveHuanCard);
		m_pLocal->saveHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this,"ExchangeCardNode",false);
}

void XZDDGameScence::OnSubXuanQueNotice( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_XuanQueNotice));
	if (wDataSize!=sizeof(CMD_S_XuanQueNotice))return;

	CMD_S_XuanQueNotice* pInfo = (CMD_S_XuanQueNotice*)pBuffer;
	m_nLeftTime = pInfo->nLeftTime;

	if (pInfo->wChairId == getSelfChairID())
	{
		m_pLocal->setQueColor(pInfo->nQueColor,false);
	}

//	if (m_nGameType == GAME_TYPE_SICHUAN)
	{
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(XZDDGameScence::DoXuanQueNotice,this),2.f);
	}
	
}

void XZDDGameScence::OnSubXuanQue( const void *pBuffer, WORD wDataSize )
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_XuanQue));
	if (wDataSize!=sizeof(CMD_S_XuanQue))return;

	CMD_S_XuanQue* pInfo = (CMD_S_XuanQue*)pBuffer;

	for (int i=0;i<MAX_PLAYER;i++)
	{
		cocos2d::log(" user chairid :%d",i);
		cocos2d::log(" user nQueColor:%d",(int)pInfo->nQueColor[i]);

		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setQueColor(pInfo->nQueColor[i],true);
		pPlayer->runQueIconAni();
	}

}

void XZDDGameScence::OnSubHuanPaiNotice(const void *pBuffer, WORD wDataSize )
{
	showHuanPai();
//	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(XZDDGameScence::showHuanPai,this),4.2f);
}

void XZDDGameScence::showHuanPai()
{
	ASSERT(m_pLocal);

	WidgetFun::setVisible(this,"ExchangeCardNode",true);
	m_pLocal->showHuanPai(GAME_TYPE_CHENGDU);
	m_nGameState = XZDD_STATE_HUANPAI;
}