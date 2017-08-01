#include "HNGameManager.h"

#include "Scene/HNScenceManager.h"
#include "Game/HNMJ/HNMJGameScence.h"

void HNGameManager::initGame()
{
	new HNMJGameScence();
	HNScenceManager::Instance().addGameNode(HNMJGameScence::pInstance());
}
void HNGameManager::StartXZDD()
{
}
void HNGameManager::StartXZDDRecord(datastream& kDataStream)
{
}
void HNGameManager::HideAll()
{
	HNMJGameScence::Instance().HideAll();
}

IClientKernelSink* HNGameManager::CreateGame(word wKindID)
{
	HNScenceManager::Instance().HideAll(NULL);
	if (HNMJGameScence::KIND_ID == wKindID)
	{
		HNMJGameScence::Instance().EnterScence();
		return HNMJGameScence::pInstance();
	}
	return NULL;
}
void HNGameManager::loadGameBaseData(word wKindID)
{
	if (HNMJGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,HNMJGameScence::MAX_PLAYER,HNMJGameScence::VERSION_CLIENT,"Game");
	}
	
}