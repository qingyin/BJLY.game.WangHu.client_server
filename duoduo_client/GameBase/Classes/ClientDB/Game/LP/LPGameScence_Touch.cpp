#include "LPGameScence.h"
#include "Game/GameLib.h"
#include "CMD_Land.h"
#include "LPYaCell.h"

using namespace cocos2d;

#define  LPTouchSize Size(20,20)
class LPTouchLayer:public Layer
{
public:
	
	static LPTouchLayer* createLPTouchLayer(); 

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
};

LPTouchLayer* LPTouchLayer::createLPTouchLayer()
{
	LPTouchLayer* pLayer = new LPTouchLayer();
	if (pLayer && pLayer->init())
	{
		pLayer->autorelease();
		return pLayer;
	}
	else
	{
		CC_SAFE_DELETE(pLayer);
		return nullptr;
	}
}

bool LPTouchLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setSwallowsTouches(false);
	return true;
}

bool LPTouchLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (!utility::isAllVisible(this))
	{
		return false;
	}
	if (touch->getID() != 0)
	{
		return false;
	}
	return LPGameScence::Instance().onTouchBegan(touch,unused_event);
}

void LPTouchLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (!utility::isAllVisible(this))
	{
		return;
	}
	if (touch->getID() != 0)
	{
		return ;
	}
	LPGameScence::Instance().onTouchMoved(touch,unused_event);
}

void LPTouchLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (!utility::isAllVisible(this))
	{
		return;
	}
	if (touch->getID() != 0)
	{
		return ;
	}
	LPGameScence::Instance().onTouchEnded(touch,unused_event);
}

void LPGameScence::initTouch()
{
	LPTouchLayer* pLayer = LPTouchLayer::createLPTouchLayer();
	addChild( pLayer  );
}
bool LPGameScence::onTouchBegan(Touch *touch, Event *unused_event)
{
	Node* LPTableNode = WidgetFun::getChildWidget(this,"LPTableNode");
	Vec2 kPos = touch->getLocation();
	kPos = LPTableNode->convertToNodeSpace(kPos);

	Node* LPTableBg =  WidgetFun::getChildWidget(LPTableNode,"LPTableBg");

	if (m_nGameState !=GAME_SCENE_BET || !LPTableBg->getBoundingBox().containsPoint(kPos)|| WidgetFun::isWidgetVisble(this,"LPHistoryBg"))
	{
		return false;
	}
	m_pCurrentChip = YaChip::create(m_nChipsType);
	m_pCurrentChip->getNode()->setPosition(kPos);
	m_pCurrentChip->getNode()->setScale(0.8f);
	LPTableNode->addChild(m_pCurrentChip->getNode());

	checkCellShadow(kPos,LPTouchSize);
	m_bDrag = true;
	return true;
}
void LPGameScence::onTouchMoved(Touch *touch, Event *unused_event)
{
	if ( !m_bDrag || m_nGameState !=GAME_SCENE_BET )
	{
		return;
	}

	Node* LPTableNode = WidgetFun::getChildWidget(this,"LPTableNode");
	Vec2 kPos = touch->getLocation();
	kPos = LPTableNode->convertToNodeSpace(kPos);

	m_pCurrentChip->getNode()->setPosition(kPos);
	checkCellShadow(kPos,LPTouchSize);
}
void LPGameScence::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_bDrag = false;
	Node* LPTableNode = WidgetFun::getChildWidget(this,"LPTableNode");
	Vec2 kPos = touch->getLocation();
	kPos = LPTableNode->convertToNodeSpace(kPos);
	m_pCurrentChipTemp = m_pCurrentChip;

	checkCellShadow(kPos,LPTouchSize);
	if (m_pCurrentYaType != ya_unknow && m_nGameState ==GAME_SCENE_BET/*&& nNum<=m_nAllCoins*/ )
	{
		Vec2 kEndPos = YaCell::cheakEndPos(m_pTouchCellList,m_pCurrentYaType);
		m_pCurrentChipTemp->setYaType(m_pCurrentYaType);
		addChipInList(m_pCurrentChipTemp);

		CheckEndPos(kEndPos);
	}
	else
	{
		ScaleTo* pScale = ScaleTo::create(0.15f,0.2f);
		FadeOut* pFade = FadeOut::create(0.15f);
		Spawn* pSpaw = Spawn::create(pScale,pFade,NULL);
		Sequence* pAction = Sequence::create(pSpaw,RemoveSelf::create(),NULL);
		m_pCurrentChipTemp->getNode()->runAction(pAction);

	}
	showCheck(false);
}