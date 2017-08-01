#include "HNPrivateScenceSC.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
#include "kernel/kernel/server/CServerItem.h"
#include "../HNGameManager.h"
#include "GameLib/Game/Game/XZDD/CMD_XZDD.h"

FV_SINGLETON_STORAGE(HNPrivateScenceSC);

HNPrivateScenceSC::HNPrivateScenceSC()
	:m_iPlayerCoutIdex(0)
	,m_dwPlayRule(0)
{
	init();
}
HNPrivateScenceSC::~HNPrivateScenceSC()
{

}
bool HNPrivateScenceSC::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/HNPrivateScenceSC.xml",this);

	WidgetManager::addButtonCB("Button_JoinRoom",this,button_selector(HNPrivateScenceSC::Button_JoinNumReset));
	WidgetManager::addButtonCB("Button_JoinNum",this,button_selector(HNPrivateScenceSC::Button_JoinNum));
	WidgetManager::addButtonCB("Button_JoinNumDel",this,button_selector(HNPrivateScenceSC::Button_JoinNumDel));
	WidgetManager::addButtonCB("Button_JoinNumReset",this,button_selector(HNPrivateScenceSC::Button_JoinNumReset));

	WidgetManager::addButtonCB("Button_Show_Create_Private",this,button_selector(HNPrivateScenceSC::Button_Show_Create_Private));
	WidgetManager::addButtonCB("Button_Show_Join_Private",this,button_selector(HNPrivateScenceSC::Button_Show_Join_Private));
	WidgetManager::addButtonCB("Button_ZZCreateRoom",this,button_selector(HNPrivateScenceSC::Button_ZZCreateRoom));
	WidgetManager::addButtonCB("Button_CSCreateRoom",this,button_selector(HNPrivateScenceSC::Button_CSCreateRoom));
	WidgetManager::addButtonCB("Button_XZCreateRoom",this,button_selector(HNPrivateScenceSC::Button_XZCreateRoom));
	WidgetManager::addButtonCB("Button_DismissPrivate",this,button_selector(HNPrivateScenceSC::Button_DismissPrivate));
	WidgetManager::addButtonCB("Button_DismissPrivateNot",this,button_selector(HNPrivateScenceSC::Button_DismissPrivateNot));
	WidgetManager::addButtonCB("Button_PrivateAgagin",this,button_selector(HNPrivateScenceSC::Button_PrivateAgagin));

	WidgetManager::addButtonCB("Button_PrivatePlayCout1",this,button_selector(HNPrivateScenceSC::Button_PrivatePlayCout));
	WidgetManager::addButtonCB("Button_PrivatePlayCout0",this,button_selector(HNPrivateScenceSC::Button_PrivatePlayCout));

	WidgetManager::addButtonCB("Button_GameRule0",this,button_selector(HNPrivateScenceSC::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule1",this,button_selector(HNPrivateScenceSC::ButtonGameRule));

	setPlayerCoutIdex(0);
	setGameRuleIdex(GAME_RULE_XZ);
	setGameRuleIdex(GAME_RULE_NO_WAIT);
	setGameRuleIdex(GAME_RULE_HUAN3);
	return true;
}
void HNPrivateScenceSC::hideAll()
{
	WidgetFun::setVisible(this,"CreateRoomPlane",false);
	WidgetFun::setVisible(this,"JoinRoomPlane",false);
}
bool HNPrivateScenceSC::BackKey()
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
void HNPrivateScenceSC::setPlayerCoutIdex(int iIdex)
{
	for (int i = 0;i<2;i++)
	{
		WidgetFun::setChecked(this,utility::toString("Button_PrivatePlayCout",i),iIdex == i);
	}
	m_iPlayerCoutIdex = iIdex;
}
void HNPrivateScenceSC::Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo*)
{
	zeromemory(&m_kPrivateInfo,sizeof(m_kPrivateInfo));
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	UserInfo::Instance().checkInGameServer();
}
void HNPrivateScenceSC::Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo*)
{
	zeromemory(&m_kPrivateInfo,sizeof(m_kPrivateInfo));
	WidgetFun::setVisible(this,"JoinRoomPlane",true);
	Button_JoinNumReset(NULL,NULL);
	UserInfo::Instance().checkInGameServer();
}

void HNPrivateScenceSC::Button_ZZCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
}
void HNPrivateScenceSC::Button_CSCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
}
void HNPrivateScenceSC::Button_XZCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	if (UserInfo::Instance().getUserChannel() == "")
	{
		NoticeMsg::Instance().ShowTopMsgByScript("PrivateTxt3");
		return;
	}

	CMD_GR_Create_Private m_kSendNet;
	zeromemory(&m_kSendNet,sizeof(m_kSendNet));
	m_kSendNet.cbGameType = Type_Private;
	m_kSendNet.bGameTypeIdex = GAME_TYPE_CHENGDU;
	m_kSendNet.bGameRuleIdex = m_dwPlayRule;
	strncpy(m_kSendNet.stHttpChannel,UserInfo::Instance().getUserChannel().c_str(),
		UserInfo::Instance().getUserChannel().size());
	m_kSendNet.bPlayCoutIdex = m_iPlayerCoutIdex;
	ConnectAndCreatePrivateByKindID(302,m_kSendNet); 
}
void HNPrivateScenceSC::Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Again_Private kNetInfo;
	strncpy(kNetInfo.stHttpChannel,UserInfo::Instance().getUserChannel().c_str(),UserInfo::Instance().getUserChannel().size());
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_RIVATE_AGAIN,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceSC::Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setPlayerCoutIdex(iIdex);
}

void HNPrivateScenceSC::ButtonGameRule(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setGameRuleIdex(iIdex);
}

void HNPrivateScenceSC::setGameRuleIdex(int iIdex)
{
	if (iIdex == GAME_RULE_XZ ||iIdex == GAME_RULE_XL)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_RULE_XZ));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_RULE_XL));
	}

	if (iIdex == GAME_RULE_XL)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_RULE_DIN_QUE));
	}
	if (iIdex == GAME_RULE_XZ)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_RULE_DIN_QUE));
		FvMask::Add(m_dwPlayRule,_MASK_((dword)GAME_RULE_DIN_QUE));
	}

	if (FvMask::HasAny(m_dwPlayRule,_MASK_((dword)iIdex)))
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	else
	{
		FvMask::Add(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	WidgetFun::setChecked(this,"Button_GameRule0",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_RULE_XZ)));
	WidgetFun::setChecked(this,"Button_GameRule1",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_RULE_XL)));
}