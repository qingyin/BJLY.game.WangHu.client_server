#include "DBGameManager.h"


FV_SINGLETON_STORAGE(DBGameManager);


DBGameManager::DBGameManager()
{
	initGame();
}

DBGameManager::~DBGameManager()
{
}

