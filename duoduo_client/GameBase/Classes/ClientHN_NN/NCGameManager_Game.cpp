#include "NCGameManager.h"

#include "Scene/NCScenceManager.h"
#include "Game/HNNN/NNGameScence.h"

void NCGameManager::initGame()
{
	new NNGameScence();
	NCScenceManager::Instance().addGameNode(NNGameScence::pInstance());
}
void NCGameManager::StartXZDD()
{
}
void NCGameManager::StartXZDDRecord(datastream& kDataStream)
{
}
void NCGameManager::HideAll()
{
	NNGameScence::Instance().HideAll();
}

IClientKernelSink* NCGameManager::CreateGame(word wKindID)
{
	NCScenceManager::Instance().HideAll(NULL);
	if (NNGameScence::KIND_ID == wKindID)
	{
		NNGameScence::Instance().EnterScence();
		return NNGameScence::pInstance();
	}
	return NULL;
}
void NCGameManager::loadGameBaseData(word wKindID)
{
	if (NNGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,NNGameScence::MAX_PLAYER,NNGameScence::VERSION_CLIENT,"Game");
	}
	
}