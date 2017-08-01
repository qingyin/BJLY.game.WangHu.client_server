#include "DBScenceManager.h"
#include "Game/Script/ActionEx.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Script/SoundFun.h"
#include "Game/Script/ScriptData.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/GameLib.h"
#include "Game/Game/QYCardTurnManager.h"

#include "ClientDB/DBGameManager.h"

#include "DBLogonScence.h"
#include "DBHomeScence.h"
#include "DBPlayerInfoScence.h"
#include "DBPublicNoticeScence.h"
#include "DBBankScence.h"
#include "DBShopScence.h"
#include "DBRankScence.h"

FV_SINGLETON_STORAGE(DBScenceManager);


DBScenceManager::DBScenceManager(cocos2d::Node* pNode)
	:m_pRootNode(NULL)
{
	m_pRootNode = cocos2d::Node::create();
	pNode->addChild(m_pRootNode);
	init();
}
DBScenceManager::DBScenceManager()
	:m_pRootNode(NULL)
{
	CCAssert(false,"");
	init();
}
DBScenceManager::~DBScenceManager(void)
{
}

void DBScenceManager::init()
{
	new DBGameManager;

	new DBLogonScence;
	m_pRootNode->addChild(DBLogonScence::pInstance());

	new DBHomeScence;
	m_pRootNode->addChild(DBHomeScence::pInstance());

	new DBPlayerInfoScence;
	m_pRootNode->addChild(DBPlayerInfoScence::pInstance());

	new DBBankScence;
	m_pRootNode->addChild(DBBankScence::pInstance());

	new DBShopScence;
	m_pRootNode->addChild(DBShopScence::pInstance());
	
	new DBPublicNoticeScence;
	m_pRootNode->addChild(DBPublicNoticeScence::pInstance());
	
	new NoticeMsg();
	m_pRootNode->addChild(NoticeMsg::pInstance());

	new DBRankScence();
	m_pRootNode->addChild(DBRankScence::pInstance());
	
	InLogonScence();
}
void DBScenceManager::HideAll(cocos2d::Node* pActShowNode)
{
	DBBankScence::Instance().HideAll();
	DBRankScence::Instance().HideAll();
	DBLogonScence::Instance().setVisible(false);
	DBHomeScence::Instance().setVisible(false);
	DBPlayerInfoScence::Instance().setVisible(false);
	DBPublicNoticeScence::Instance().setVisible(false);
	DBGameManager::Instance().hideAll();
	if (QYCardTurnManager::pInstance())
	{
		QYCardTurnManager::Instance().HideCard();
	}
}
cocos2d::Node* DBScenceManager::getRootNode()
{
	return m_pRootNode;
}
void DBScenceManager::addGameNode(cocos2d::Node* pNode)
{
	m_pRootNode->addChild(pNode);
}
void DBScenceManager::InLogonScence()
{
	HideAll(DBLogonScence::pInstance());
	DBLogonScence::Instance().setVisible(true);
	DBLogonScence::Instance().EnterScence();
}
void DBScenceManager::InHomeScence()
{
	HideAll(DBLogonScence::pInstance());
	DBHomeScence::Instance().setVisible(true);
	DBHomeScence::Instance().EnterScence();

	IServerItem::get()->IntermitConnect(true);
}
void DBScenceManager::update(float fTime)
{

}
void DBScenceManager::GameBackScence()
{
	InHomeScence();
}