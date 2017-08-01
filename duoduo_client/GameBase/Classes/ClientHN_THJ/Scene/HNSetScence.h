#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/MissionWeiXin.h"
#include "Platform/PFKernel/CGPGameRecord.h"

class HNSetScence
	:public cocos2d::Node
	,public FvSingleton<HNSetScence> 
{
public:
	HNSetScence();
	~HNSetScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();

public:
	void Button_EffectGuan(cocos2d::Ref*,WidgetUserInfo*);
	void Button_EffectKai(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SoundKai(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SoundGuan(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SheZhi(cocos2d::Ref*,WidgetUserInfo*);
	void Button_SetChangeAccount(cocos2d::Ref*,WidgetUserInfo*);
	
};