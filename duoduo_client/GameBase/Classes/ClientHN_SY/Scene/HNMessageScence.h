#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/MissionWeiXin.h"
#include "Platform/PFKernel/CGPGameRecord.h"

class HNMessageScence
	:public cocos2d::Node
	,public FvSingleton<HNMessageScence> 
{
public:
	HNMessageScence();
	~HNMessageScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();
public:
	void Button_Message(cocos2d::Ref*,WidgetUserInfo*);
};