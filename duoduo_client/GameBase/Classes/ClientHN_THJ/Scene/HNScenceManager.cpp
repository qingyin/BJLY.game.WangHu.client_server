#include "HNScenceManager.h"
#include "Game/GameLib.h"


#include "HNHomeScence.h"
#include "HNLogonScence.h"
#include "HNPrivateScenceHN.h"
#include "HNGameRecord.h"
#include "HNSetScence.h"
#include "HNNewsScence.h"
#include "HNHelpScence.h"
#include "HNRouletteScence.h"
#include "HNShopScence.h"
#include "HNExchangeScence.h"
#include "../HNGameManager.h"

FV_SINGLETON_STORAGE(HNScenceManager);


HNScenceManager::HNScenceManager(cocos2d::Node* pNode)
	:m_pRootNode(NULL)
	,m_bInBackground(false)
	,m_iScenceTimeID(0)
{
	m_pRootNode = cocos2d::Node::create();
	pNode->addChild(m_pRootNode);
	init();
}
HNScenceManager::HNScenceManager()
	:m_pRootNode(NULL)
	,m_bInBackground(false)
{
	CCAssert(false,"");
	init();
}
HNScenceManager::~HNScenceManager(void)
{
}

void HNScenceManager::init()
{
	new HNGameManager;

	new HNHomeScence;
	m_pRootNode->addChild(HNHomeScence::pInstance(),-2);

	new HNLogonScence;
	m_pRootNode->addChild(HNLogonScence::pInstance());

	new HNExchangeScence;
	m_pRootNode->addChild(HNExchangeScence::pInstance());
	
	new HNRouletteScence;
	m_pRootNode->addChild(HNRouletteScence::pInstance());

	new HNPrivateScenceHN;
	m_pRootNode->addChild(HNPrivateScenceHN::pInstance());

	new HNGameRecord;
	m_pRootNode->addChild(HNGameRecord::pInstance());

	new HNNewsScence;
	m_pRootNode->addChild(HNNewsScence::pInstance());

	new HNHelpScence;
	m_pRootNode->addChild(HNHelpScence::pInstance());
	
	new HNSetScence;
	m_pRootNode->addChild(HNSetScence::pInstance());

	new HNShopScence;
	m_pRootNode->addChild(HNShopScence::pInstance());
	
	new PopScence;
	m_pRootNode->addChild(PopScence::pInstance());

	new EffectScence();
	m_pRootNode->addChild(EffectScence::pInstance());
	
	new NoticeMsg();
	m_pRootNode->addChild(NoticeMsg::pInstance());

	InLogonScence();
}
void HNScenceManager::HideAll(cocos2d::Node* pActShowNode)
{
	HNLogonScence::Instance().setVisible(false);
	HNHomeScence::Instance().setVisible(false);
	HNExchangeScence::Instance().setVisible(false);
	HNRouletteScence::Instance().hideAll();
	HNPrivateScenceHN::Instance().hideAll();
	HNGameRecord::Instance().hideAll();
	HNGameManager::Instance().HideAll();
	HNSetScence::Instance().hideAll();
	HNNewsScence::Instance().hideAll();
	HNHelpScence::Instance().hideAll();
	HNShopScence::Instance().hideAll();
}
cocos2d::Node* HNScenceManager::getRootNode()
{
	return m_pRootNode;
}
void HNScenceManager::addGameNode(cocos2d::Node* pNode)
{
	m_pRootNode->addChild(pNode,-1);
}
void HNScenceManager::InLogonScence()
{
	HideAll(HNLogonScence::pInstance());
	HNLogonScence::Instance().setVisible(true);
	HNLogonScence::Instance().EnterScence();
}
void HNScenceManager::InHomeScence()
{
	HideAll(HNLogonScence::pInstance());
	HNHomeScence::Instance().setVisible(true);
	HNHomeScence::Instance().EnterScence();
}
void HNScenceManager::InPlayerInfoScence()
{
}
void HNScenceManager::Button_CB_ExitGame(cocos2d::Ref*,WidgetUserInfo*)
{
	cocos2d::Director::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
	exit(0);
#endif
}
void HNScenceManager::BackKey()
{
	if (HNLogonScence::Instance().isVisible())
	{
		PopScence::Instance().showAccessPlane(utility::getScriptString("HomeExitKeyBack"),this,
			button_selector(HNScenceManager::Button_CB_ExitGame),NULL);
		return;
	}
	if (GameManagerBase::InstanceBase().BackKey())
	{
		return ;
	}
	if (HNGameRecord::Instance().isVisible())
	{
		InHomeScence();
		return;
	}
	if (HNPrivateScenceHN::Instance().BackKey())
	{
		return;
	}
	PopScence::Instance().showAccessPlane(utility::getScriptString("HomeExitKeyBack"),this,
		button_selector(HNScenceManager::Button_CB_ExitGame),NULL);
}
void HNScenceManager::update(float fTime)
{
}
void HNScenceManager::GameBackScence()
{
	InHomeScence();
}