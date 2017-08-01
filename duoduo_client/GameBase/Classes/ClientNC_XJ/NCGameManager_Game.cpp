#include "NCGameManager.h"

#include "Scene/NCScenceManager.h"
#include "Game/NCMJ/NCMJGameScence.h"

void NCGameManager::initGame()
{
	new NCMJGameScence();
	NCScenceManager::Instance().addGameNode(NCMJGameScence::pInstance());
}
void NCGameManager::StartXZDD()
{
}
void NCGameManager::StartXZDDRecord(datastream& kDataStream)
{
	NCScenceManager::Instance().HideAll(NULL);
	NCMJGameScence::Instance().EnterScence();
	if (!NCMJGameScence::Instance().StartRecord(kDataStream))
	{
		NCScenceManager::Instance().InHomeScence();
		return;
	}
}
void NCGameManager::HideAll()
{
	NCMJGameScence::Instance().HideAll();
}

IClientKernelSink* NCGameManager::CreateGame(word wKindID)
{
	NCScenceManager::Instance().HideAll(NULL);
	if (NCMJGameScence::KIND_ID == wKindID)
	{
		NCMJGameScence::Instance().EnterScence();
		return NCMJGameScence::pInstance();
	}
	return NULL;
}
void NCGameManager::loadGameBaseData(word wKindID)
{
	if (NCMJGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,NCMJGameScence::MAX_PLAYER,NCMJGameScence::VERSION_CLIENT,"Game");
	}
	
}