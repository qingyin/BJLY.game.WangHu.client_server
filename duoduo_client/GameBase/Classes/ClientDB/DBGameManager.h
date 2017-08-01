#pragma once

#include "cocos2d.h"
#include "Kernel/kernel/server/IServerItemSink.h"
#include "Platform/PFDefine/data/ServerListData.h"
#include "kernel/kernel/server/CServerItem.h"

#include "Game/FV/FvSingleton.h"
#include "Game/Game/GameManagerBase.h"

class DBGameManager 
	:public GameManagerBase
	,public FvSingleton<DBGameManager>
{
public:
	DBGameManager();
	~DBGameManager();
public:
	void initGame();
	void StartBaccarat();
	void StartCar();
	void StartRound();

	void hideAll();

	IClientKernelSink* CreateGame(word wKindID);
	void loadGameBaseData(word wKindID);
};