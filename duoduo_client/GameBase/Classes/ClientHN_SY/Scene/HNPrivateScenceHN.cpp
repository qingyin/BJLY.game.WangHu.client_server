#include "HNPrivateScenceHN.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
#include "ClientSC/SCGameManager.h"
#include "kernel/kernel/server/CServerItem.h"
#include "../HNGameManager.h"
#include "Game/Game/SYMJ/CMD_SYMJ.h"
#include "Game/Game/SYMJ/SYMJGameScence.h"

#define GAME_RULE_NO_WAIT 1

FV_SINGLETON_STORAGE(HNPrivateScenceHN);

HNPrivateScenceHN::HNPrivateScenceHN()
	:m_iPlayerCoutIdex(0)
	,m_dwPlayRule(0)
	,m_iGameTypeIndex(GAME_TYPE_SY)
{
	init();
}
HNPrivateScenceHN::~HNPrivateScenceHN()
{

}
bool HNPrivateScenceHN::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/HNPrivateScenceHN.xml",this);

	WidgetManager::addButtonCB("Button_JoinRoom",this,button_selector(HNPrivateScenceHN::Button_JoinNumReset));
	WidgetManager::addButtonCB("Button_JoinNum",this,button_selector(HNPrivateScenceHN::Button_JoinNum));
	WidgetManager::addButtonCB("Button_JoinNumDel",this,button_selector(HNPrivateScenceHN::Button_JoinNumDel));
	WidgetManager::addButtonCB("Button_JoinNumReset",this,button_selector(HNPrivateScenceHN::Button_JoinNumReset));

	WidgetManager::addButtonCB("Button_Show_Create_Private",this,button_selector(HNPrivateScenceHN::Button_Show_Create_Private));
	WidgetManager::addButtonCB("Button_Show_Join_Private",this,button_selector(HNPrivateScenceHN::Button_Show_Join_Private));
	WidgetManager::addButtonCB("Button_SYCreateRoom",this,button_selector(HNPrivateScenceHN::Button_SYCreateRoom));
	WidgetManager::addButtonCB("Button_DismissPrivate",this,button_selector(HNPrivateScenceHN::Button_DismissPrivate));
	WidgetManager::addButtonCB("Button_DismissPrivateNot",this,button_selector(HNPrivateScenceHN::Button_DismissPrivateNot));
	WidgetManager::addButtonCB("Button_PrivateAgagin",this,button_selector(HNPrivateScenceHN::Button_PrivateAgagin));

	WidgetManager::addButtonCB("Button_PrivatePlayCout1",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));
	WidgetManager::addButtonCB("Button_PrivatePlayCout0",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));

	WidgetManager::addButtonCB("Button_GameRule0",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule1",this,button_selector(HNPrivateScenceHN::ButtonGameRule));

	setPlayerCoutIdex(0);
	setGameRuleIdex(GAME_TYPE_ZIMO_HU);
	return true;
}
void HNPrivateScenceHN::hideAll()
{
	WidgetFun::setVisible(this,"CreateRoomPlane",false);
	WidgetFun::setVisible(this,"JoinRoomPlane",false);
}
bool HNPrivateScenceHN::BackKey()
{
	if (WidgetFun::isWidgetVisble(this,"CreateRoomPlane"))
	{
		WidgetFun::setVisible(this,"CreateRoomPlane",false);
		return true;
	}
	if (WidgetFun::isWidgetVisble(this,"JoinRoomPlane"))
	{
		WidgetFun::setVisible(this,"JoinRoomPlane",false);
		return true;
	}
	return false;
}
void HNPrivateScenceHN::setPlayerCoutIdex(int iIdex)
{
	for (int i = 0;i<2;i++)
	{
		WidgetFun::setChecked(this,utility::toString("Button_PrivatePlayCout",i),iIdex == i);
	}
	m_iPlayerCoutIdex = iIdex;
}
void HNPrivateScenceHN::Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo*)
{
	zeromemory(&m_kPrivateInfo,sizeof(m_kPrivateInfo));
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	UserInfo::Instance().checkInGameServer();
}
void HNPrivateScenceHN::Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo*)
{
	zeromemory(&m_kPrivateInfo,sizeof(m_kPrivateInfo));
	WidgetFun::setVisible(this,"JoinRoomPlane",true);
	Button_JoinNumReset(NULL,NULL);
	UserInfo::Instance().checkInGameServer();
}

void HNPrivateScenceHN::Button_SYCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	createRoom(SYMJGameScence::KIND_ID);
}

void HNPrivateScenceHN::Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Again_Private kNetInfo;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_RIVATE_AGAIN,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceHN::Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setPlayerCoutIdex(iIdex);
}

void HNPrivateScenceHN::ButtonGameRule( cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setGameRuleIdex(iIdex);
}

void HNPrivateScenceHN::createRoom( int nKindID )
{
	CMD_GR_Create_Private m_kSendNet;
	zeromemory(&m_kSendNet,sizeof(m_kSendNet));
	m_kSendNet.bGameRuleIdex = m_dwPlayRule;
	m_kSendNet.cbGameType = Type_Private;
	m_kSendNet.bGameTypeIdex = 0;
	m_kSendNet.bPlayCoutIdex = m_iPlayerCoutIdex;
	ConnectAndCreatePrivateByKindID(nKindID,m_kSendNet); 
}

void HNPrivateScenceHN::setGameRuleIdex(int iIdex)
{
	if (FvMask::HasAny(m_dwPlayRule,_MASK_((dword)iIdex)))
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	else
	{
		FvMask::Add(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	WidgetFun::setChecked(this,"Button_GameRule0",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_SY_FENG)));
	WidgetFun::setChecked(this,"Button_GameRule1",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_SY_CHI)));
}
