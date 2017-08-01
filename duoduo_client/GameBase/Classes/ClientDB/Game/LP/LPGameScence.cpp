#include "LPGameScence.h"
#include "Game/GameLib.h"
#include "LPYaCell.h"
#include "LPGameLogic.h"
#include "Game/Script/SoundFun.h"

FV_SINGLETON_STORAGE(LPGameScence);
USING_NS_CC;
LPGameScence::LPGameScence()
	:m_nChipsType(YaChip::gameChips1)
	,m_bDrag(false)
	,m_pCurrentYaType(ya_unknow)
	,m_nEndNum(-1)
	,m_lWinScore(0)
	,m_lTotalScore(0)
{
	init();
}
LPGameScence::~LPGameScence()
{

}
bool LPGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence("Script/GameLP/LPGameScence.xml",this);


	initButton();
	initNet();
	initTouch();
	initCell();
	return true;
}
void LPGameScence::EnterScence()
{
	m_bDrag = false;
	setVisible(true);
	defaultState();
}
bool LPGameScence::IsInGame()
{
	return isVisible();
}
void LPGameScence::defaultState()
{
	cleanYaChips(m_pYaChipList);
	cleanYaChips(m_kBetChipMap);
	m_kHistoryList.clear();
	updateHistotry();
	LPButton_CloseHistory(NULL,NULL);

	m_kLastBetList.clear();
	m_kLastBetListTemp.clear();

	m_nEndNum = -1;
	m_lWinScore = 0;

	WidgetFun::setVisible(this,"LPWheelPointNode",false);
	WidgetFun::setVisible(this,"LPPointNode",false);

}
void LPGameScence::showClock(int iTime,bool bShow)
{
	WidgetFun::setVisible(this,"LPTimeNode",bShow);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"LPTime_Num");
	if (!bShow)
	{
		WidgetFun::runWidgetAction(this,"LPTimeRotate","Stop");
		pNode->setVisible(false);
		return;
	}
	WidgetFun::runWidgetAction(this,"LPTimeRotate","Start");
	pNode->setVisible(true);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTime::create(iTime,iTime,0));

}
void LPGameScence::showClockTimeCallBack(const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}
void LPGameScence::initCell()
{
	for (int i=0;i<=MAX_CELL_NUM;i++)
	{
		WidgetFun::setVisible(this,utility::toString("LP_",i),false);

		YaCell* pCell = new YaCell(utility::toString("LP_",i),WidgetFun::getChildWidget(this,utility::toString("LP_",i)));
		m_pYaCellList.push_back(pCell);
	}
}
void LPGameScence::showCheck(bool bShow)
{
	for (int i=0;i<(int)m_pYaCellList.size();i++)
	{
		m_pYaCellList[i]->show(bShow);
	}
}

void LPGameScence::showCell(std::vector<int>& kIndexList,bool bAni)
{
	for (int i=0;i<(int)kIndexList.size();i++)
	{
		WidgetFun::setVisible(this,utility::toString("LP_",i),true);
		if (bAni)
		{
			WidgetFun::runWidgetAction(this,utility::toString("LP_",i),"Start");
		}
	}
}

bool LPGameScence::checkCellShadow( const cocos2d::Vec2& kPos,const cocos2d::Size& kSize )
{
	Rect kRect = Rect(kPos.x-kSize.width/2,kPos.y-kSize.height/2,kSize.width,kSize.height);

	m_pTouchCellList.clear();
	for (int i = 0; i <(int) m_pYaCellList.size(); i++)
	{
		YaCell* pCell = m_pYaCellList[i];
		if (pCell->checkByRect(kRect))
		{
			m_pTouchCellList.push_back(pCell);
		}

	}
	m_pCurrentYaType = checkTypeByYaCell();
	if (m_pCurrentYaType == ya_unknow)
	{
		m_pTouchCellList.clear();
		for (int i = 0; i <(int) m_pYaCellList.size(); i++)
		{
			YaCell* pCell = m_pYaCellList[i];
			if (pCell->checkByPos(kPos))
			{
				m_pTouchCellList.push_back(pCell);
				break;
			}

		}
		m_pCurrentYaType = checkTypeByYaCell();
	}

//	showCheck(true);

	return m_pCurrentYaType != ya_unknow;
}

int LPGameScence::checkTypeByYaCell()
{
	m_pCurrentChip->clearNumList();

	int nLenth = m_pTouchCellList.size();
	int nYaType = ya_unknow;
	if (nLenth == 1)
	{
		nYaType = CheckYa_1(m_pTouchCellList);
	}
	else if (nLenth == 2)
	{
		nYaType = CheckYa_2(m_pTouchCellList);
		if (nYaType == ya_unknow)
		{
			nYaType = CheckYa_3(m_pTouchCellList);
		}
	}
	else if ( nLenth == 3 )
	{
		nYaType = CheckYa_6(m_pTouchCellList);
		if (nYaType == ya_unknow)
		{
			nYaType = CheckYa_3(m_pTouchCellList);
		}
	}
	else if ( nLenth == 4 )
	{
		nYaType = CheckYa_4(m_pTouchCellList);
	}
	return nYaType;
}

void LPGameScence::showCellByNumList( std::vector<int>& kNumList )
{
	for (int i=0;i<(int)m_pYaCellList.size();i++)
	{
		for (int j=0;j<(int)kNumList.size();j++)
		{
			if (m_pYaCellList[i]->getName() == utility::toString("LP_",kNumList[j]) )
			{
				m_pYaCellList[i]->show(true);
			}
		}
	}
}

int LPGameScence::CheckYa_1(const std::vector<YaCell*>& pCellList)
{
	int nNum  = m_pTouchCellList[0]->getNumber();
	if (nNum == 50)
	{
		m_pTouchCellList[0]->show(false);
		return ya_unknow;
	}
	m_pTouchCellList[0]->show(true);
	if (nNum>37 && nNum< 50 )
	{
		std::vector<int> kNumsList = LPGameLogic::Instance().getNumListByName(m_pTouchCellList[0]->getName());
		m_pCurrentChip->setNumList(kNumsList);
		showCellByNumList(kNumsList);
	}
	else
	{
		m_pCurrentChip->insertNum(nNum);
	}
	return ya_1;
}

int  LPGameScence::CheckYa_2(const std::vector<YaCell*>& pCellList )
{
	int nLenth = pCellList.size();

	if (nLenth != 2)
	{
		return ya_unknow;
	}

	pCellList[0]->show(false);
	pCellList[1]->show(false);
	int nNum1 = pCellList[0]->getNumber();
	int nNum2 = pCellList[1]->getNumber();

	std::vector<int> kVecTemp;
	kVecTemp.push_back(nNum1);
	kVecTemp.push_back(nNum2);
	std::sort(kVecTemp.begin(),kVecTemp.end());
	
	if( nNum1>=38 || nNum2>=38)
	{
		return ya_unknow;
	}
	if ((kVecTemp[0]== 0 && kVecTemp[1]==2)||(kVecTemp[0]== 2 && kVecTemp[1]==37) )
	{
		return ya_unknow;
	}
	pCellList[0]->show(true);
	pCellList[1]->show(true);
	m_pCurrentChip->setNumList(kVecTemp);
	showCellByNumList(kVecTemp);
	return ya_2;
}
int  LPGameScence::CheckYa_3(const std::vector<YaCell*>& pCellList )
{
	int nLenth = pCellList.size();
	if (nLenth == 2)
	{
		int nNum1 = pCellList[0]->getNumber();
		int nNum2 = pCellList[1]->getNumber();
		pCellList[0]->show(false);
		pCellList[1]->show(false);

		std::vector<int> kVecTemp;
		kVecTemp.push_back(nNum1);
		kVecTemp.push_back(nNum2);
		std::sort(kVecTemp.begin(),kVecTemp.end());

		if ( kVecTemp[1] != 50)
		{
			return ya_unknow;
		}
		std::vector<int> kNumsList;
		kNumsList.push_back(kVecTemp[0]);
		kNumsList.push_back(kVecTemp[0]-1);
		kNumsList.push_back(kVecTemp[0]-2);
		m_pCurrentChip->setNumList(kNumsList);
		showCellByNumList(kNumsList);
	}
	else if( nLenth == 3)
	{
		int nNum1 = pCellList[0]->getNumber();
		int nNum2 = pCellList[1]->getNumber();
		int nNum3 = pCellList[2]->getNumber();
		pCellList[0]->show(false);
		pCellList[1]->show(false);
		pCellList[2]->show(false);

		std::vector<int> kVecTemp;
		kVecTemp.push_back(nNum1);
		kVecTemp.push_back(nNum2);
		kVecTemp.push_back(nNum3);
		std::sort(kVecTemp.begin(),kVecTemp.end());

		if( kVecTemp[1] == 2 )
		{	
			m_pCurrentChip->setNumList(kVecTemp);
			showCellByNumList(kVecTemp);
		}
		else
		{
			return ya_unknow;
		}
	}
	return ya_3;
}
int  LPGameScence::CheckYa_6(const std::vector<YaCell*>& pCellList )
{
	int nLenth = pCellList.size();
	if (nLenth == 3)
	{
		pCellList[0]->show(false);
		pCellList[1]->show(false);
		pCellList[2]->show(false);

		int kNum1 = pCellList[0]->getNumber();
		int kNum2 = pCellList[1]->getNumber();
		int kNum3 = pCellList[2]->getNumber();

		std::vector<int> kVecTemp;
		kVecTemp.push_back(kNum1);
		kVecTemp.push_back(kNum2);
		kVecTemp.push_back(kNum3);
		std::sort(kVecTemp.begin(),kVecTemp.end());
		if (kVecTemp[2]==50 && kVecTemp[0]>0 && kVecTemp[1]<=36 )
		{
			std::vector<int> kNumsList;
			kNumsList.push_back(kVecTemp[0]);
			kNumsList.push_back(kVecTemp[0]-1);
			kNumsList.push_back(kVecTemp[0]-2);
			kNumsList.push_back(kVecTemp[1]);
			kNumsList.push_back(kVecTemp[1]-1);
			kNumsList.push_back(kVecTemp[1]-2);
			m_pCurrentChip->setNumList(kNumsList);
			showCellByNumList(kNumsList);
			return ya_6;
		}
		else if ( kVecTemp[2]==50 && kVecTemp[0]==0)
		{
			std::vector<int> kNumsList;
			kNumsList.push_back(kVecTemp[0]);
			kNumsList.push_back(37);
			kNumsList.push_back(kVecTemp[1]);
			kNumsList.push_back(kVecTemp[1]-1);
			kNumsList.push_back(kVecTemp[1]-2);
			m_pCurrentChip->setNumList(kNumsList);
			showCellByNumList(kNumsList);
			return ya_5;
		}
	}
	
	return ya_unknow;
}
int  LPGameScence::CheckYa_4(const std::vector<YaCell*>& pCellList )
{
	int nLenth = pCellList.size();
	if (nLenth != 4)
	{
		return ya_unknow;
	}
	pCellList[0]->show(false);
	pCellList[1]->show(false);
	pCellList[2]->show(false);
	pCellList[3]->show(false);

	int kNum1 = pCellList[0]->getNumber();
	int kNum2 = pCellList[1]->getNumber();
	int kNum3 = pCellList[2]->getNumber();
	int kNum4 = pCellList[3]->getNumber();

	std::vector<int> kVecTemp;
	kVecTemp.push_back(kNum1);
	kVecTemp.push_back(kNum2);
	kVecTemp.push_back(kNum3);
	kVecTemp.push_back(kNum4);
	std::sort(kVecTemp.begin(),kVecTemp.end());

	if (kVecTemp[3] >= 37)
	{
		return ya_unknow;
	}
	showCellByNumList(kVecTemp);
	m_pCurrentChip->setNumList(kVecTemp);
	return ya_4;
}
void LPGameScence::CheckEndPos(const Vec2& pos)
{
// 	Vec2 winPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","WinPos"));
// 	m_pCurrentChipTemp->getNode()->setPosition(winPos);

	MoveTo* pMove = MoveTo::create(0.1f,pos);
	ScaleTo* pScale = ScaleTo::create(0.1f,0.78f);
	CallFunc* pFun = CallFunc::create(CC_CALLBACK_0(LPGameScence::commpletYa,this,pos));
	Sequence* pAction =Sequence::create(Spawn::create(pMove,pScale,NULL),pFun,NULL);
	m_pCurrentChipTemp->getNode()->runAction(pAction);

	SoundFun::Instance().playEffect("BJLSound/effect_rw_chips.mp3");

}
void LPGameScence::commpletYa(cocos2d::Vec2 pos)
{
//	SoundFun::playEffect("putChip.ogg");
	m_pCurrentChipTemp->updateText();
	Command_PlaceBet(m_pCurrentChipTemp->getYaID(),m_pCurrentChipTemp->getYaNum(),pos.x,pos.y);
}

void LPGameScence::addChipInList( YaChip*& pChip )
{
	int nYaID = pChip->getYaID();
	int nAllNum = pChip->getYaNum();
	for (int i=0;i<(int)m_pYaChipList.size();i++)
	{
		if(!m_pYaChipList[i]->getNode()->isVisible())
		{
			continue;
		}
		int nId = m_pYaChipList[i]->getYaID();
		if (nId == nYaID)
		{
			nAllNum += m_pYaChipList[i]->getYaNum(); 
		}
	}
	pChip->setYaNumTemp(nAllNum);
	m_pYaChipList.push_back(pChip);

// 	int nTotal =  getBetNum();
// 	setBetNum(nTotal);
}

void LPGameScence::wheel_run(int nEndNum)
{
	SoundFun::Instance().playEffect("LPSound/effect_rw_ball.mp3");
	WidgetFun::setButtonEnabled(this,"LPButton_ResetAll",false);
	WidgetFun::setButtonEnabled(this,"LPButton_Reset",false);

	WidgetFun::runWidgetAction(this,"LPGameBg","MoveToR");

	Node* pWheel = WidgetFun::getChildWidget(this,"LPWheel");
	RotateBy* pRo = RotateBy::create(12.f,-180*10);
	EaseInOut* pAction = EaseInOut::create(pRo,1.5);
	pWheel->runAction(pAction);

	float fRangle = LPGameLogic::Instance().getRangle(nEndNum);

	Node* wheel_temp = WidgetFun::getChildWidget(this,"wheel_temp");
	wheel_temp->setRotation(0);
	wheel_temp->setScale(1.0f);

	RotateBy* pRo1 = RotateBy::create(12.f,180*10+fRangle);
	ScaleTo* pScale = ScaleTo::create(12.0f,0.68f);
	EaseOut* pOut = EaseOut::create(pRo1,1.5f);
	CallFunc* pFun = CallFunc::create(CC_CALLBACK_0(LPGameScence::runEnd,this));
	Spawn* pSpawn = Spawn::create(pOut,pScale,NULL);
	Sequence* pSeq = Sequence::create(pOut,pFun,NULL);

	wheel_temp->runAction(pSeq);

	WidgetFun::runWidgetAction(this,"ball_effect","Hide");
	WidgetFun::runWidgetAction(this,"img_ball","MoveDown");
}

void LPGameScence::runEnd()
{
	WidgetFun::runWidgetAction(this,"ball_effect","Blink");

	Node* wheelNode = WidgetFun::getChildWidget(this,"LPWheelNode");
	RotateBy* pRo = CCRotateBy::create(4.f,-50);
	EaseOut* pOut = CCEaseOut::create(pRo,1.1f);
	wheelNode->runAction(pOut);

	WidgetFun::setText(this,"LPPoint_Num",m_nEndNum);
	WidgetFun::setVisible(this,"LPPointNode",true);
	WidgetFun::setText(this,"LPWheelPointNum",m_nEndNum);
	WidgetFun::setVisible(this,"LPWheelPointNode",true);

	SoundFun::Instance().playEffect(utility::toString("LPSound/effect_rw_result_",m_nEndNum,".mp3"));
}
void LPGameScence::playTimeOver1()
{
	WidgetFun::runWidgetAction(this,"LPGameBg","MoveToL");
	WidgetFun::runWidgetAction(this,"ball_effect","Hide");

	blinkCell(m_nEndNum);
	saveEndNum(m_nEndNum);
}
void LPGameScence::playTimeOver2()
{
	endChipsAni(m_nEndNum);
	endChipAniOther(m_nEndNum);
	TimeManager::Instance().addCerterTimeCB1<LONGLONG>(CC_CALLBACK_1(LPGameScence::setWinScore,this),this,1.5f,m_lWinScore);
}
void LPGameScence::cleanYaChips(ChipList& kChipsList)
{
	for (int i=0;i<(int)kChipsList.size();i++)
	{
		kChipsList[i]->remove();
	}
	kChipsList.clear();
}

void LPGameScence::cleanYaChips(BetChipMap& kChipMap)
{
	BetChipMap::iterator iter  = kChipMap.begin();
	for (;iter!=kChipMap.end();iter++)
	{
		cleanYaChips(iter->second);
	}
	kChipMap.clear();
}

void LPGameScence::blinkCell(int nEndNum)
{
//	showCheck(true);
	//ÖÐ½±ÇøÓò¶¯»­
	ASSERT(nEndNum>=0 && nEndNum<38);
	if (nEndNum < 0)
	{
		return;
	}
	WidgetFun::runWidgetAction(this,utility::toString("LP_",nEndNum),"Start");

	std::vector<std::string> kNameList =  LPGameLogic::Instance().getOtherCellName(nEndNum);
	for (int i=0;i<(int)kNameList.size();i++)
	{
		WidgetFun::runWidgetAction(this,kNameList[i],"Start");
	}

}

void LPGameScence::endChipsAni(int nEndNum)
{
	Vec2 pos1 = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","WinPos"));
	Vec2 pos2 = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","LosePos"));

	ChipList::iterator iter = m_pYaChipList.begin();
	for (int i=0;i<(int)m_pYaChipList.size();i++)
	{
		if (m_pYaChipList[i]->isYaZhongByArea(nEndNum))
		{
			chip_move(m_pYaChipList[i]->getNode(),pos1,i+50);
		}
		else
		{
			chip_move(m_pYaChipList[i]->getNode(),pos2,i);
		}
	}
}

void LPGameScence::endChipAniOther(int nEndNum)
{
	Vec2 pos1 = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","OtherPos"));
	Vec2 pos2 = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","LosePos"));

	BetChipMap::iterator iter = m_kBetChipMap.begin();
	for (;iter!=m_kBetChipMap.end();iter++)
	{
		for (int i=0;i<(int)iter->second.size();i++)
		{
			if (iter->second[i]->isYaZhongByArea(nEndNum))
			{
				chip_move(iter->second[i]->getNode(),pos1,i+50);
			}
			else
			{
				chip_move(iter->second[i]->getNode(),pos2,i);
			}
		}
	}
	
}

void LPGameScence::chip_move(Node* pChip,const Vec2& pos,int nIndex)
{
	MoveTo* pMove = MoveTo::create(0.2f,pos);
	DelayTime* pDelay = DelayTime::create(0.02f*nIndex);
	pChip->removeAllChildren();
	pChip->runAction(Sequence::create(pDelay,pMove,Hide::create(),NULL));
}

void LPGameScence::saveEndNum(int nEndNum)
{
	m_kHistoryList.push_back(nEndNum);
	if (m_kHistoryList.size() > MAX_HISTORY)
	{
		m_kHistoryList.pop_front();
	}
}

void LPGameScence::updateHistotry()
{
	std::list<int>::iterator iter = m_kHistoryList.begin();
	for (int i=1;i<= MAX_HISTORY;i++)
	{
		WidgetFun::setVisible(this,utility::toString("LPHistory_Row",i,":RowTx1"),false);
		WidgetFun::setVisible(this,utility::toString("LPHistory_Row",i,":RowTx2"),false);
		if (i >(int) m_kHistoryList.size())
		{
			continue;
		}

		bool bBlack = LPGameLogic::Instance().bIsBlackNum(*iter);
		if (bBlack)
		{
			WidgetFun::setVisible(this,utility::toString("LPHistory_Row",i,":RowTx2"),true);
			WidgetFun::setText(this,utility::toString("LPHistory_Row",i,":RowTx2"),*iter);
		}
		else if(*iter==0)
		{
			WidgetFun::setVisible(this,utility::toString("LPHistory_Row",i,":RowTx1"),true);
			WidgetFun::setText(this,utility::toString("LPHistory_Row",i,":RowTx1"),*iter);
			WidgetFun::setTextColor(this,utility::toString("LPHistory_Row",i,":RowTx1"),Color3B::GREEN);
		}
		else if(*iter==37) //00
		{
			WidgetFun::setVisible(this,utility::toString("LPHistory_Row",i,":RowTx1"),true);
			WidgetFun::setText(this,utility::toString("LPHistory_Row",i,":RowTx1"),"00");
			WidgetFun::setTextColor(this,utility::toString("LPHistory_Row",i,":RowTx1"),Color3B::GREEN);
		}
		else
		{
			WidgetFun::setVisible(this,utility::toString("LPHistory_Row",i,":RowTx1"),true);
			WidgetFun::setText(this,utility::toString("LPHistory_Row",i,":RowTx1"),*iter);
			WidgetFun::setTextColor(this,utility::toString("LPHistory_Row",i,":RowTx1"),Color3B::RED);
		}
		iter++;

	}
}

void LPGameScence::recycleYaChips(ChipList& kChipsList,bool isSelf)
{
	for (int i=0;i<(int)kChipsList.size();i++)
	{
		recycleYaChip(kChipsList[i],isSelf,i);
	}
	kChipsList.clear();
}

void LPGameScence::recycleYaChip(YaChip* pChip,bool isSelf,int nIndex)
{
	Vec2 kEndPos;
	if (isSelf)
	{
		kEndPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","WinPos"));
	}
	else
	{
		kEndPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","OtherPos"));
	}
	pChip->recycle(kEndPos,nIndex);
	SoundFun::Instance().playEffect("BJLSound/effect_rw_chips.mp3");
}

bool LPGameScence::setWinScore(LONGLONG lWinScore)
{
	WidgetFun::setText(this,"LPGold_Num",m_lTotalScore + lWinScore);

	if (lWinScore >0)
	{
		WidgetFun::setText(this,"LPJieSuan_WinNum",utility::toString("+",lWinScore));
	}
	else if(lWinScore <0)
	{
		WidgetFun::setText(this,"LPJieSuan_WinNum",utility::toString("-",-lWinScore));
	}
	else
	{
		WidgetFun::setText(this,"LPJieSuan_WinNum",lWinScore);
	}
	WidgetFun::runWidgetAction(this,"LPTip_JieSuan","Start");

	return false;
}

bool LPGameScence::setUserScore(LONGLONG lScore)
{
	LONGLONG lScoreTemp = utility::parseInt(WidgetFun::getText(this,"LPGold_Num"));
	WidgetFun::setText(this,"LPGold_Num",lScoreTemp + lScore);
	return false;
}

