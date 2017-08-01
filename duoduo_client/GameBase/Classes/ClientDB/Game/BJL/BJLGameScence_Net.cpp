#include "BJLGameScence.h"
#include "Game/GameLib.h"

#include "CMD_Baccarat.h"
#include "Game/Game/QYCardTurnManager.h"

void BJLGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_FREE,this,BJLGameScence::OnSubGameFree);
	addNetCBDefine(SUB_S_GAME_START,this,BJLGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_PLACE_JETTON,this,BJLGameScence::OnSubPlaceBet);
	addNetCBDefine(SUB_S_GAME_END,this,BJLGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_GAME_OPENCARD,this,BJLGameScence::OnSubGameOpenCard);
	addNetCBDefine(SUB_S_TUCH_TURNCARD,this,BJLGameScence::OnSubTuchTurnCard);
	addNetCBDefine(SUB_S_SEND_RECORD,this,BJLGameScence::OnSubGameRecord);
	addNetCBDefine(SUB_S_PLACE_JETTON_FAIL,this,BJLGameScence::OnSubPlaceBetFail);
	addNetCBDefine(SUB_S_LEFT_COUT,this,BJLGameScence::OnSubLeftCout);
	
	addNetCBDefine(SUB_S_BANKER_LIST,this,BJLGameScence::OnSubBankerList);
	addNetCBDefine(SUB_S_APPLY_BANKER,this,BJLGameScence::OnSubApplyBanker);
	addNetCBDefine(SUB_S_CANCEL_BANKER,this,BJLGameScence::OnSubCancleBanker);
	addNetCBDefine(SUB_S_CHANGE_BANKER,this,BJLGameScence::OnSubChangeBanker);

	addNetCBDefine(SUB_S_CANCEL_BET,this,BJLGameScence::OnSubCancleBet);
}
//游戏空闲
void BJLGameScence::OnSubGameFree(const void * pBuffer, word wDataSize)
{
	utility::toString("OnSubGameFree1");
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return;
	utility::toString("OnSubGameFree2");

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	showClock(pGameFree->cbTimeLeave,true);
	setTimeDesc(GAME_SCENE_FREE);
	defaultState();
}
//游戏开始
void BJLGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	utility::toString("OnSubGameStart1");
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return;
	utility::toString("OnSubGameStart2");

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	showClock(pGameStart->cbTimeLeave);
	setTimeDesc(GAME_SCENE_BET);
	setInGameShow(true);
	statusTip(GAME_SCENE_BET);
	upBetAreaState(true);

	WidgetFun::runWidgetAction(this,"BJL_AreaIcon_0","Start");
	WidgetFun::runWidgetAction(this,"BJL_AreaIcon_1","Start");

	setZhuang(pGameStart->wBankerUser);  //设置庄家
}
void BJLGameScence::OnSubPlaceBet(const void * pBuffer, word wDataSize)
{
	utility::toString("OnSubPlaceBet1");
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceBet));
	if (wDataSize!=sizeof(CMD_S_PlaceBet)) return ;
	utility::toString("OnSubPlaceBet2");

	//消息处理
	CMD_S_PlaceBet * pPlaceBet=(CMD_S_PlaceBet *)pBuffer;	
	BJLPlayer* pPlayer = (BJLPlayer*)getPlayerByChairID(pPlaceBet->wChairID);
	if (!pPlayer)
	{
		return;
	}

	Vec2 pos = pPlayer->getSeatPos();
	MoveGold(true,pPlaceBet->cbBetArea,pPlaceBet->lBetScore,pos,pPlaceBet->wChairID);

	int nSelfBet = 0;
	if (pPlaceBet->wChairID == getSelfChairID())
	{
		nSelfBet = pPlaceBet->lBetScore;
		int nScore = utility::parseInt(WidgetFun::getText(this,"BJLSelfGold"));
		WidgetFun::setText(this,"BJLSelfGold",nScore - pPlaceBet->lBetScore);

		if (pPlaceBet->cbBetArea == AREA_XIAN)
		{
			WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_XIAN),true);
			WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_ZHUANG),false);
		}
		else if(pPlaceBet->cbBetArea == AREA_ZHUANG)
		{
			WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_ZHUANG),true);
			WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_XIAN),false);
		}
	}
	setAreaScore(pPlaceBet->cbBetArea,pPlaceBet->lBetScore,nSelfBet,false,true);
	setAreaBetMax(pPlaceBet->wXianChairID,pPlaceBet->wZhuangChairID,pPlaceBet->wXianBet,pPlaceBet->wZhuangBet);;
}
void BJLGameScence::OnSubGameEnd(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return ;
	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;	

	setShowCardsNum(false,INDEX_PLAYER,0);
	setShowCardsNum(false,INDEX_BANKER,0);
	//保存结算数据
	m_nWinScore = pGameEnd->lPlayAllScore;
	zeromemory(m_nAllWinScore,sizeof(m_nAllWinScore));
	memcpy(m_nAllWinScore,pGameEnd->IUserWinScore,sizeof(m_nAllWinScore));
	
	sendCardBack(INDEX_PLAYER,0);
	sendCardBack(INDEX_PLAYER,1);
	sendCardBack(INDEX_BANKER,0);
	sendCardBack(INDEX_BANKER,1);

	showClock(pGameEnd->cbTimeLeave,true);
	setTimeDesc(GAME_SCENE_END);
	statusTip(GAME_SCENE_END);

	//隐藏最大下注
	showAreaBetMax(INDEX_PLAYER,false);
	showAreaBetMax(INDEX_BANKER,false);
	moveAreaIconToPlayer(INDEX_PLAYER);
	moveAreaIconToPlayer(INDEX_BANKER);

	upBetAreaState(false);
	//showClockTimeCallBack(pGameEnd->cbTimeLeave,CC_CALLBACK_0(BJLGameScence::resultAni,this),8); 
	if (!m_kLastBetListTemp.empty())
	{
		m_kLastBetList = m_kLastBetListTemp;//保存本局的押注数据
		m_kLastBetListTemp.clear();
	}
}

void BJLGameScence::OnSubGameOpenCard(const void * pBuffer, word wDataSize)
{
	WidgetFun::setVisible(this,"BJLZhuangListNode",false);
	showClock(0,false);
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameOpenCard));
	if (wDataSize!=sizeof(CMD_S_GameOpenCard)) return ;
	//消息处理
	CMD_S_GameOpenCard * pOpenCard=(CMD_S_GameOpenCard *)pBuffer;

	m_nNowOpenIndex = pOpenCard->nOpenIndex;
	m_nOpenChairID = pOpenCard->nChairID;
	showClockTimeCallBack(QYCardTurnManager::pInstance(),"BJLOpenShadowInfo:TimeNum",pOpenCard->cbTimeLeave-3,
		CC_CALLBACK_0(BJLGameScence::openCardTimeOver,this,pOpenCard->nOpenIndex,pOpenCard->nChairID),0); 
	TimeManager::Instance().addCerterTimeCB1<int>(CC_CALLBACK_1(BJLGameScence::autoOpenCard,this),this,pOpenCard->cbActTime,m_nNowOpenIndex);
	if (pOpenCard->nOpenIndex == OPEN_XIAN_12CARD)
	{
		//设置牌信息及开始发牌 只设置一遍
		setCardInfo(pOpenCard->cbCardCount,pOpenCard->cbTableCardArray);
	}
	OnOpenCard(pOpenCard->nOpenIndex,pOpenCard->nChairID);
}

void BJLGameScence::OnSubTuchTurnCard(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_TuchTurnCard));
	if (wDataSize!=sizeof(CMD_S_TuchTurnCard)) return ;

	CMD_S_TuchTurnCard* pInfo = (CMD_S_TuchTurnCard*)pBuffer;
	if (pInfo->wChairID != getSelfChairID())
	{
		QYCardTurnManager::Instance().onNetTouchAction(pInfo->nIndex,pInfo->nTuchType,Vec2(pInfo->touchX,pInfo->touchY));
	}
}
void BJLGameScence::OnSubGameRecord(const void * pBuffer, word wDataSize)
{

}
void BJLGameScence::OnSubPlaceBetFail(const void * pBuffer, word wDataSize)
{

}
void BJLGameScence::OnSubLeftCout(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize==sizeof(dword));
	if (wDataSize!=sizeof(dword)) return ;

	dword * pNetInfo = (dword*)pBuffer;
	WidgetFun::setText(this,"BJL_LeftCard",(int)*pNetInfo);
}
void BJLGameScence::OnSubBankerList(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_BankerList));
	if (wDataSize!=sizeof(CMD_S_BankerList)) return ;

	ListViewEx* pList = WidgetFun::getListViewWidget(this,"BJLZhuang_List");
	pList->removeAllChildren();
	CMD_S_BankerList * pListInfo = (CMD_S_BankerList*)pBuffer;

	WidgetFun::setVisible(this,"BJLButton_Shangzhuang",true);
	WidgetFun::setVisible(this,"BJLButton_QuXiaoZhuang",false);

	for (int i=0;i<GAME_PLAYER;i++)
	{
		int wChairID = pListInfo->kBankerList[i];
		if (!getPlayerByChairID(wChairID))
		{
			continue;
		}
		if (wChairID<0)continue;
		std::string kName = getPlayerByChairID(wChairID)->GetNickName();
		SCORE nScore = getPlayerByChairID(wChairID)->GetUserScore();
		Node* pNode = WidgetManager::Instance().createWidget("BJLZhuangNode",pList);

		WidgetFun::setText(pNode,"zhuang_order",i+1);
		WidgetFun::setText(pNode,"zhuang_name",kName);
		WidgetFun::setText(pNode,"zhuang_gold",nScore);
		WidgetFun::setVisible(pNode,"zhuang_text",false);
		if (i==0)
		{
			WidgetFun::setVisible(pNode,"zhuang_text",true);
		}
		if (wChairID == getSelfChairID())
		{
			WidgetFun::setTextColor(pNode,"zhuang_order",Color3B::RED);
			WidgetFun::setTextColor(pNode,"zhuang_name",Color3B::RED);

			WidgetFun::setVisible(this,"BJLButton_Shangzhuang",false);
			WidgetFun::setVisible(this,"BJLButton_QuXiaoZhuang",true);
		}
	}
}

void BJLGameScence::OnSubApplyBanker(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return ;
	BJLButton_ZhuangList(NULL,NULL);
}
void BJLGameScence::OnSubCancleBanker(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return ;
	BJLButton_ZhuangList(NULL,NULL);
}
void BJLGameScence::OnSubChangeBanker( const void * pBuffer, word wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return ;
	CMD_S_ChangeBanker* pInfo = (CMD_S_ChangeBanker*)pBuffer;

	setZhuang(pInfo->wBankerUser);  //设置庄家
}

void BJLGameScence::OnSubCancleBet(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancleBet));
	if (wDataSize!=sizeof(CMD_S_CancleBet)) return ;

	CMD_S_CancleBet* pInfo =(CMD_S_CancleBet*)pBuffer; 

	for (int i=0;i<(int)m_pBetChipList.size();i++)
	{
		if(pInfo->wChairID == m_pBetChipList[i].nChairID && m_pBetChipList[i].pChip != NULL)
		{
			m_pBetChipList[i].pChip->removeFromParentAndCleanup(true);
			m_pBetChipList[i].pChip = NULL;
		}
	}

	for (int i=0;i<AREA_MAX;++i)
	{
		setAreaScore(i,pInfo->lAllBet[i],0,pInfo->wChairID == getSelfChairID());
	}
	setAreaBetMax(pInfo->wXianChairID,pInfo->wZhuangChairID,pInfo->wXianBet,pInfo->wZhuangBet);
	if (pInfo->wChairID == getSelfChairID())
	{
		m_kLastBetListTemp.clear();
	}

	WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_ZHUANG),true);
	WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_XIAN),true);
	//隐藏最大下注
	showAreaBetMax(INDEX_PLAYER,false);
	showAreaBetMax(INDEX_BANKER,false);
}