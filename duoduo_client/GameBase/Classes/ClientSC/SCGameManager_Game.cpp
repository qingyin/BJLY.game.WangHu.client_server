#include "SCGameManager.h"

#include "Scene/SCScenceManager.h"
#include "Game/XZDD/XZDDGameScence.h"

void SCGameManager::initGame()
{
	new XZDDGameScence();
	SCScenceManager::Instance().addGameNode(XZDDGameScence::pInstance());

}
void SCGameManager::StartXZDD()
{
	connectGameServerByKindID(XZDDGameScence::KIND_ID);
}
void SCGameManager::HideAll()
{
	XZDDGameScence::Instance().HideAll();
}

IClientKernelSink* SCGameManager::CreateGame(word wKindID)
{
	SCScenceManager::Instance().HideAll(NULL);
	if (XZDDGameScence::KIND_ID == wKindID)
	{
		XZDDGameScence::Instance().EnterScence();
		return XZDDGameScence::pInstance();
	}
	return NULL;
}
void SCGameManager::loadGameBaseData(word wKindID)
{
	if (XZDDGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,XZDDGameScence::MAX_PLAYER,XZDDGameScence::VERSION_CLIENT,"Game");
	}
	
}