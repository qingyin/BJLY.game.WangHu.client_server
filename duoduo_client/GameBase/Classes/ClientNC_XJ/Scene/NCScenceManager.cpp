#include "NCScenceManager.h"
#include "Game/GameLib.h"


#include "NCHomeScence.h"
#include "NCLogonScence.h"
#include "NCPrivateScenceNC.h"
#include "NCGameRecord.h"
#include "NCSetScence.h"
#include "NCNewsScence.h"
#include "NCHelpScence.h"
#include "NCRouletteScence.h"
#include "NCShopScence.h"
#include "../NCGameManager.h"

FV_SINGLETON_STORAGE(NCScenceManager);


NCScenceManager::NCScenceManager(cocos2d::Node* pNode)
	:m_pRootNode(NULL)
	,m_bInBackground(false)
	,m_iScenceTimeID(0)
{
	m_pRootNode = cocos2d::Node::create();
	pNode->addChild(m_pRootNode);
	init();
}
NCScenceManager::NCScenceManager()
	:m_pRootNode(NULL)
	,m_bInBackground(false)
{
	CCAssert(false,"");
	init();
}
NCScenceManager::~NCScenceManager(void)
{
}

void NCScenceManager::init()
{
	new NCGameManager;

	new NCHomeScence;
	m_pRootNode->addChild(NCHomeScence::pInstance(),-2);

	new NCLogonScence;
	m_pRootNode->addChild(NCLogonScence::pInstance());

	new NCRouletteScence;
	m_pRootNode->addChild(NCRouletteScence::pInstance());

	new NCPrivateScenceNC;
	m_pRootNode->addChild(NCPrivateScenceNC::pInstance());

	new NCGameRecord;
	m_pRootNode->addChild(NCGameRecord::pInstance());

	new NCNewsScence;
	m_pRootNode->addChild(NCNewsScence::pInstance());

	new NCHelpScence;
	m_pRootNode->addChild(NCHelpScence::pInstance());
	
	new NCSetScence;
	m_pRootNode->addChild(NCSetScence::pInstance());

	new NCShopScence;
	m_pRootNode->addChild(NCShopScence::pInstance());
	
	new PopScence;
	m_pRootNode->addChild(PopScence::pInstance());

	new EffectScence();
	m_pRootNode->addChild(EffectScence::pInstance());
	
	new NoticeMsg();
	m_pRootNode->addChild(NoticeMsg::pInstance());

	InLogonScence();
}
void NCScenceManager::HideAll(cocos2d::Node* pActShowNode)
{
	NCLogonScence::Instance().setVisible(false);
	NCHomeScence::Instance().setVisible(false);
	NCRouletteScence::Instance().hideAll();
	NCPrivateScenceNC::Instance().hideAll();
	NCGameRecord::Instance().hideAll();
	NCGameManager::Instance().HideAll();
	NCSetScence::Instance().hideAll();
	NCNewsScence::Instance().hideAll();
	NCHelpScence::Instance().hideAll();
	NCShopScence::Instance().hideAll();
}
cocos2d::Node* NCScenceManager::getRootNode()
{
	return m_pRootNode;
}
void NCScenceManager::addGameNode(cocos2d::Node* pNode)
{
	m_pRootNode->addChild(pNode,-1);
}
void NCScenceManager::InLogonScence()
{
	HideAll(NCLogonScence::pInstance());
	NCLogonScence::Instance().setVisible(true);
	NCLogonScence::Instance().EnterScence();
}
void NCScenceManager::InHomeScence()
{
	HideAll(NCLogonScence::pInstance());
	NCHomeScence::Instance().setVisible(true);
	NCHomeScence::Instance().EnterScence();
}
void NCScenceManager::InPlayerInfoScence()
{
}
void NCScenceManager::update(float fTime)
{
}
void NCScenceManager::GameBackScence()
{
	InHomeScence();
}