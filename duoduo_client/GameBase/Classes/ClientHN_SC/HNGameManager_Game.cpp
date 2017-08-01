#include "HNGameManager.h"

#include "Scene/HNScenceManager.h"
#include "Game/Game/XZDD/XZDDGameScence.h"

void HNGameManager::initGame()
{
	new XZDDGameScence();
	HNScenceManager::Instance().addGameNode(XZDDGameScence::pInstance());
}
void HNGameManager::StartXZDD()
{
}
void HNGameManager::StartXZDDRecord(datastream& kDataStream)
{
	HNScenceManager::Instance().HideAll(NULL);
	XZDDGameScence::Instance().EnterScence();
	if (!XZDDGameScence::Instance().StartRecord(kDataStream))
	{
		HNScenceManager::Instance().InHomeScence();
		return;
	}
}
void HNGameManager::hideAll()
{
	XZDDGameScence::Instance().HideAll();
}

IClientKernelSink* HNGameManager::CreateGame(word wKindID)
{
	HNScenceManager::Instance().HideAll(NULL);
	if (XZDDGameScence::KIND_ID == wKindID)
	{
		XZDDGameScence::Instance().EnterScence();
		return XZDDGameScence::pInstance();
	}
	return NULL;
}
void HNGameManager::loadGameBaseData(word wKindID)
{
	if (XZDDGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,XZDDGameScence::MAX_PLAYER,XZDDGameScence::VERSION_CLIENT,"Game");
	}
	
}