#include "CarGameScence.h"
#include "Game/GameLib.h"
#include "CMD_RedNine100.h"

USING_NS_CC;
FV_SINGLETON_STORAGE(CarGameScence);

const int CarGameScence::UNIT_BET[4]={100,500,1000,5000};

CarGameScence::CarGameScence()
	:m_nStartIndex(0)
	,m_nEndIndex(-1)
	,m_nUnitBet(UNIT_BET[0])
	,m_nWinScore(0)
	,m_nUserScore(0)
	,m_nKeYongScore(0)
{
	init();
}
CarGameScence::~CarGameScence()
{

}
bool CarGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence("Script/GameCar/CarGameScence.xml",this);
	initButton();
	initData();
	initNet();
	defaultInit();
	return true;
}

bool CarGameScence::IsInGame()
{
	return isVisible();
}
void CarGameScence::defaultInit()
{
	zeromemory(m_arrySelfBetScore,sizeof(m_arrySelfBetScore));
	zeromemory(m_arryAllBetScore,sizeof(m_arryAllBetScore));
	zeromemory(m_arrySelfBetScore_Temp,sizeof(m_arrySelfBetScore_Temp));
	setUnitBet(m_nUnitBet);
}
void CarGameScence::initData()
{
	m_pCarNodeList.clear();
	for (int i=0;i<MAX_CAR_NUM;i++)
	{
		Node* pCarNode = WidgetFun::getChildWidget(this,utility::toString("CarItem",i+1));
		WidgetFun::getChildWidget(pCarNode,"CarBg")->setVisible(false);
		WidgetFun::getChildWidget(pCarNode,"CarEnable")->setVisible(false);
		WidgetFun::getChildWidget(pCarNode,"CarDisable")->setVisible(true);

		m_pCarNodeList.push_back(pCarNode);
	}
	m_pCarRecordList.clear();
	for (int i=0;i<S_AREA_COUNT;i++)
	{
		Node* pCarRecordItem = WidgetFun::getChildWidget(this,utility::toString("CarRecordItem",i+1));
		pCarRecordItem->setVisible(false);
		m_pCarRecordList.push_back(pCarRecordItem);
	}
	m_nRecordIndexList.clear();
}
void CarGameScence::hideAllCarItem()
{
	for (int i=0;i<MAX_CAR_NUM;i++)
	{
		Node* pCarNode = m_pCarNodeList[i];
		WidgetFun::getChildWidget(pCarNode,"CarBg")->setVisible(false);
		WidgetFun::getChildWidget(pCarNode,"CarEnable")->setVisible(false);
	}

}
void CarGameScence::showCarItem(int nIndex,bool bAni)
{
	if (nIndex<0)return;

	int nIndexTemp = (nIndex ) % MAX_CAR_NUM;
	Node* pCarNode = m_pCarNodeList[nIndexTemp];
	Sprite* pCarBg =(Sprite*)WidgetFun::getChildWidget(pCarNode,"CarBg");
	Sprite* CarEnable =(Sprite*)WidgetFun::getChildWidget(pCarNode,"CarEnable");

	if (bAni)
	{
		WidgetFun::CallAction(pCarBg,"Start");
		WidgetFun::CallAction(CarEnable,"Start");
	}
	else
	{
		WidgetFun::CallAction(pCarBg,"Show");
		WidgetFun::CallAction(CarEnable,"Show");
	}
}
void CarGameScence::blinkItem(int nIndex)
{
	Node* pCarNode = m_pCarNodeList[nIndex];
	Sprite* pCarBg =(Sprite*)WidgetFun::getChildWidget(pCarNode,"CarBg");
	Sprite* CarEnable =(Sprite*)WidgetFun::getChildWidget(pCarNode,"CarEnable");

	WidgetFun::CallAction(pCarBg,"blink");
	WidgetFun::CallAction(CarEnable,"blink");
}


void CarGameScence::carLogoAni()
{
	showCarItem(m_nEndIndex,false);

	Node* pLogoImg = WidgetFun::getChildWidget(this,"CarLogoImg");

	std::string kLogoImgFile = WidgetFun::getWidgetUserInfo(m_pCarNodeList[m_nEndIndex],"LogoImg");
	WidgetFun::setImagic(pLogoImg,"CarLogoStencil",kLogoImgFile);

	WidgetFun::CallAction(pLogoImg,"CarLogoStencil","Start");
	WidgetFun::CallAction(pLogoImg,"CarLogoEff","Start");
}

void CarGameScence::EnterScence()
{
	setVisible(true);
}

void CarGameScence::showClockTime( int nStatus,int nSecond )
{
	Node* pNode = WidgetFun::getChildWidget(this,"Car_TxtAtlasTime");
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTime::create(nSecond,nSecond,0));

	setClockDesc(nStatus);
}

void CarGameScence::showClockTimeCallBack(int nStatus,int nSecond,const std::function<void()>& pCallBack,float fCheakTime)
{

	Node* pNode = WidgetFun::getChildWidget(this,"Car_TxtAtlasTime");
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));

	setClockDesc(nStatus);
}
void CarGameScence::setClockDesc(int nStatus)
{
	Node* Car_TimeStatus = WidgetFun::getChildWidget(this,"Car_TimeStatus");
	int nValue = 1;
	if (nStatus == GAME_SCENE_FREE || GAME_STATUS_FREE)
	{
		nValue = 1;
	}
	else if ( nStatus == GAME_SCENE_PLACE_JETTON )
	{
		nValue = 2;
	}
	else if ( nStatus == GAME_SCENE_GAME_END )
	{
		nValue = 3;
	}
	std::string kStr = ScriptData<std::string>(utility::toString("Car_TimeStatus",nValue)).Value();
	WidgetFun::setText(Car_TimeStatus,kStr);
}
void CarGameScence::setSelfBet(int nIndex ,SCORE nScore)
{
	if (!(nIndex>=1 && nIndex<=S_AREA_COUNT)) return;
	m_arrySelfBetScore[nIndex] += nScore;
	WidgetFun::setAtlasTxt(this,utility::toString("CarBetTxt",nIndex),utility::toString(m_arrySelfBetScore[nIndex]));
}

void CarGameScence::setAllBet( int nIndex,SCORE nScore )
{
	if (!(nIndex>=1 && nIndex<=S_AREA_COUNT)) return;
	m_arryAllBetScore[nIndex] += nScore;
	WidgetFun::setAtlasTxt(this,utility::toString("CarAllBetTxt",nIndex),utility::toString(m_arryAllBetScore[nIndex]));
}
void CarGameScence::setUnitBet( SCORE nScore )
{
	std::string kstr = ScriptData<std::string>("CarUnitBet").Value();
	WidgetFun::setText(this,utility::toString("Car_TxtDanzhu"),utility::toString(kstr,nScore));
}

void CarGameScence::clearBetInfo()
{
	zeromemory(m_arrySelfBetScore,sizeof(m_arrySelfBetScore));
	zeromemory(m_arryAllBetScore,sizeof(m_arryAllBetScore));
	for (int i=1;i<=S_AREA_COUNT;i++)
	{
		setSelfBet(i,m_arrySelfBetScore[i]);
		setAllBet(i,m_arryAllBetScore[i]);
	}
}

void CarGameScence::setUserScore( SCORE nScore )
{
	WidgetFun::setAtlasTxt(this,utility::toString("Car_TxtAtlasGold"),utility::toString(nScore));
	WidgetFun::setText(this,utility::toString("Car_TxtUserGold"),utility::toString(nScore));
}

void CarGameScence::setWinScore( SCORE nScore )
{
	WidgetFun::setAtlasTxt(this,utility::toString("Car_TxtAtlasWin"),utility::toString(nScore));
}

void CarGameScence::setUserNickeName(const std::string& username)
{
	WidgetFun::setText(this,utility::toString("Car_TxtNickName"),username);
}
void CarGameScence::setUserKeYong(SCORE nScore)
{
	WidgetFun::setText(this,utility::toString("Car_TxtKeYong"),utility::toString(nScore));
}
void CarGameScence::setUserResoult(SCORE nScore)
{
	WidgetFun::setText(this,utility::toString("Car_TxtResult"),utility::toString(nScore));
}

void CarGameScence::atuoBet()
{
	for (int i=1;i<=S_AREA_COUNT;i++)
	{
		Command_UserBet(i,m_arrySelfBetScore_Temp[i]);
	}
}

void CarGameScence::addRecordIndex( int nIndex )
{
	m_nRecordIndexList.push_back(nIndex);
	if ((int)m_nRecordIndexList.size() > S_AREA_COUNT + 1 )
	{
		m_nRecordIndexList.pop_front();
	}
}

void CarGameScence::setGameHistory()
{
	if ((int)m_nRecordIndexList.size() <= S_AREA_COUNT )
	{
		 oldItemMove(false);
	}
	else
	{
		 m_nRecordIndexList.pop_front();
		 oldItemMove(true);
	}
}
void CarGameScence::oldItemMove( bool bMove )
{
	std::list<int>::iterator iter = m_nRecordIndexList.begin();
	int i=0;
	for (; iter!= m_nRecordIndexList.end() && i<S_AREA_COUNT;iter++,i++)
	{
		std::string strFile = WidgetFun::getWidgetUserInfo(m_pCarNodeList[*iter],"RecordFile");
		WidgetFun::setWidgetUserInfo(m_pCarRecordList[i],"NewReordFile",strFile);
		WidgetFun::setVisible(m_pCarRecordList[i],true);
		WidgetFun::setVisible(m_pCarRecordList[i],"NewIcon",false);

		if (bMove && i > 0)
		{
			Vec2 pos =m_pCarRecordList[i]->getPosition();
			WidgetFun::setWidgetUserInfo(m_pCarRecordList[i],"OldPos", utility::toString(pos.x," ",pos.y));
			WidgetFun::CallAction(m_pCarRecordList[i],"MoveByX");
		}
		else if (bMove && i == 0 )
		{
			WidgetFun::CallAction(m_pCarRecordList[i],"ScaleFirst");
		}
		else
		{
			WidgetFun::setImagic(m_pCarRecordList[i],strFile,true);
		}

	}
	newItemMove(i-1,bMove);
}
void CarGameScence::newItemMove( int nNewItemIndex,bool bMoveByX )
{
	if (nNewItemIndex<0) return;

	Node* pTemp = WidgetFun::getChildWidget(this,"CarRecordItemTemp");
	Node* pNewItem = m_pCarRecordList[nNewItemIndex];
	pNewItem->setVisible(true);
	if(!bMoveByX)
	{
		pNewItem->setVisible(false);
	}
	int nRecordCar = m_nRecordIndexList.back();

	Vec2 pos = pNewItem->getPosition();
	Vec2 from = m_pCarNodeList[nRecordCar]->getPosition();

	std::string file =WidgetFun::getWidgetUserInfo(m_pCarNodeList[nRecordCar],"RecordFile"); 
	WidgetFun::setImagic(pTemp,file,true);

	WidgetFun::setWidgetUserInfo(pTemp,"FromPos", utility::toString(from.x," ",from.y));
	WidgetFun::setWidgetUserInfo(pTemp,"ToPos", utility::toString(pos.x," ",pos.y));

	WidgetFun::CallAction(pTemp,"NewItemMove");
	WidgetFun::CallAction(pNewItem,"ShowCarRecord");                      
	WidgetFun::CallAction(pNewItem,"NewIcon","ShowNewIccon");				//ÏÔÊ¾×îÐÂ
}

void CarGameScence::daoShuAni()
{
	cocos2d::log("daoShuAni");
	WidgetFun::CallAction(this,"CarDaoShu","Start");
}











