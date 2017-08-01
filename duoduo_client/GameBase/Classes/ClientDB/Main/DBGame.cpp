#include "DBGame.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"
#include "Game/Script/WindowInfo.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Kernel/socket/Socket.h"
#include "Game/Script/ScriptXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "ClientDB/Scene/DBScenceManager.h"

#include "Game/Game/QYCardTurnManager.h"

USING_NS_CC;

CCScene* DBGame::scene()
{
	int iValue = 0;
	for (int i = 0;i<24;i++)
	{
		FvMask::Add(iValue,_MASK_(i));
	}
	CCScene *scene = CCScene::create();
	DBGame *layer = DBGame::create();
	scene->addChild(layer);
	return scene;
}
void DBGame::setString(std::string kTxt)
{
}
bool DBGame::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	WidgetSkinXMLparse kSkinXml1("Script/DBWidgetSkin.xml");

	cocos2d::ScriptXMLparse kScriptXml1("Script/DBScriptValue.xml");
	cocos2d::ScriptXMLparse kScriptXml2("Script/DBScriptValueStr.xml");

	new WindowInfo;
	this->scheduleUpdate();
	ScriptData<std::string> kUITest("UITest");

	if (kUITest.Value() != "")
	{
		CCMenuItemImage *pImagicItem1 = CCMenuItemImage::create(
			"progress_y.png",
			"progress_n.png",
			this,
			menu_selector(DBGame::reLoad));

		pImagicItem1->setPosition(ccp(80 ,150));

		CCMenuItemImage *pImagicItem2 = CCMenuItemImage::create(
			"progress_y.png",
			"progress_n.png",
			this,
			menu_selector(DBGame::reMask));

		pImagicItem1->setPosition(ccp(80 ,150));
		pImagicItem2->setPosition(ccp(80 ,70));

		CCMenu* pMenu1 = CCMenu::create(pImagicItem1,pImagicItem2,NULL);
		pMenu1->setPosition(CCPoint::ZERO);
		this->addChild(pMenu1, 2);


		cocos2d::CCNode* pNode = cocos2d::CCNode::create();
		addChild(pNode);
		pNode->setTag(10000);

		if (!WidgetManager::pInstance())
		{
			new WidgetManager;
		}
		WidgetScenceXMLparse kXml11(kUITest.Value(),pNode);
		WindowInfo::Instance().changWinSize(pNode);
		cocos2d::Sprite* pSprite = cocos2d::Sprite::create("Point.png");
		pSprite->setPosition(640,360);
		ScriptData<int> kAple("UITestMaskAple");
		pSprite->setOpacity(kAple.Value());
		pNode->addChild(pSprite, 2);
		pSprite->setTag(11000);

		return true;
	}

	//cocos2d::Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);

	new DBScenceManager(this);

	cocos2d::Node* pTempNode = Node::create();
	addChild(pTempNode);
	new QYCardTurnManager;
	pTempNode->addChild(QYCardTurnManager::pInstance());
	QYCardTurnManager::Instance().initCard();

	Sprite* pSprite1 = Sprite::create("GameBJL/cards/cardback.png");
	Sprite* pSprite2 = Sprite::create("GameBJL/cards/50.png");
	Sprite* pSprite3 = Sprite::create("GameBJL/cards/cardback.png");
	Sprite* pSprite4 = Sprite::create("GameBJL/cards/50.png");

	QYCardTurnManager::Instance().showCard(true,pSprite1,pSprite2,pSprite3,pSprite4);

	QYCardTurnManager::Instance().HideCard();
	WindowInfo::Instance().changWinSize(pTempNode);

	WindowInfo::Instance().changWinSize(DBScenceManager::Instance().getRootNode());

	return true;
}

void DBGame::reLoad(cocos2d::Ref* pSender)
{
	ScriptDataManager::getInterface()->clear();
	cocos2d::ScriptXMLparse kScriptXml1("Script/DBScriptValue.xml");
	cocos2d::ScriptXMLparse kScriptXml2("Script/DBScriptValueStr.xml");


	ScriptData<std::string> kUITest("UITest");
	WidgetManager::Instance().clearWidgetSkin();
	WidgetSkinXMLparse kSkinXml1("Script/DBWidgetSkin.xml");
	cocos2d::CCNode* pNode = getChildByTag(10000);
	pNode->removeAllChildren();
	WidgetScenceXMLparse kXml11(kUITest.Value(),pNode);

	cocos2d::Sprite* pSprite = cocos2d::Sprite::create("Point.png");
	pSprite->setPosition(640,360);
	ScriptData<int> kAple("UITestMaskAple");
	pSprite->setOpacity(kAple.Value());
	pNode->addChild(pSprite, 2);
	pSprite->setTag(11000);


	std::string kValue = WidgetFun::getWidgetUserInfo(this,"Aple");
	if (kValue == "")
	{
		pSprite->setOpacity(0);
	}
	else
	{
		ScriptData<int> kAple("UITestMaskAple");
		pSprite->setOpacity(kAple.Value());
	}

}
void DBGame::reMask(cocos2d::Ref* pSender)
{
	std::string kValue = WidgetFun::getWidgetUserInfo(this,"Aple");
	cocos2d::CCNode* pNode = getChildByTag(10000);
	cocos2d::Sprite* pSprite = (cocos2d::Sprite*)pNode->getChildByTag(11000);
	if (kValue == "")
	{
		ScriptData<int> kAple("UITestMaskAple");
		pSprite->setOpacity(kAple.Value());
		WidgetFun::setWidgetUserInfo(this,"Aple","1");
	}
	else
	{
		WidgetFun::setWidgetUserInfo(this,"Aple","");
		pSprite->setOpacity(0);
	}
}
void DBGame::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void DBGame::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void DBGame::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void DBGame::onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void DBGame::update(float delta)
{
}
void DBGame::onExit()
{
	cocos2d::Layer::onExit();
}
void DBGame::ExitGame()
{
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
