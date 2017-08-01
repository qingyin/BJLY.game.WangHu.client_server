#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"

class DBShopScence
	:public cocos2d::Node
	,public FvSingleton<DBShopScence> 
{
public:
	DBShopScence();
	~DBShopScence();
public:
	virtual bool init();
public:
	void Button_Shop(cocos2d::Ref*,WidgetUserInfo*);
};