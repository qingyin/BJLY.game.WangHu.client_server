#include "HNPrivateScenceHN.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
#include "kernel/kernel/server/CServerItem.h"
#include "Game/Game/HNMJ/CMD_HNMJ.h"

FV_SINGLETON_STORAGE(HNPrivateScenceHN);

HNPrivateScenceHN::HNPrivateScenceHN()
	:m_iPlayCoutIdex(0)
	,m_dwPlayRule(0)
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
	WidgetManager::addButtonCB("Button_Show_Create_Public",this,button_selector(HNPrivateScenceHN::Button_Show_Create_Public));
	WidgetManager::addButtonCB("Button_Show_Join_Private",this,button_selector(HNPrivateScenceHN::Button_Show_Join_Private));
	WidgetManager::addButtonCB("Button_CreateRoom",this,button_selector(HNPrivateScenceHN::Button_CreateRoom));
	WidgetManager::addButtonCB("Button_JoinPublic",this,button_selector(HNPrivateScenceHN::Button_JoinPublic));
	WidgetManager::addButtonCB("Button_DismissPrivate",this,button_selector(HNPrivateScenceHN::Button_DismissPrivate));
	WidgetManager::addButtonCB("Button_DismissPrivateNot",this,button_selector(HNPrivateScenceHN::Button_DismissPrivateNot));
	WidgetManager::addButtonCB("Button_PrivateAgagin",this,button_selector(HNPrivateScenceHN::Button_PrivateAgagin));

	WidgetManager::addButtonCB("Button_GameCout0",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));
	WidgetManager::addButtonCB("Button_GameCout1",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));

	WidgetManager::addButtonCB("Button_GameRule0",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule1",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule2",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule3",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule4",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule5",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	
	setPlayerCoutIdex(0);
	setGameRuleIdex(GAME_TYPE_ZZ_QIANGGANGHU);
//	setGameRuleIdex(GAME_TYPE_CS_ZHUANGXIANFEN);
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
		WidgetFun::setChecked(this,utility::toString("Button_GameCout",i),iIdex == i);
	}
	m_iPlayCoutIdex = iIdex;
}
void HNPrivateScenceHN::setGameRuleIdex(int iIdex)
{
	if (iIdex == GAME_TYPE_ZZ_QIANGGANGHU 
		||iIdex == GAME_TYPE_ZZ_ZIMOHU
		||iIdex == GAME_TYPE_ZZ_258)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_QIANGGANGHU));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZIMOHU));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_258));
	}
	if (iIdex == GAME_TYPE_ZZ_ZHANIAO2)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO4));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO6));
	}
	if (iIdex == GAME_TYPE_ZZ_ZHANIAO4)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO2));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO6));
	}
	if (iIdex == GAME_TYPE_ZZ_ZHANIAO6)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO4));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO2));
	}
	if (FvMask::HasAny(m_dwPlayRule,_MASK_((dword)iIdex)))
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	else
	{
		FvMask::Add(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	WidgetFun::setChecked(this,"Button_GameRule0",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_QIANGGANGHU)));
	WidgetFun::setChecked(this,"Button_GameRule1",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZIMOHU)));
	WidgetFun::setChecked(this,"Button_GameRule2",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_258)));
	WidgetFun::setChecked(this,"Button_GameRule3",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO2)));
	WidgetFun::setChecked(this,"Button_GameRule4",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO4)));
	WidgetFun::setChecked(this,"Button_GameRule5",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO6)));
}
void HNPrivateScenceHN::Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	UserInfo::Instance().checkInGameServer();
	WidgetFun::setVisible(this,"PlaneCreate",true);
	WidgetFun::setVisible(this,"PlanePublic",false);
}
void HNPrivateScenceHN::Button_Show_Create_Public(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	UserInfo::Instance().checkInGameServer();
	WidgetFun::setVisible(this,"PlaneCreate",false);
	WidgetFun::setVisible(this,"PlanePublic",true);
}
void HNPrivateScenceHN::Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	WidgetFun::setVisible(this,"JoinRoomPlane",true);
	Button_JoinNumReset(NULL,NULL);
	UserInfo::Instance().checkInGameServer();
}

void HNPrivateScenceHN::Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Again_Private kNetInfo;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_RIVATE_AGAIN,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceHN::Button_CreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet,sizeof(kSendNet));
	kSendNet.cbGameType = Type_Private;
	if(FvMask::HasAny(m_dwPlayRule,_MASK_(GAME_TYPE_ZZ_258)))
	{
		kSendNet.bGameTypeIdex = GAME_TYPE_CS;
		FvMask::Del(m_dwPlayRule,_MASK_(GAME_TYPE_ZZ_258));
	}
	else
	{
		kSendNet.bGameTypeIdex = GAME_TYPE_CS;
	}
	kSendNet.bGameRuleIdex = m_dwPlayRule;
	kSendNet.bPlayCoutIdex = m_iPlayCoutIdex;
	ConnectAndCreatePrivateByKindID(310,kSendNet);
}
void HNPrivateScenceHN::Button_JoinPublic(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet,sizeof(kSendNet));
	kSendNet.cbGameType = Type_Public;
	kSendNet.bGameRuleIdex = m_dwPlayRule;
	kSendNet.bGameTypeIdex = GAME_TYPE_CS;
	kSendNet.bPlayCoutIdex = m_iPlayCoutIdex;
	ConnectAndCreatePrivateByKindID(310,kSendNet);
}
void HNPrivateScenceHN::Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setPlayerCoutIdex(iIdex);
}
void HNPrivateScenceHN::ButtonGameRule(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setGameRuleIdex(iIdex);
}