#pragma once
#include "cocos2d.h"
#include "Game/Script/TimeNode.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Game/ScenceManagerBase.h"


class SCScenceManager
	:public FvSingleton<SCScenceManager>
	,public TimeNode
	,public ScenceManagerBase
{
public:
	SCScenceManager(cocos2d::Node* pNode);
	SCScenceManager();
	~SCScenceManager();
public:
	virtual void init();
	void HideAll(cocos2d::Node* pActShowNode);
	cocos2d::Node* getRootNode();
	void update(float fTime);

public:
	void addGameNode(cocos2d::Node* pNode);
	void InLogonScence();
	void InHomeScence();
	void InPlayerInfoScence();
public:
	virtual void GameBackScence();

protected:
	int					m_iScenceTimeID;
	bool				m_bInBackground;
	cocos2d::Node*		m_pRootNode;
};

