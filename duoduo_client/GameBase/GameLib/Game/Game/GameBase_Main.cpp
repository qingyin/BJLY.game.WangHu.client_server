#include "GameBase.h"

#include "Game/GameLib.h"
#include "ScenceManagerBase.h"
#include "GameManagerBase.h"


//启动游戏
bool GameBase::SetupGameClient()
{
	return true;
}
//重置游戏
void GameBase::ResetGameClient()
{
	return;
}
//关闭游戏
void GameBase::CloseGameClient(bool isExit)
{
	if (isExit)
	{
		GameManagerBase::InstanceBase().index = 1;
		ExitGame();
	}else{
		GameManagerBase::InstanceBase().index = 0;
	}
	
	return;
}