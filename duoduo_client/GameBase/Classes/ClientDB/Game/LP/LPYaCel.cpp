#include "LPYaCell.h"
#include "Game/GameLib.h"
#include "LPGameLogic.h"
#include "LPGameScence.h"

YaCell::YaCell( const std::string& kName,Node* pShadow )
	:m_kName(kName)
	,m_pCellShadow((Sprite*)pShadow)
{

}

YaCell::~YaCell()
{

}

bool YaCell::checkByRect( const Rect& kRect )
{
	Rect kTemp = m_pCellShadow->getBoundingBox();
	bool bValue = kTemp.intersectsRect(kRect);
	m_pCellShadow->setVisible(bValue);
	return bValue;
}

bool YaCell::checkByPos( const Vec2& kPos )
{
	Rect kTemp = m_pCellShadow->getBoundingBox();
	bool bValue = kTemp.containsPoint(kPos);
	m_pCellShadow->setVisible(bValue);
	return bValue;
}
void YaCell::show( bool bShow )
{
	m_pCellShadow->setVisible(bShow);
}

std::string& YaCell::getName()
{
	return m_kName;
}

bool YaCell::isShow()
{
	return m_pCellShadow->isVisible();
}

const Vec2& YaCell::getPosition()
{
	return m_pCellShadow->getPosition();
}

float YaCell::getTop()
{
	Vec2 kR= m_pCellShadow->getPosition();
	return kR.y + m_pCellShadow->getBoundingBox().size.height/2;
}

int YaCell::getNumber()
{
	return utility::parseInt(WidgetFun::getWidgetUserInfo(m_pCellShadow,"Number"));
}
Vec2 YaCell::cheakEndPos(std::vector<YaCell*>& kCellList,int nBetType)
{
	Vec2 kEndPos;
	std::vector<YaCell*>::iterator iter = kCellList.begin();
	while (iter != kCellList.end())
	{
		if ((*iter)->isShow())
		{
			iter++;
		}
		else
		{
			iter = kCellList.erase(iter);
		}
	}
	int nLenth = kCellList.size();
	if (nLenth == 1)
	{
		kEndPos = kCellList[0]->getPosition(	);
		if (nBetType == ya_3)
		{
			kEndPos.y = kCellList[0]->getTop();
		}
	}
	else if (nLenth == 2)
	{
		kEndPos = (kCellList[0]->getPosition()+kCellList[1]->getPosition())/2;
		if (nBetType == ya_6 || nBetType == ya_5 )
		{
			kEndPos.y = kCellList[0]->getTop();
		}
	}
	else if (nLenth == 3)
	{
		int kNum1 = kCellList[0]->getNumber();
		int kNum2 = kCellList[1]->getNumber();
		int kNum3 = kCellList[2]->getNumber();

		Vec2 kPos1 = kCellList[0]->getPosition();
		Vec2 kPos2 = kCellList[1]->getPosition();
		Vec2 kPos3 = kCellList[2]->getPosition();
		if (kNum1 == 1 )
		{
			kEndPos.x = (kPos1.x + kPos3.x)/2;
			kEndPos.y = (kPos1.y+kPos2.y)/2;

		}
		else if(kNum3 == 3 )
		{
			kEndPos.x = (kPos2.x + kPos1.x)/2;
			kEndPos.y = (kPos2.y+kPos3.y)/2;

		}
		else
		{
			kEndPos.x = (kPos1.x + kPos2.x)/2;
			kEndPos.y = (kPos1.y+kPos3.y)/2;
		}
	}
	else if (nLenth == 4)
	{
		kEndPos = (kCellList[0]->getPosition()+kCellList[1]->getPosition()+kCellList[2]->getPosition()+kCellList[3]->getPosition())/4;	
	}
	return kEndPos;
}

YaChip::YaChip()
	:m_pChip(NULL)
	,m_pTextBg(NULL)
	,m_pText(NULL)
	,m_nChipType(gameChips1)
	,m_nYaType(ya_unknow)
	,m_nNum(1)
	,m_nNumTemp(0)
	,m_nYaID(-1)
{
	defaultInit();
}


YaChip::~YaChip()
{

}
void YaChip::defaultInit()
{
	m_kNumList.clear();
	m_nYaID = -1;
	m_pChip = NULL;
	m_pText = NULL;
	m_pTextBg = NULL;
}
void YaChip::init( int nType,bool bTxt )
{
	m_nYaType = nType;
	m_nNum = nType;
	m_nNumTemp = m_nNum;

	std::string kChip = utility::toString("GameLP/gameChips",nType,".png");
	m_pChip = Sprite::create(kChip.c_str());
	if (bTxt)
	{
		m_pText = Label::createWithSystemFont(utility::toString(m_nNum).c_str(),"",24);
		m_pText->setColor(Color3B(0,0,0));

		Size sz = m_pChip->getContentSize();
		m_pTextBg = Sprite::create("GameLP/betTxtBg.png");

		m_pTextBg->setPosition(sz.width/2,-20);
		m_pChip->addChild(m_pTextBg);
		m_pTextBg->setVisible(false);

		Size sz1 =  m_pTextBg->getContentSize();
		m_pText->setPosition(Vec2(sz1.width/2,sz1.height/2));
		m_pTextBg->addChild(m_pText);
	}
	
}
YaChip* YaChip::create( int nType,bool bTxt)
{
	YaChip* pChip = new YaChip;
	if (pChip)
	{
		pChip->init(nType,bTxt);
		return pChip;
	}
	else
	{
		CC_SAFE_DELETE(pChip);
		pChip = NULL;
		return NULL;
	}
}

Node* YaChip::getNode()
{
	return m_pChip;
}

void YaChip::setYaType( int nYaType )
{
	m_nYaType = nYaType;
}

void YaChip::setYaID(int nArea)
{
	m_nYaID = nArea;
}
int YaChip::getYaID()
{
	if (m_nYaID == -1)
	{
		m_nYaID = LPGameLogic::Instance().getBetID(m_kNumList);
	}
	return m_nYaID;
}

int YaChip::getYaNum()
{
	return m_nNum;
}

void YaChip::setYaNumTemp(int nNum)
{
	m_nNumTemp = nNum;
}

void YaChip::updateText()
{
	if ( m_pText&& m_pTextBg)
	{
		m_pTextBg->setVisible(true);
		m_pText->setString(utility::toString(m_nNumTemp).c_str());
	}
}

void YaChip::clearNumList()
{
	m_kNumList.clear();
}
void YaChip::setNumList(std::vector<int> kList)
{
	m_kNumList = kList;
}
void YaChip::insertNum(int nNum)
{
	m_kNumList.push_back(nNum);
}
bool YaChip::isYaZhong(int nEndNum)
{
	return std::find(m_kNumList.begin(),m_kNumList.end(),nEndNum) != m_kNumList.end();
}

bool YaChip::isYaZhongByArea(int nEndNum)
{
	m_kNumList = LPGameLogic::Instance().getNumListByBetID(m_nYaID);
	return isYaZhong(nEndNum);
}

void YaChip::moveToTable(const cocos2d::Vec2& kFrom,const cocos2d::Vec2& kTo,int nIndex)
{
	m_pChip->setPosition(kFrom);
	MoveTo* pMove = MoveTo::create(0.1f,kTo);
	CallFunc* pFun = CallFunc::create(CC_CALLBACK_0(YaChip::updateText,this));
	m_pChip->runAction(Sequence::create(DelayTime::create(nIndex*0.01f),pMove,pFun,NULL));

}
void YaChip::remove()
{
	if (m_pChip)
	{
		m_pChip->removeFromParentAndCleanup(true);
	}
	defaultInit();
}
void YaChip::recycle(const cocos2d::Vec2& kTo,int nIndex)
{
	m_pChip->removeAllChildren();
	m_pTextBg = NULL;
	m_pText = NULL;

	MoveTo* pMove = MoveTo::create(0.05f,kTo);
	CallFunc* pFun = CallFunc::create(CC_CALLBACK_0(YaChip::remove,this));
	m_pChip->runAction(Sequence::create(DelayTime::create(nIndex*0.01f),pMove,pFun,NULL));
}