#include "CarGameScence.h"
#include "Game/GameLib.h"
#include "CMD_RedNine100.h"

void CarGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_FREE,this,CarGameScence::onSubGameFree);
	addNetCBDefine(SUB_S_GAME_START,this,CarGameScence::onSubGameStart);
	addNetCBDefine(SUB_S_PLACE_JETTON,this,CarGameScence::onSubGameJetton);
	addNetCBDefine(SUB_S_GAME_END,this,CarGameScence::onSubGameEnd);
	addNetCBDefine(SUB_S_SEND_RECORD,this,CarGameScence::onSubGameRecord);

}
void CarGameScence::onSubGameFree(const void* pBuffer,word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	clearBetInfo();
	hideAllCarItem();

	m_nGameStatus = GAME_SCENE_FREE;
	updateBetBtnStatus();
	showClockTime(GAME_SCENE_FREE,pGameFree->cbTimeLeave);
}
void CarGameScence::onSubGameStart(const void* pBuffer,word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_nGameStatus = GAME_SCENE_PLACE_JETTON;
	updateBetBtnStatus();
	showClockTime(GAME_SCENE_PLACE_JETTON,pGameStart->cbTimeLeave);
	showClockTimeCallBack(GAME_SCENE_PLACE_JETTON,pGameStart->cbTimeLeave-4,CC_CALLBACK_0(CarGameScence::daoShuAni,this),0); //提前4秒播倒计时动画
}
void CarGameScence::onSubGameJetton(const void* pBuffer,word wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	setAllBet(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
}
void CarGameScence::onSubGameEnd(const void* pBuffer,word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	int nCarIndex = pGameEnd->cbTableCardArray[0][0];
	cocos2d::log("car index :%d",nCarIndex);

	m_nEndIndex = (nCarIndex-1) % (MAX_CAR_NUM);
	m_nWinScore = pGameEnd->lUserScore;
	cocos2d::log("car m_nEndIndex :%d",m_nEndIndex);
	run_start();
	m_nGameStatus = GAME_SCENE_GAME_END;
	memcpy(m_arrySelfBetScore_Temp,m_arrySelfBetScore,sizeof(m_arrySelfBetScore));
	updateBetBtnStatus();
	showClockTime(GAME_SCENE_GAME_END,pGameEnd->cbTimeLeave);
}
void CarGameScence::onSubGameRecord(const void* pBuffer,word wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return;

	//设置记录
	word wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (word wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);
//		setGameHistory(pServerGameRecord->bWinMen);
	}
}

void CarGameScence::onGameStatusFree(const void* pBuffer,word wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	//消息处理
	CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

	m_nUserScore = pStatusFree->lUserMaxScore;
	setUserScore(m_nUserScore);

	m_nGameStatus = GAME_STATUS_FREE;
	showClockTime(GAME_STATUS_FREE,pStatusFree->cbTimeLeave);
	updateBetBtnStatus();
}
void CarGameScence::onGameStatusPlay( const void* pBuffer,word wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	//消息处理
	CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

	for (int i=1;i<=S_AREA_COUNT;i++)
	{
		m_arrySelfBetScore[i]= pStatusPlay->lUserJettonScore[i];
		m_arryAllBetScore[i]= pStatusPlay->lAllJettonScore[i];
		setSelfBet(i,pStatusPlay->lUserJettonScore[i]);
		setAllBet(i,pStatusPlay->lAllJettonScore[i]);
	}
	setUserScore(pStatusPlay->lUserMaxScore);
	if (pStatusPlay->cbGameStatus==GS_GAME_END)
	{
		setWinScore(pStatusPlay->lEndUserScore);
	}
	m_nGameStatus = GAME_SCENE_GAME_END;
	updateBetBtnStatus();
	//设置时间
	showClockTime(GAME_SCENE_GAME_END,pStatusPlay->cbTimeLeave);
}

void CarGameScence::updateBetBtnStatus()
{
	Node* CarBtn_ReBet = WidgetFun::getChildWidget(this,"CarBtn_ReBet");
	if (m_nGameStatus == GAME_SCENE_FREE || m_nGameStatus == GAME_SCENE_GAME_END)
	{
		setBetBtnEnble(false);
		WidgetFun::setButtonEnabled(CarBtn_ReBet,false);
	}
	else if (m_nGameStatus == GAME_SCENE_PLACE_JETTON)
	{
		setBetBtnEnble(true);
		WidgetFun::setButtonEnabled(CarBtn_ReBet,true);
	}	
	Node* CarBtn_AutoBet = WidgetFun::getChildWidget(this,"CarBtn_AutoBet");
	WidgetFun::setButtonEnabled(CarBtn_AutoBet,false);
}

void CarGameScence::setBetBtnEnble( bool bEnble )
{
	for (int i=1;i<=S_AREA_COUNT;i++)
	{
		Node* pNode = WidgetFun::getChildWidget(this,utility::toString("CarBtn_Bet",i));
		WidgetFun::setButtonEnabled(pNode,bEnble);
	}
}