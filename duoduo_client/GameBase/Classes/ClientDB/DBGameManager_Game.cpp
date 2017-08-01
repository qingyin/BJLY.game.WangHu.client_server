#include "DBGameManager.h"
#include "Game/BJL/BJLGameScence.h"
#include "Game/Car/CarGameScence.h"
#include "Game/LP/LPGameScence.h"
#include "Scene/DBScenceManager.h"
#include "Game/Script/SoundFun.h"

USING_NS_CC;

void DBGameManager::initGame()
{
	new BJLGameScence();
	DBScenceManager::Instance().addGameNode(BJLGameScence::pInstance());
	BJLGameScence::pInstance()->setVisible(false);

	new CarGameScence();
	DBScenceManager::Instance().addGameNode(CarGameScence::pInstance());
	CarGameScence::pInstance()->setVisible(false);

	new LPGameScence();
	DBScenceManager::Instance().addGameNode(LPGameScence::pInstance());
	LPGameScence::pInstance()->setVisible(false);
}

void DBGameManager::StartBaccarat()
{
	connectGameServerByKindID(BJLGameScence::KIND_ID);
}

void DBGameManager::StartCar()
{
}

void DBGameManager::StartRound()
{
	//connectGameServerByKindID(CarGameScence::KIND_ID);
	connectGameServerByKindID(LPGameScence::KIND_ID);
}
void DBGameManager::hideAll()
{
	BJLGameScence::pInstance()->setVisible(false);
	CarGameScence::pInstance()->setVisible(false);
	LPGameScence::pInstance()->setVisible(false);
	BJLGameScence::Instance().clearInfo();
	CarGameScence::Instance().clearInfo();
	LPGameScence::Instance().clearInfo();
}

IClientKernelSink* DBGameManager::CreateGame(word wKindID)
{
	DBScenceManager::Instance().HideAll(NULL);
	if (BJLGameScence::KIND_ID == wKindID)
	{
		SoundFun::Instance().playBackMusic("B2.mp3");
		int lEnterScore = mGameServerItem->m_GameServer.lEnterScore;
		BJLGameScence::Instance().EnterScence(lEnterScore);
		BJLGameScence::Instance().setUserRight(m_dwUserRight);
		return BJLGameScence::pInstance();
	}
	else if (CarGameScence::KIND_ID == wKindID )
	{
		BJLGameScence::Instance().clearInfo();
		CarGameScence::Instance().EnterScence();
		return CarGameScence::pInstance();
	}
	else if (LPGameScence::KIND_ID == wKindID )
	{
		BJLGameScence::Instance().clearInfo();
		LPGameScence::Instance().EnterScence();
		return LPGameScence::pInstance();
	}
	
	DBScenceManager::Instance().InHomeScence();
	return NULL;
}
void DBGameManager::loadGameBaseData(word wKindID)
{
	 if (BJLGameScence::KIND_ID == wKindID)
	 {
		 DF::shared()->init(BJLGameScence::KIND_ID,100,BJLGameScence::VERSION_CLIENT,"Game");
	 }
	 else if (CarGameScence::KIND_ID == wKindID)
	 {
		 DF::shared()->init(CarGameScence::KIND_ID,100,CarGameScence::VERSION_CLIENT,"Game");
	 }
	 else if (LPGameScence::KIND_ID == wKindID)
	 {
		 DF::shared()->init(LPGameScence::KIND_ID,100,LPGameScence::VERSION_CLIENT,"Game");
	 }
}