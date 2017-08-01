#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"

#include "Kernel/kernel/IStringMessageSink.h"

class ScenceManagerBase
	:public FvSingletonBase<ScenceManagerBase>
{
public:
	ScenceManagerBase();
	~ScenceManagerBase();
public:
	virtual void GameBackScence() = 0;
};