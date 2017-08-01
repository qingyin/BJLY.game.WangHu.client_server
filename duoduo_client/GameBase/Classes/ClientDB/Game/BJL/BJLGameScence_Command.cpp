#include "BJLGameScence.h"
#include "Game/GameLib.h"
#include "Game/Game/QYCardTurnManager.h"

#include "CMD_Baccarat.h"

void BJLGameScence::Command_PlaceBet(int iArea,int iBetScore)
{
	CMD_C_PlaceBet PlaceBet;
	zeromemory(&PlaceBet,sizeof(PlaceBet));

	//构造变量
	PlaceBet.cbBetArea = iArea;
	PlaceBet.lBetScore = iBetScore;

	BJLUserBetInfo kInfo;
	kInfo.nArea = iArea;
	kInfo.nScore = iBetScore;
	m_kLastBetListTemp.push_back(kInfo);

	SendSocketData(SUB_C_PLACE_JETTON,&PlaceBet,sizeof(PlaceBet));
}

void BJLGameScence::Command_ControlArea(int iArea)
{
	CMD_C_AdminReq kInfo;
	zeromemory(&kInfo,sizeof(kInfo));
	kInfo.cbReqType = RQ_SET_WIN_AREA;
	kInfo.cbExtendData[0]=(BYTE)iArea;
	kInfo.cbExtendData[1]= 1;

	SendSocketData(SUB_C_AMDIN_COMMAND,&kInfo,sizeof(kInfo));
}
void BJLGameScence::touchOpenCardCallBack(int nIndex,int nTouchType,cocos2d::Vec2 touchPos)
{
	if (nTouchType == QYCardTurnManager::TYPE_MOVE && m_pTuchPosTemp.distance(touchPos)<20.f)
	{
		return;
	}
	m_pTuchPosTemp = touchPos;

	CMD_C_TuchTurnCard kInfo;
	zeromemory(&kInfo,sizeof(kInfo));

	kInfo.nIndex = nIndex;
	kInfo.nTuchType = nTouchType;
	kInfo.touchX = touchPos.x;
	kInfo.touchY = touchPos.y;

	SendSocketData(SUB_C_TOUCH_TURNCARD,&kInfo,sizeof(kInfo));
}


void BJLGameScence::BJL_TurnCard(cocos2d::Ref*,WidgetUserInfo*)
{
	Node* pFront1 = Node::create();
	WidgetManager::Instance().createWidget("BJLPlayerCard",pFront1);
	WidgetFun::setVisible(pFront1,"CardBack",false);

	Node* pFront2 = Node::create();
	WidgetManager::Instance().createWidget("BJLPlayerCard",pFront2);
	WidgetFun::setVisible(pFront2,"CardBack",false);

	Sprite* pSprite1 = Sprite::create("GameBJL/cards/cardback.png");
	Sprite* pSprite3 = Sprite::create("GameBJL/cards/cardback.png");

	if (m_nNowOpenIndex == OPEN_XIAN_12CARD)
	{
		Node* pXian = WidgetFun::getChildWidget(this,"CardNode0");
		int nValue1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card0","CardValue"));
		int nColor1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card0","CardColor"));
		int nValue2 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card1","CardValue"));
		int nColor2 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card1","CardColor"));
		setCardImge(pFront1,nValue1,nColor1);
		setCardImge(pFront2,nValue2,nColor2);
		QYCardTurnManager::Instance().showCard(true,pSprite1,pFront1,pSprite3,pFront2);

	}
	else if (m_nNowOpenIndex == OPEN_XIAN_3CARD)
	{
		Node* pXian = WidgetFun::getChildWidget(this,"CardNode0");
		int nValue1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card2","CardValue"));
		int nColor1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card2","CardColor"));
		setCardImge(pFront1,nValue1,nColor1);
		QYCardTurnManager::Instance().showCard(true,pSprite1,pFront1,NULL,NULL);
	}
	else if(m_nNowOpenIndex == OPEN_ZHUANG_12CARD)
	{
		Node* pXian = WidgetFun::getChildWidget(this,"CardNode1");
		int nValue1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card0","CardValue"));
		int nColor1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card0","CardColor"));
		int nValue2 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card1","CardValue"));
		int nColor2 = utility::parseInt(WidgetFun::getWidgetUserInfo(pXian,"Card1","CardColor"));
		setCardImge(pFront1,nValue1,nColor1);
		setCardImge(pFront2,nValue2,nColor2);
		QYCardTurnManager::Instance().showCard(true,pSprite1,pFront1,pSprite3,pFront2);
	}
	else if(m_nNowOpenIndex == OPEN_ZHUANG_3CARD)
	{
		Node* pZhuang = WidgetFun::getChildWidget(this,"CardNode1");
		int nValue1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pZhuang,"Card2","CardValue"));
		int nColor1 = utility::parseInt(WidgetFun::getWidgetUserInfo(pZhuang,"Card2","CardColor"));
		setCardImge(pFront1,nValue1,nColor1);
		QYCardTurnManager::Instance().showCard(true,pSprite1,pFront1,NULL,NULL);
	}
	QYCardTurnManager::Instance().setCallFun(CC_CALLBACK_3(BJLGameScence::touchOpenCardCallBack,this));
	QYCardTurnManager::Instance().setEndCallBack(CC_CALLBACK_0(BJLGameScence::turnCard1End,this),CC_CALLBACK_0(BJLGameScence::turnCard2End,this));
	
	if (m_nOpenChairID != getSelfChairID())
	{
		QYCardTurnManager::Instance().setTouchEnabled(false);
	}

	BJLPlayer* pPlayer =(BJLPlayer*)getPlayerByChairID(m_nOpenChairID);
	if (pPlayer)
	{
		QYCardTurnManager::Instance().setShadowInfoScale(1);
		pPlayer->LookingCard(true);
		QYCardTurnManager::Instance().setOpenPlayerName(pPlayer->GetNickName());
	}
	else
	{
		QYCardTurnManager::Instance().setShadowInfoScale(0);
		autoOpenCard(0);
	}

	WidgetFun::runWidgetAction(this,"BJLOpenShadow","ShowShadow");
	QYCardTurnManager::Instance().showShadowInfo(true);
}
void BJLGameScence::BJL_SubCard(cocos2d::Ref*,WidgetUserInfo*)
{
	int iLeft = utility::parseInt(WidgetFun::getText(this,"BJL_LeftCard"));
	iLeft--;
	WidgetFun::setText(this,"BJL_LeftCard",utility::toString(iLeft));
}

bool BJLGameScence::autoOpenCard(int nOpenIndex)
{
	QYCardTurnManager::Instance().AutoOpen();
	return false;
}

void BJLGameScence::turnCard1End()
{
	if (QYCardTurnManager::Instance().isCardTurndEnd())
	{
		endClockTimeCallBack(QYCardTurnManager::pInstance(),"BJLOpenShadowInfo:TimeNum");
		QYCardTurnManager::Instance().HideCard();
		openCardTimeOver(m_nNowOpenIndex,m_nOpenChairID);
		if (getSelfChairID() == m_nOpenChairID)
		{
			CMD_C_TurnCardEnd kNetInfo;
			kNetInfo.nIndex = m_nNowOpenIndex;
			kNetInfo.cbChairID = getSelfChairID();
			SendSocketData(SUB_C_TURNCARD_END,&kNetInfo,sizeof(kNetInfo));
		}
	}
}
void BJLGameScence::turnCard2End()
{
	if (QYCardTurnManager::Instance().isCardTurndEnd())
	{
		endClockTimeCallBack(QYCardTurnManager::pInstance(),"BJLOpenShadowInfo:TimeNum");
		QYCardTurnManager::Instance().HideCard();
		openCardTimeOver(m_nNowOpenIndex,m_nOpenChairID);
		if (getSelfChairID() == m_nOpenChairID)
		{
			CMD_C_TurnCardEnd kNetInfo;
			kNetInfo.nIndex = m_nNowOpenIndex;
			kNetInfo.cbChairID = getSelfChairID();
			SendSocketData(SUB_C_TURNCARD_END,&kNetInfo,sizeof(kNetInfo));
		}
	}
}