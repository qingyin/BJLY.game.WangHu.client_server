#pragma once

#include "cocos2d.h"
#include "Game/Game/GameManagerBase.h"

#include "Game/FV/FvSingleton.h"

class SCGameManager 
	:public GameManagerBase
	,public FvSingleton<SCGameManager>
{
public:
	SCGameManager();
	~SCGameManager();
public:
	void initGame();
	void StartXZDD();
	void HideAll();

	IClientKernelSink* CreateGame(word wKindID);
	void loadGameBaseData(word wKindID);
};