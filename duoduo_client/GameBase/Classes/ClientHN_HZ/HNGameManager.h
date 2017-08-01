#pragma once

#include "cocos2d.h"
#include "Game/Game/GameManagerBase.h"

#include "Game/FV/FvSingleton.h"

class HNGameManager 
	:public GameManagerBase
	,public FvSingleton<HNGameManager>
{
public:
	HNGameManager();
	~HNGameManager();
public:
	void initGame();
	void StartXZDD();
	void StartXZDDRecord(datastream& kDataStream);
	void HideAll();

	IClientKernelSink* CreateGame(word wKindID);
	void loadGameBaseData(word wKindID);
};