#include "SCScenceManager.h"
#include "Game/GameLib.h"

#include "../SCGameManager.h"

#include "SCLogonScence.h"
#include "SCHomeScence.h"
#include "SCSignInScence.h"
#include "SCLowProtectScence.h"
#include "SCPlayerInfoScence.h"
#include "SCBeginnerScence.h"
#include "SCAddRankScence.h"
#include "SCMatchScence.h"
#include "SCShopScence.h"
#include "SCExchangeScence.h"

FV_SINGLETON_STORAGE(SCScenceManager);


SCScenceManager::SCScenceManager(cocos2d::Node* pNode)
	:m_pRootNode(NULL)
	,m_bInBackground(false)
	,m_iScenceTimeID(0)
{
	m_pRootNode = cocos2d::Node::create();
	pNode->addChild(m_pRootNode);
	init();
}
SCScenceManager::SCScenceManager()
	:m_pRootNode(NULL)
	,m_bInBackground(false)
{
	CCAssert(false,"");
	init();
}
SCScenceManager::~SCScenceManager(void)
{
}

void SCScenceManager::init()
{
	new SCGameManager;

	new SCHomeScence;
	m_pRootNode->addChild(SCHomeScence::pInstance());

	new SCMatchScence;
	m_pRootNode->addChild(SCMatchScence::pInstance());
	
	new SCPlayerInfoScence;
	m_pRootNode->addChild(SCPlayerInfoScence::pInstance());

	new SCShopScence;
	m_pRootNode->addChild(SCShopScence::pInstance());

	new SCExchangeScence;
	m_pRootNode->addChild(SCExchangeScence::pInstance());

	new SCLogonScence;
	m_pRootNode->addChild(SCLogonScence::pInstance());

	new SCBeginnerScence;
	m_pRootNode->addChild(SCBeginnerScence::pInstance());

	new SCAddRankScence;
	m_pRootNode->addChild(SCAddRankScence::pInstance());
	
	new SCLowProtectScence;
	m_pRootNode->addChild(SCLowProtectScence::pInstance());
	
	new PopScence;
	m_pRootNode->addChild(PopScence::pInstance());

	new EffectScence();
	m_pRootNode->addChild(EffectScence::pInstance());
	
	new NoticeMsg();
	m_pRootNode->addChild(NoticeMsg::pInstance());

	InLogonScence();
}
void SCScenceManager::HideAll(cocos2d::Node* pActShowNode)
{
	SCLogonScence::Instance().HideAll();
	SCHomeScence::Instance().setVisible(false);
	SCBeginnerScence::Instance().HideAll();
	SCAddRankScence::Instance().HideAll();
	SCPlayerInfoScence::Instance().setVisible(false);
	SCShopScence::Instance().setVisible(false);
	SCExchangeScence::Instance().setVisible(false);
	SCMatchScence::Instance().setVisible(false);
	SCGameManager::Instance().HideAll();
}
cocos2d::Node* SCScenceManager::getRootNode()
{
	return m_pRootNode;
}
void SCScenceManager::addGameNode(cocos2d::Node* pNode)
{
	m_pRootNode->addChild(pNode,-1);
}
void SCScenceManager::InLogonScence()
{
	HideAll(SCLogonScence::pInstance());
	SCHomeScence::Instance().setVisible(true);
	SCLogonScence::Instance().EnterScence();
}
void SCScenceManager::InHomeScence()
{
	HideAll(SCLogonScence::pInstance());
	SCHomeScence::Instance().setVisible(true);
	SCHomeScence::Instance().EnterScence();
}
void SCScenceManager::InPlayerInfoScence()
{
	HideAll(SCPlayerInfoScence::pInstance());
	SCPlayerInfoScence::Instance().setVisible(true);
	SCPlayerInfoScence::Instance().EnterScence();
}
void SCScenceManager::update(float fTime)
{
}
void SCScenceManager::GameBackScence()
{
	InHomeScence();
}