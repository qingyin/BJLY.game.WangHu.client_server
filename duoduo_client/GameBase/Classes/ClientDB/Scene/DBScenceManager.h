#pragma once
#include "cocos2d.h"
#include "Game/Script/TimeNode.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Game/ScenceManagerBase.h"


class DBScenceManager
	:public FvSingleton<DBScenceManager>
	,public ScenceManagerBase
	,public TimeNode
{
public:
	DBScenceManager(cocos2d::Node* pNode);
	DBScenceManager();
	~DBScenceManager();
public:
	virtual void init();
	void HideAll(cocos2d::Node* pActShowNode);
	cocos2d::Node* getRootNode();
	void update(float fTime);

public:
	void addGameNode(cocos2d::Node* pNode);
	void InLogonScence();
	void InHomeScence();
public:
	virtual void GameBackScence();
protected:
	cocos2d::Node*		m_pRootNode;
};

