#include "BJLGameScence.h"
#include "Game/GameLib.h"
#include "BJLGameLogic.h"
#include "CMD_Baccarat.h"
#include "Game/Game/QYCardTurnManager.h"

bool BJLGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	utility::log("BJLGameScence::OnEventSceneMessage");
	if (cbGameStatus == GAME_SCENE_FREE)
	{
		OnFreeScence(data,dataSize);
	}

	if (cbGameStatus == GAME_SCENE_PLAY
		|| cbGameStatus == GAME_SCENE_END|| cbGameStatus == GAME_SCENE_OPEN)
	{
		OnPlayScence(data,dataSize);
	}
	return true;
}
void BJLGameScence::OnFreeScence(void* data, int wDataSize)
{
	utility::log("BJLGameScence::OnFreeScence1");
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;
	utility::log("BJLGameScence::OnFreeScence2");

	//消息处理
	CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)data;

	WidgetFun::setText(this,"BJL_LeftCard",(int)pStatusFree->cbCardLeft);

	showClock(pStatusFree->cbTimeLeave);
	setTimeDesc(GAME_SCENE_FREE);
	WidgetFun::setText(this,"BJLSelfGold",pStatusFree->lPlayFreeSocre);
	defaultState();
}
void BJLGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	//消息处理
	CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)data;

	WidgetFun::setText(this,"BJL_LeftCard",(int)pStatusPlay->cbCardLeft);

	WidgetFun::setText(this,"BJLSelfGold",pStatusPlay->lPlayFreeSocre); 

	setAreaBetMax(pStatusPlay->wXianChairID,pStatusPlay->wZhuangChairID,pStatusPlay->wXianBet,pStatusPlay->wZhuangBet);

	if (pStatusPlay->cbGameStatus == GAME_SCENE_END )
	{

		setShowCardsNum(false,INDEX_PLAYER,0);
		setShowCardsNum(false,INDEX_BANKER,0);

		setTimeDesc(GAME_SCENE_END);
		showClock(pStatusPlay->cbTimeLeave);
//		showClockTimeCallBack(pStatusPlay->cbTimeLeave,CC_CALLBACK_0(BJLGameScence::resultAni,this),pStatusPlay->cbTimeLeave/5); 

		showAreaBetMax(INDEX_PLAYER,false);
		showAreaBetMax(INDEX_BANKER,false);

		moveAreaIconToPlayer(INDEX_PLAYER);
		moveAreaIconToPlayer(INDEX_BANKER);

		sendCardBack(INDEX_PLAYER,0,false);
		sendCardBack(INDEX_PLAYER,1,false);
		sendCardBack(INDEX_BANKER,0,false);
		sendCardBack(INDEX_BANKER,1,false);

		upBetAreaState(false);

	}
	else if (pStatusPlay->cbGameStatus == GAME_SCENE_OPEN)
	{
		showClock(0,false);
		setCardInfo(pStatusPlay->cbCardCount,pStatusPlay->cbTableCardArray);

		int nPlayerPoint = BJLGameLogic::Instance().GetCardListPip(pStatusPlay->cbTableCardArray[INDEX_PLAYER],pStatusPlay->cbCardCount[INDEX_PLAYER]);
		int nBankerPoint = BJLGameLogic::Instance().GetCardListPip(pStatusPlay->cbTableCardArray[INDEX_BANKER],pStatusPlay->cbCardCount[INDEX_BANKER]);

		m_nNowOpenIndex = pStatusPlay->nOpenCardIndex;
		m_nOpenChairID = pStatusPlay->nOpenChairID;
		float timeMoveToArea = pStatusPlay->cbTimeLeave-3;
		float timeAutoOpen = pStatusPlay->cbTimeLeave-4;
		if (timeMoveToArea<0)
		{
			timeMoveToArea = 0;
		}
		if (timeAutoOpen>0)
		{
			TimeManager::Instance().addCerterTimeCB1<int>(CC_CALLBACK_1(BJLGameScence::autoOpenCard,this),this,timeAutoOpen,m_nNowOpenIndex);
		}

		showClockTimeCallBack(QYCardTurnManager::pInstance(),"BJLOpenShadowInfo:TimeNum",timeMoveToArea,
			CC_CALLBACK_0(BJLGameScence::openCardTimeOver,this,pStatusPlay->nOpenCardIndex,pStatusPlay->nOpenChairID),0); 
		
		if ( pStatusPlay->nOpenCardIndex == OPEN_XIAN_12CARD )
		{
			showCardBack(INDEX_BANKER,0,true);
			showCardBack(INDEX_BANKER,1,true);
		}
		else if ( pStatusPlay->nOpenCardIndex == OPEN_ZHUANG_12CARD )
		{
			showCardBack(INDEX_PLAYER,0,false);
			showCardBack(INDEX_PLAYER,1,false);
		}
		else if (pStatusPlay->nOpenCardIndex == OPEN_XIAN_3CARD)
		{
			showCardBack(INDEX_PLAYER,0,false);
			showCardBack(INDEX_PLAYER,1,false);
			showCardBack(INDEX_BANKER,0,false);
			showCardBack(INDEX_BANKER,1,false);
		}
		else if (pStatusPlay->nOpenCardIndex == OPEN_ZHUANG_3CARD)
		{
			showCardBack(INDEX_PLAYER,0,false);
			showCardBack(INDEX_PLAYER,1,false);
			showCardBack(INDEX_PLAYER,2,false);
			showCardBack(INDEX_BANKER,0,false);
			showCardBack(INDEX_BANKER,1,false);
		}
		OnOpenCard(pStatusPlay->nOpenCardIndex,pStatusPlay->nOpenChairID);
	}
	else
	{
		setInGameShow(true);
		showClock(pStatusPlay->cbTimeLeave);
		setTimeDesc(GAME_SCENE_BET);

		upBetAreaState(true);
	}
	//设置筹码和押注信息
	for (int i=0;i<AREA_MAX;++i)
	{
		MoveGold(false,i,pStatusPlay->lAllBet[i],Vec2(0,0),-1);
		setAreaScore(i,pStatusPlay->lAllBet[i],pStatusPlay->lPlayBet[i]);
	}
	
}
bool BJLGameScence::OnGFTableMessage(const char* szMessage)
{
	NoticeMsg::Instance().ShowTopMsg(szMessage);
	return true;
}