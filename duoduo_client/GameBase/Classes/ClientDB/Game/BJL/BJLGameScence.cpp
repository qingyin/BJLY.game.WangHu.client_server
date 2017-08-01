#include "BJLGameScence.h"
#include "Game/GameLib.h"
#include "CMD_Baccarat.h"
#include "BJLGameLogic.h"
#include "Game/Game/QYCardTurnManager.h"
#include "Game/Script/SoundFun.h"

FV_SINGLETON_STORAGE(BJLGameScence);
USING_NS_CC;
BJLGameScence::BJLGameScence()
	:m_iActSelectGoldButton(0)
	,m_nWinScore(0)
	,m_nWinArea(-1)
	,m_nNowOpenIndex(OPEN_INDEX_NULL)
	,m_nOpenChairID(-1)
{
	m_lPlayBet = new LONGLONG[AREA_MAX];
	zeromemory(m_lPlayBet, sizeof(m_lPlayBet));
	zeromemory(m_nAllWinScore,sizeof(m_nAllWinScore));

	init();
}
BJLGameScence::~BJLGameScence()
{

}
bool BJLGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence("Script/GameBJL/BJLGameScence.xml",this);

	WidgetManager::addButtonCB("BJLButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	return true;
}
bool BJLGameScence::IsInGame()
{
	return isVisible();
}
void BJLGameScence::EnterScence(int EnterScore)
{
	m_nHistoryList.clear();
	updateBetButtonInfo(EnterScore);
	setVisible(true);
	defaultState();
	initPlayerNode();
	initHistory();
	WidgetFun::setVisible(this,"BJLZhuangListNode",false);
	m_kLastBetList.clear();
	m_kLastBetListTemp.clear();
}
void BJLGameScence::updateBetButtonInfo( int EnterScore )
{
	int buttonNum[4] = {};
	if ( EnterScore <=20000)
	{
		buttonNum[0] = 100;
		buttonNum[1] = 200;
		buttonNum[2] = 500;
		buttonNum[3] = 1000;

	}
	else if (EnterScore <= 500000)
	{
		buttonNum[0] = 2000;
		buttonNum[1] = 5000;
		buttonNum[2] = 10000;
		buttonNum[3] = 20000;
	}
	else if (EnterScore <= 20000000)
	{
		buttonNum[0] = 50000;
		buttonNum[1] = 100000;
		buttonNum[2] = 200000;
		buttonNum[3] = 500000;
	}

	for (int i = 0;i<4;i++)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BJLButton_Zhu_",i));
		WidgetFun::setWidgetUserInfo(pNode,"BetNum",utility::toString(buttonNum[i]));
		WidgetFun::setButtonTxt(pNode,utility::NumToStringMyriabit(buttonNum[i]));
	}
}


void BJLGameScence::setUserRight(dword dwUserRight )
{
	// 控制按钮 536870912
	WidgetFun::setVisible(this,"BJLButton_Admin",false);
	if( CUserRight::IsGameCheatUser(dwUserRight))
	{
		WidgetFun::setVisible(this,"BJLButton_Admin",true);
	}
}
void BJLGameScence::defaultState()
{
	utility::log("BJLGameScence::defaultState1");
	setInGameShow(false);
	utility::log("BJLGameScence::defaultState2");
	upButtonState();
	utility::log("BJLGameScence::defaultState3");
	setShowCardsNum(false,INDEX_PLAYER,0);
	utility::log("BJLGameScence::defaultState4");
	setShowCardsNum(false,INDEX_BANKER,0);
	utility::log("BJLGameScence::defaultState5");
	zeromemory(m_lPlayBet, sizeof(m_lPlayBet));
	utility::log("BJLGameScence::defaultState6");
	clearBetChips();
	utility::log("BJLGameScence::defaultState7");

	showAreaBetMax(INDEX_PLAYER,false);
	utility::log("BJLGameScence::defaultState8");
	showAreaBetMax(INDEX_BANKER,false);
	utility::log("BJLGameScence::defaultState9");
	WidgetFun::setVisible(this,"BJL_AreaIcon_0",false);
	utility::log("BJLGameScence::defaultState10");
	WidgetFun::setVisible(this,"BJL_AreaIcon_1",false);
	utility::log("BJLGameScence::defaultState11");
	WidgetFun::setVisible(this,"BJLAdminPopo",false);
	utility::log("BJLGameScence::defaultState12");

	WidgetFun::setWidgetUserInfo(this,"BJL_AreaIcon_0","Player_Pos",utility::toString(0," ",0));
	utility::log("BJLGameScence::defaultState13");
	WidgetFun::setWidgetUserInfo(this,"BJL_AreaIcon_1","Player_Pos",utility::toString(0," ",0));
	utility::log("BJLGameScence::defaultState14");
}
void BJLGameScence::clearBetChips()
{
	WidgetFun::getChildWidget(this,"GoldPlane")->removeAllChildren();
	m_pBetChipList.clear();
}
void BJLGameScence::setInGameShow(bool bShow)
{
	for (int i = 0;i<8;i++)
	{
		if (i == AREA_XIAN_TIAN ||
			i == AREA_ZHUANG_TIAN ||
			i == AREA_TONG_DUI 
			)
		{
			continue;
		}
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BJLZhuPlane",i));
		WidgetFun::setVisible(pNode,"SelfTxt",bShow);
		WidgetFun::setVisible(pNode,"TotalTxt",bShow);
		WidgetFun::setText(pNode,"SelfTxt","");
		WidgetFun::setText(pNode,"TotalTxt","");
	}
}
void BJLGameScence::showClock(int iTime,bool bShow)
{
	iTime = iTime+1;
	WidgetFun::setVisible(this,"BJLTime_Desc",bShow);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"BJLTime");
	if (!bShow)
	{
		pNode->setVisible(false);
		return;
	}
	pNode->setVisible(true);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTime::create(iTime,iTime,0));

}
void BJLGameScence::showClockTimeCallBack(const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime )
{
	nSecond = nSecond+1;
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}
void BJLGameScence::showClockTimeCallBack(Node* pRoot,const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime )
{
	nSecond = nSecond+1;
	Node* pNode = WidgetFun::getChildWidget(pRoot,kName);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}

void BJLGameScence::endClockTimeCallBack(const std::string& kName)
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
}

void BJLGameScence::endClockTimeCallBack(Node* pRoot,const std::string& kName)
{
	Node* pNode = WidgetFun::getChildWidget(pRoot,kName);
	pNode->stopAllActions();
}

void BJLGameScence::setTimeDesc( int nState )
{
	std::string kDesc;
	if (nState == GAME_SCENE_FREE)
	{
		kDesc = WidgetFun::getWidgetUserInfo(this,"BJLTime_Desc","TxtFree");
	}
	else if (nState == GAME_SCENE_BET)
	{
		kDesc = WidgetFun::getWidgetUserInfo(this,"BJLTime_Desc","TxtBet");
	}
	else if (nState == GAME_SCENE_END)
	{
		kDesc = WidgetFun::getWidgetUserInfo(this,"BJLTime_Desc","TxtResult");
	}
	else
	{
		return;
	}

	WidgetFun::setText(this,"BJLTime_Desc",kDesc);
}

void BJLGameScence::statusTip( int nState )
{
	if (nState == GAME_SCENE_BET)
	{
		WidgetFun::setVisible(this,"State_StartBet",true);
		WidgetFun::setVisible(this,"State_StopBet",false);
		SoundFun::Instance().playEffect("BJLSound/voice_kaishixiazhu.mp3");
	}
	else if (nState == GAME_SCENE_END)
	{
		WidgetFun::setVisible(this,"State_StartBet",false);
		WidgetFun::setVisible(this,"State_StopBet",true);
		SoundFun::Instance().playEffect("BJLSound/voice_tingzhixiazhu.mp3");
	}
	else
	{
		return;
	}
	WidgetFun::runWidgetAction(this,"Start_Bet_State","Start"); //开始下注

}
void BJLGameScence::upButtonState()
{
	utility::log("BJLGameScence::upButtonState1");
	if (getSelfGamePlayer() == NULL)
	{
		return;
	}
	utility::log("BJLGameScence::upButtonState2");
	int iMinSelectIdex = 4;
	int lSelfGold = getSelfGamePlayer()->GetUserScore();
	utility::log("BJLGameScence::upButtonState3");
	for (int i = 0;i<4;i++)
	{
		utility::log(utility::toString(i).c_str());
		utility::log("BJLGameScence::upButtonState4");
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BJLButton_Zhu_",i));
		utility::log("BJLGameScence::upButtonState5");
		int iGold = utility::parseInt(WidgetFun::getButtonTxt(pNode));
		utility::log("BJLGameScence::upButtonState6");
		if (lSelfGold >= iGold)
		{
			iMinSelectIdex = i;
			WidgetFun::setEnable(pNode,true);
		}
		else
		{
			WidgetFun::setEnable(pNode,false);
		}
		utility::log("BJLGameScence::upButtonState7");
	}
	if (m_iActSelectGoldButton > iMinSelectIdex)
	{
		m_iActSelectGoldButton = iMinSelectIdex;
	}
	utility::log("BJLGameScence::upButtonState8");
	WidgetFun::setEnable(this,utility::toString("BJLButton_Zhu_",m_iActSelectGoldButton),false);
	utility::log("BJLGameScence::upButtonState9");
}

void BJLGameScence::upBetAreaState(bool bEnbale)
{
	WidgetFun::setButtonEnabled(this,"BJHButton_Bet_0",bEnbale);
	WidgetFun::setButtonEnabled(this,"BJHButton_Bet_1",bEnbale);
	WidgetFun::setButtonEnabled(this,"BJHButton_Bet_2",bEnbale);
	WidgetFun::setButtonEnabled(this,"BJHButton_Bet_6",bEnbale);
	WidgetFun::setButtonEnabled(this,"BJHButton_Bet_7",bEnbale);
}											 
											
int BJLGameScence::getAreaScore(int iIdex)		 
{												 
	CCASSERT(iIdex < 4,"");
	if (iIdex >= 4)
	{
		return 0;
	}
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BJLButton_Zhu_",iIdex));
	int nNum = utility::parseInt(WidgetFun::getWidgetUserInfo(pNode,"BetNum"));

	return nNum;
}
void BJLGameScence::MoveGold(bool bNeedMove,int iArea,int iBetScore,Vec2 posFrom,int wChairID)
{		
	if (iBetScore == 0||iArea == AREA_XIAN_TIAN ||iArea == AREA_ZHUANG_TIAN ||iArea == AREA_TONG_DUI )
	{
		return;
	}
	int iTempValue = getChipIndexByScore(iBetScore);
	cocos2d::Node* pGoldNode = WidgetFun::getChildWidget(this,"GoldPlane");
	cocos2d::Node* pAreaNode = WidgetFun::getChildWidget(this,utility::toString("BJHButton_Bet_",iArea));
	CCASSERT(pAreaNode,"");

	SoundFun::Instance().playEffect("BJLSound/effect_rw_chips.mp3");
	cocos2d::Sprite* pSrite = cocos2d::Sprite::create(utility::toString("GameBJL/BJL_Zhu",iTempValue,".png"));
	pGoldNode->addChild(pSrite);
	cocos2d::Point kNodePos = pAreaNode->getPosition();
	int iAreaX = (int)pAreaNode->getContentSize().width-40;
	int iAreaY = (int)pAreaNode->getContentSize().height-40;
	kNodePos.x += (rand()%(iAreaX))-iAreaX/2;
	kNodePos.y += (rand()%(iAreaY))-iAreaY/2;
	if (!bNeedMove)
	{
		pSrite->setPosition(kNodePos);
	}
	else
	{
		pSrite->setPosition(posFrom);
		float fTime = utility::getMoveTime(posFrom,kNodePos,2000.0f);
		pSrite->runAction(cocos2d::MoveTo::create((rand()%100)/400.0f+fTime,kNodePos));
	}

	if (wChairID>=0)
	{
		BetChip kBetChip;
		kBetChip.nChairID = wChairID;
		kBetChip.pChip = pSrite;
		m_pBetChipList.push_back(kBetChip);
	}
}

void BJLGameScence::setShowCardsNum( bool bShow,int nArea,int nNum)
{
	CCASSERT(nArea == INDEX_PLAYER || nArea == INDEX_BANKER,"");
	Node* pNode	= WidgetFun::getChildWidget(this,utility::toString("CardNode",nArea));
	pNode->setVisible(bShow);

	for (int i=0;i<3;i++)
	{
		WidgetFun::setVisible(pNode,utility::toString("Card",i),false);
	}

	if (nNum == 0 && bShow)
	{
		nNum = utility::parseInt(WidgetFun::getWidgetUserInfo(pNode,"CardCount"));
	}

	for (int i=0;i<nNum&&bShow;i++)
	{
		WidgetFun::setVisible(pNode,utility::toString("Card",i),bShow);
	}
}

void BJLGameScence::setCardImge( cocos2d::Node* pCardNode,int nValue,int nColor )
{
	if ( pCardNode == NULL || !(nValue>0&&nValue<=13)||!(nColor>=0&&nColor<4))
	{
		CCASSERT(false,"");
		return;
	}
	nValue += nColor*13;
	WidgetFun::setImagic(pCardNode,"CardShow",utility::toString("GameBJL/cards/",nValue,".png"),true);
// 	if (nColor == 1|| nColor == 3)
// 	{
// 		//方块 红桃
// 		WidgetFun::setImagic(pCardNode,"Number",utility::toString("GameBJL/cards/r_",nValue,".png"));
// 	}
// 	else
// 	{
// 		//草花 黑桃
// 		WidgetFun::setImagic(pCardNode,"Number",utility::toString("GameBJL/cards/b_",nValue,".png"));
// 	}
}
void BJLGameScence::setCardInfo( BYTE cbCardCount[2], BYTE cbTableCardArray[2][3] )
{
	//保存赢区域
	int nPlayerPoint = BJLGameLogic::Instance().GetCardListPip(cbTableCardArray[INDEX_PLAYER],cbCardCount[INDEX_PLAYER]);
	int nBankerPoint = BJLGameLogic::Instance().GetCardListPip(cbTableCardArray[INDEX_BANKER],cbCardCount[INDEX_BANKER]);
	if( nPlayerPoint < nBankerPoint )
		m_nWinArea = AREA_ZHUANG;
	else if( nPlayerPoint > nBankerPoint )
		m_nWinArea = AREA_XIAN;
	else
		m_nWinArea = AREA_PING;
	

	Node* pXian		= WidgetFun::getChildWidget(this,"CardNode0");
	Node* pZhuang   = WidgetFun::getChildWidget(this,"CardNode1");

	WidgetFun::setWidgetUserInfo(pXian,	 "CardCount",utility::toString((int)cbCardCount[INDEX_PLAYER]));
	WidgetFun::setWidgetUserInfo(pZhuang,"CardCount",utility::toString((int)cbCardCount[INDEX_BANKER]));

	WidgetFun::setVisible(pXian,"ResultPoint",false);
	WidgetFun::setVisible(pZhuang,"ResultPoint",false);
	for (int n = 0; n < 2; n++)
	{
		for (int i=0;i<3;i++)
		{
			BYTE nValue = BJLGameLogic::Instance().GetCardValue(cbTableCardArray[n][i]);
			BYTE nColor = BJLGameLogic::Instance().GetCardColor(cbTableCardArray[n][i]);
			BYTE nPoint = BJLGameLogic::Instance().GetCardPip(cbTableCardArray[n][i]);
			Node* pCard = WidgetFun::getChildWidget(pXian,utility::toString("Card",i));
			if (n==1)
			{
				pCard = WidgetFun::getChildWidget(pZhuang,utility::toString("Card",i));
			}
			WidgetFun::setWidgetUserInfo(pCard,"CardPoint",utility::toString((int)nPoint));
			WidgetFun::setWidgetUserInfo(pCard,"CardValue",utility::toString((int)nValue));
			WidgetFun::setWidgetUserInfo(pCard,"CardColor",utility::toString((int)nColor));
			if (i==2 && nValue ==0 )
			{
				Node* pNode   = WidgetFun::getChildWidget(this,utility::toString("CardNode",n));
				WidgetFun::setWidgetUserInfo(pNode,"CardCount","2");
			}
			else
			{
				setCardImge(pCard,nValue,nColor);
			}
		}
	}
	
}

void BJLGameScence::sendCardBack( int nArea,int nCardIndex,bool bHaveAni)
{
	CCASSERT(nArea == INDEX_PLAYER || nArea == INDEX_BANKER,"");
	CCASSERT(nCardIndex == 0 || nCardIndex == 1|| nCardIndex == 2,"");

	Node* pNode	= WidgetFun::getChildWidget(this,utility::toString("CardNode",nArea));
	WidgetFun::setVisible(pNode,"ResultPoint",false);

	setShowCardsNum(true,nArea,nCardIndex+1);

	if ( nCardIndex == 0 || nCardIndex == 1)
	{
		Node*pCard = WidgetFun::getChildWidget(pNode,utility::toString("Card",nCardIndex));
		WidgetFun::setVisible(pCard,"CardShow",false);
		WidgetFun::setVisible(pCard,"CardBack",true);
		if (bHaveAni)
		{
			WidgetFun::CallAction(pCard,"SendAni");
		}
		else
		{
			WidgetFun::CallAction(pCard,"Send_NoAni");
		}
	}
	else if( nCardIndex == 2)
	{
		Node*pCard1 = WidgetFun::getChildWidget(pNode,utility::toString("Card",1));
		WidgetFun::CallAction(pCard1,"MoveByPos");

		Node*pCard2 = WidgetFun::getChildWidget(pNode,utility::toString("Card",nCardIndex));
		WidgetFun::setVisible(pCard2,"CardShow",false);
		WidgetFun::setVisible(pCard2,"CardBack",true);
	}
	
}

void BJLGameScence::showCardBack(int nArea,int nCardIndex,bool bShow)
{
	CCASSERT(nArea == INDEX_PLAYER || nArea == INDEX_BANKER,"");
	CCASSERT(nCardIndex == 0 || nCardIndex == 1|| nCardIndex == 2,"");

	Node* pNode	= WidgetFun::getChildWidget(this,utility::toString("CardNode",nArea));
	setShowCardsNum(true,nArea,nCardIndex+1);

	Node*pCard = WidgetFun::getChildWidget(pNode,utility::toString("Card",nCardIndex));
	WidgetFun::setVisible(pCard,"CardShow",!bShow);
	WidgetFun::setVisible(pCard,"CardBack",bShow);
}
void BJLGameScence::resultAni()
{
	CCASSERT( m_nWinArea >= AREA_XIAN && m_nWinArea <= AREA_ZHUANG,"" );
	WidgetFun::setImagic(this,"Result_State_WinArea",utility::toString("GameBJL/Result_State",m_nWinArea,".png"));
	WidgetFun::setVisible(this,"Result_State_WinNode",false);
	addHistoryWinArea(m_nWinArea);
	SoundFun::Instance().playEffect(utility::toString("BJLSound/voice_win",m_nWinArea,".mp3"));
	if (m_nWinScore > 0 )
	{
		WidgetFun::setVisible(this,"Result_State_WinNode",true);
		WidgetFun::setText(this,"Result_State_WinChipTxt",utility::toString("+",m_nWinScore));
	}
	WidgetFun::runWidgetAction(this,"Result_State","Start");

	for (int i=0;i<GAME_PLAYER;i++)
	{
		if(m_nAllWinScore[i] == 0)continue;
		BJLPlayer* pPlayer = (BJLPlayer*)getPlayerByChairID(i);
		if (pPlayer)
		{
			pPlayer->setWinScore(m_nAllWinScore[i]);
		}
	}

}

int BJLGameScence::getChipIndexByScore( int nScore )
{
	int iTempValue = 0;
	while (iTempValue < 3)
	{
		if (getAreaScore(iTempValue+1) > nScore)
		{
			break;
		}
		iTempValue++;
	}
	return iTempValue;
}
void BJLGameScence::setAreaScore( int nArea,int nAllBet,int nSelfBet,bool bRest,bool bAdd)
{
	if (nArea == AREA_XIAN_TIAN ||nArea == AREA_ZHUANG_TIAN ||nArea == AREA_TONG_DUI )
	{
		return;
	}
	cocos2d::Node* pAreaNode = WidgetFun::getChildWidget(this,utility::toString("BJLZhuPlane",nArea));
	int iTotle = utility::parseInt(WidgetFun::getText(pAreaNode,"TotalTxt"));
	int iSelf = utility::parseInt(WidgetFun::getText(pAreaNode,"SelfTxt"));
	if (bAdd)
	{
		iTotle += nAllBet;
	}
	else
	{
		iTotle = nAllBet;
	}
	if (bRest)
	{
		iSelf = 0;
	}
	else
	{
		iSelf += nSelfBet;
	}

	WidgetFun::setText(pAreaNode,"TotalTxt",iTotle);
	WidgetFun::setText(pAreaNode,"SelfTxt",iSelf);
	if (iSelf>0)
	{
		WidgetFun::setVisible(pAreaNode,"SelfTxt",true);
	}
	else
	{
		WidgetFun::setVisible(pAreaNode,"SelfTxt",false);
	}
}

void BJLGameScence::setAreaBetMax(word wXianChairID,word wZhuangChairID,longlong wXianBet,longlong wZhuangBet)
{
	if (wXianChairID != INVALID_WORD)
	{
		showAreaBetMax(INDEX_PLAYER,true);
		BJLPlayer* pPlayerXian   =(BJLPlayer*) getPlayerByChairID(wXianChairID);
		if (!pPlayerXian)
		{
			return;
		}
		WidgetFun::setText(this,"MaxBetInfo0:NickName",utility::a_u8(pPlayerXian->GetNickName()));
		WidgetFun::setText(this,"MaxBetInfo0:BetChips",(int)wXianBet);
		Vec2 posXian = pPlayerXian->getAreaIconPos();
		WidgetFun::setWidgetUserInfo(this,"BJL_AreaIcon_0","Player_Pos",utility::toString(posXian.x," ",posXian.y));
	}
	if (wZhuangChairID != INVALID_WORD)
	{
		showAreaBetMax(INDEX_BANKER,true);
		BJLPlayer* pPlayerZhuang =(BJLPlayer*) getPlayerByChairID(wZhuangChairID);
		if (!pPlayerZhuang)
		{
			return;
		}
		WidgetFun::setText(this,"MaxBetInfo1:NickName",utility::a_u8(pPlayerZhuang->GetNickName()));
		WidgetFun::setText(this,"MaxBetInfo1:BetChips",(int)wZhuangBet);

		Vec2 posZhuang = pPlayerZhuang->getAreaIconPos();

		WidgetFun::setWidgetUserInfo(this,"BJL_AreaIcon_1","Player_Pos",utility::toString(posZhuang.x," ",posZhuang.y));
	}
}

void BJLGameScence::showAreaBetMax(int nArea, bool bShow )
{
	ASSERT(nArea == INDEX_PLAYER|| nArea == INDEX_BANKER);
	WidgetFun::setVisible(this,utility::toString("MaxBetInfo",nArea),bShow);
}

void BJLGameScence::moveAreaIconToPlayer( int nArea )
{
	ASSERT(nArea == INDEX_PLAYER|| nArea == INDEX_BANKER);
	Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,utility::toString("BJL_AreaIcon_",nArea),"Player_Pos"));
// 	if (m_nMaxBet0 == 0 || m_nMaxBet1 == 0)
// 	{
// 		if (m_nMaxBet0 == 0)
// 		{
// 			WidgetFun::setVisible(this,utility::toString("BJL_AreaIcon_",0),false);
// 		}
// 		if (m_nMaxBet1 == 0)
// 		{
// 			WidgetFun::setVisible(this,utility::toString("BJL_AreaIcon_",1),false);
// 		}
// 	}
// 	else
	if (pos == Vec2(0,0))
	{
		WidgetFun::setVisible(this,utility::toString("BJL_AreaIcon_",nArea),false);
	}
	else
	{
		WidgetFun::runWidgetAction(this,utility::toString("BJL_AreaIcon_",nArea),"MoveToPlayer");
	}
}

void BJLGameScence::OnOpenCard(int nOpenIndex,int nChairID)
{
	bool isSelf = nChairID == getSelfChairID();
//	WidgetFun::setVisible(this,"BJLOpenShadowInfo:BJL_OpenCard",isSelf);
	QYCardTurnManager::Instance().showOpenBtn(isSelf);

	if(isSelf)
	{
		openActionByOpenIndex(nOpenIndex,"MoveToOpen_Self");
	}
	else
	{
		openActionByOpenIndex(nOpenIndex,"MoveToOpen");
	}
}

void BJLGameScence::openActionByCardIndex(int nArea,int nCardIndex,const std::string& kActionName)
{
	ASSERT(nArea == INDEX_PLAYER|| nArea == INDEX_BANKER);
	ASSERT(nCardIndex == 0|| nCardIndex == 1||nCardIndex == 2 );

	Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",nArea));
	Node* pCard = WidgetFun::getChildWidget(pCardNode,utility::toString("Card",nCardIndex));

	if (kActionName == "MoveToArea")
	{
		WidgetFun::runWidgetAction(pCard,kActionName);
		WidgetFun::setVisible(pCard,"CardShow",true);
		WidgetFun::setVisible(pCard,"CardBack",false);

		WidgetFun::runWidgetAction(this,"BJLOpenShadow","HideShadow");
		QYCardTurnManager::Instance().showShadowInfo(false);

		//隐藏翻牌
		QYCardTurnManager::Instance().HideCard();
	}
	else if (kActionName == "MoveToOpen"|| kActionName == "MoveToOpen_Self" )
	{
		if (nCardIndex == 2)
		{
			//开第三张时往左移第二张
			sendCardBack(nArea,2);
			//发牌背动画和开牌动画是一个
			WidgetFun::runWidgetAction(pCard,utility::toString(kActionName,"_3Card"));
		}
		else
		{
			WidgetFun::runWidgetAction(pCard,kActionName);
		}
		WidgetFun::setVisible(pCard,"CardShow",false);
		WidgetFun::setVisible(pCard,"CardBack",true);
	}
}


void BJLGameScence::openActionByOpenIndex(int nOpenIndex,const std::string& kActionName)
{
	ASSERT(nOpenIndex == OPEN_XIAN_12CARD || nOpenIndex == OPEN_XIAN_3CARD 
		|| nOpenIndex == OPEN_ZHUANG_12CARD|| nOpenIndex == OPEN_ZHUANG_3CARD);

	if (nOpenIndex == OPEN_XIAN_12CARD)
	{
		setShowCardsNum(true,INDEX_PLAYER,2);
		openActionByCardIndex(INDEX_PLAYER,0,kActionName);
		openActionByCardIndex(INDEX_PLAYER,1,kActionName);
		if(kActionName == "MoveToArea")
		{
			//开牌完成显示点数
			Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",INDEX_PLAYER));
			WidgetFun::runWidgetAction(pCardNode,"ResultPoint","Start");
		}
	}
	else if (nOpenIndex == OPEN_ZHUANG_12CARD)
	{
		setShowCardsNum(true,INDEX_BANKER,2);
		openActionByCardIndex(INDEX_BANKER,0,kActionName);
		openActionByCardIndex(INDEX_BANKER,1,kActionName);
	}
	else if (nOpenIndex == OPEN_XIAN_3CARD)
	{
		setShowCardsNum(true,INDEX_PLAYER,3);
		openActionByCardIndex(INDEX_PLAYER,2,kActionName);
	}
	else if (nOpenIndex == OPEN_ZHUANG_3CARD)
	{
		setShowCardsNum(true,INDEX_BANKER,3);
		openActionByCardIndex(INDEX_BANKER,2,kActionName);
	}

	if(kActionName == "MoveToArea")
	{
		//开牌完成显示点数
		showPointByOpenEnd(nOpenIndex);
	}
}

void BJLGameScence::openCardTimeOver(int nOpenIndex,int nChairID)
{
	openActionByOpenIndex(nOpenIndex,"MoveToArea");

	BJLPlayer* pPlayer =(BJLPlayer*)getPlayerByChairID(nChairID);
	if (pPlayer)
	{
		pPlayer->LookingCard(false);
	}

	Node* pXianNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",INDEX_PLAYER));
	Node* pZhuangNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",INDEX_BANKER));
	int nXianCount  = utility::parseInt(WidgetFun::getWidgetUserInfo(pXianNode,"CardCount"));
	int nZhuangCount  = utility::parseInt(WidgetFun::getWidgetUserInfo(pZhuangNode,"CardCount"));
	
	//结算条件判断
	if (nOpenIndex == OPEN_ZHUANG_12CARD && nXianCount == 2 && nZhuangCount == 2  )
	{
		resultAni();
	}
	else if ( nOpenIndex == OPEN_XIAN_3CARD && nZhuangCount == 2)
	{
		resultAni();
	}
	else if(nOpenIndex == OPEN_ZHUANG_3CARD)
	{
		resultAni();
	}
}

void BJLGameScence::showPointByOpenEnd( int nOpenIndex )
{
	ASSERT(nOpenIndex == OPEN_XIAN_12CARD || nOpenIndex == OPEN_XIAN_3CARD 
		|| nOpenIndex == OPEN_ZHUANG_12CARD|| nOpenIndex == OPEN_ZHUANG_3CARD);


	if (nOpenIndex == OPEN_XIAN_12CARD)
	{
		Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",(int)INDEX_PLAYER));
		int nPoint = getAreaPoint(INDEX_PLAYER,2);
	
		WidgetFun::setAtlasTxt(this,"ResultPoint",utility::toString(nPoint));
		WidgetFun::runWidgetAction(pCardNode,"ResultPoint","Start");	
	}
	else if (nOpenIndex == OPEN_ZHUANG_12CARD)
	{
		Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",(int)INDEX_BANKER));
		int nPoint = getAreaPoint(INDEX_BANKER,2);

		WidgetFun::setAtlasTxt(pCardNode,"ResultPoint",utility::toString(nPoint));
		WidgetFun::runWidgetAction(pCardNode,"ResultPoint","Start");
	}
	else if (nOpenIndex == OPEN_XIAN_3CARD)
	{
		int nPoint = getAreaPoint(INDEX_PLAYER,3);
		int iTempIdex = INDEX_PLAYER;
		CallFunc* pFun = CallFunc::create(CC_CALLBACK_0(BJLGameScence::set3CardPoint,this,iTempIdex,nPoint));
		runAction(Sequence::create(DelayTime::create(0.5f),pFun,NULL));
	}
	else if (nOpenIndex == OPEN_ZHUANG_3CARD)
	{
		int nPoint = getAreaPoint(INDEX_BANKER,3);
		int iTempIdex = INDEX_PLAYER;
		CallFunc* pFun = CallFunc::create(CC_CALLBACK_0(BJLGameScence::set3CardPoint,this,iTempIdex,nPoint));
		runAction(Sequence::create(DelayTime::create(0.5f),pFun,NULL));
	}
}

int BJLGameScence::getAreaPoint( int nArea,int nCardNum )
{
	ASSERT(nArea == INDEX_PLAYER|| nArea == INDEX_BANKER);
	ASSERT( nCardNum == 1||nCardNum == 2 || nCardNum == 3);

	Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",nArea));

	int nPoint = 0;
	for (int i=0;i<nCardNum;i++)
	{
		Node* pCard = WidgetFun::getChildWidget(pCardNode,utility::toString("Card",i));
		nPoint +=  utility::parseInt(WidgetFun::getWidgetUserInfo(pCard,"CardPoint"));
	}
	return nPoint%10;
}

void BJLGameScence::set3CardPoint( int nArea,int nPoint )
{
	ASSERT(nArea == INDEX_PLAYER|| nArea == INDEX_BANKER);

	Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("CardNode",nArea));
	WidgetFun::setAtlasTxt(pCardNode,"ResultPoint",utility::toString(nPoint));
}

void BJLGameScence::initHistory()
{
	m_nHistoryList.clear();
	refreshHistory();

}
void BJLGameScence::addHistoryWinArea( int nAreaWin )
{
	m_nHistoryList.push_back(nAreaWin);
	if (m_nHistoryList.size()>MAX_HISTORY_NUM)
	{
		for (int i=0;i<HISTORY_ROW_NUM;i++)
		{
			m_nHistoryList.pop_front();
		}
	}
	refreshHistory();
}

void BJLGameScence::refreshHistory()
{
	int nXianNum = 0;
	int nZhuangNum = 0;
	int nPingNum = 0;

	std::list<int>::iterator iter = m_nHistoryList.begin();
	int nSize = m_nHistoryList.size();
	for (int i=0;i<MAX_HISTORY_NUM;i++)
	{
		
		int nRowX = i/6;
		int nRowY = i%6;
		Node* pNodeX = WidgetFun::getChildWidget(this,utility::toString("WinPointRow",nRowX));
		Node* pNodeY = WidgetFun::getChildWidget(pNodeX,utility::toString("Point",nRowY));

		pNodeX->setVisible(true);
		if (i>=nSize)
		{
			pNodeY->setVisible(false);
		}
		else
		{
			pNodeY->setVisible(true);

			if ((*iter)==AREA_XIAN)
			{
				nXianNum++;
				WidgetFun::setImagic(pNodeY,"GameBJL/res_baccarat_BJL017.png",true);
			}	
			else if ((*iter)==AREA_ZHUANG)
			{
				nZhuangNum++;
				WidgetFun::setImagic(pNodeY,"GameBJL/res_baccarat_BJL019.png",true);
			}
			else if ((*iter)==AREA_PING)
			{
				nPingNum++;
				WidgetFun::setImagic(pNodeY,"GameBJL/res_baccarat_BJL018.png",true);
			}

			iter++;
		}

		
	}
	WidgetFun::setText(this,"HistoryArea_Xian:AreaWinNum",nXianNum);
	WidgetFun::setText(this,"HistoryArea_Zhuang:AreaWinNum",nZhuangNum);
	WidgetFun::setText(this,"HistoryArea_Ping:AreaWinNum",nPingNum);

}








