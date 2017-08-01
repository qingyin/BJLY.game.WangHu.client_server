#include "NCGame.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"
#include "Game/Script/WindowInfo.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Kernel/socket/Socket.h"
#include "../Scene/NCScenceManager.h"
#include "Game/Script/ScriptXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
USING_NS_CC;

CCScene* NCGame::scene()
{
    CCScene *scene = CCScene::create();
    NCGame *layer = NCGame::create();
    scene->addChild(layer);
    return scene;
}
void NCGame::setString(std::string kTxt)
{
}
bool NCGame::init()
{
    if ( !CCLayer::init() )
    {
        return false;
	}
	WidgetSkinXMLparse kSkinXml1("Script/HNWidgetSkin.xml");

	cocos2d::ScriptXMLparse kScriptXml1("Script/HNScriptValue.xml");
	cocos2d::ScriptXMLparse kScriptXml2("Script/HNScriptValueStr.xml");

	this->scheduleUpdate();
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);

	new WindowInfo;
	
	ScriptData<std::string> kUITest("UITest");

	if (kUITest.Value() != "")
	{
		CCMenuItemImage *pImagicItem1 = CCMenuItemImage::create(
			"progress_y.png",
			"progress_n.png",
			this,
			menu_selector(NCGame::reLoad));

		pImagicItem1->setPosition(ccp(80 ,150));

		CCMenuItemImage *pImagicItem2 = CCMenuItemImage::create(
			"progress_y.png",
			"progress_n.png",
			this,
			menu_selector(NCGame::reMask));

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

	new NCScenceManager(this);

	WindowInfo::Instance().changWinSize(NCScenceManager::Instance().getRootNode());

    return true;
}

void NCGame::reLoad(cocos2d::Ref* pSender)
{
	ScriptDataManager::getInterface()->clear();
	cocos2d::ScriptXMLparse kScriptXml1("Script/HNScriptValue.xml");
	cocos2d::ScriptXMLparse kScriptXml2("Script/HNScriptValueStr.xml");

	ScriptData<std::string> kUITest("UITest");
	WidgetManager::Instance().clearWidgetSkin();
	WidgetSkinXMLparse kSkinXml1("Script/HNWidgetSkin.xml");
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
void NCGame::reMask(cocos2d::Ref* pSender)
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
void NCGame::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
	
}
void NCGame::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void NCGame::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void NCGame::onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event)
{
	if (touches.size() == 0)
	{
		return;
	}
}
void NCGame::update(float delta)
{
}
void NCGame::onExit()
{
	cocos2d::Layer::onExit();
}
void NCGame::ExitGame()
{
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
