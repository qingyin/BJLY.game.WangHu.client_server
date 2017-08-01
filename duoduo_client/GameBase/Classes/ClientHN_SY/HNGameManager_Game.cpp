#include "HNGameManager.h"

#include "Scene/HNScenceManager.h"
#include "GameLib/Game/Game//SYMJ/SYMJGameScence.h"

void HNGameManager::initGame()
{
	new SYMJGameScence(0,0);
	HNScenceManager::Instance().addGameNode(SYMJGameScence::pInstance());
}
void HNGameManager::StartXZDD()
{
}
void HNGameManager::StartXZDDRecord(datastream& kDataStream)
{
	HNScenceManager::Instance().HideAll(NULL);
	SYMJGameScence::Instance().EnterScence();
	if (!SYMJGameScence::Instance().StartRecord(kDataStream))
	{
		HNScenceManager::Instance().InHomeScence();
		return;
	}
}
void HNGameManager::hideAll()
{
	SYMJGameScence::Instance().HideAll();
}

IClientKernelSink* HNGameManager::CreateGame(word wKindID)
{
	HNScenceManager::Instance().HideAll(NULL);
	if (SYMJGameScence::KIND_ID == wKindID)
	{
		SYMJGameScence::Instance().EnterScence();
		return SYMJGameScence::pInstance();
	}
	return NULL;
}
void HNGameManager::loadGameBaseData(word wKindID)
{
	if (SYMJGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,SYMJGameScence::MAX_PLAYER,SYMJGameScence::VERSION_CLIENT,"Game");
	}
	
}