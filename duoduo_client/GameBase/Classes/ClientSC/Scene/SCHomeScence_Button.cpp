#include "SCHomeScence.h"

#include "Game/Widget/WidgetManager.h"

#include "../SCGameManager.h"
#include "GameLib/Game/GameLib.h"

#include "SCSignInScence.h"
#include "SCLowProtectScence.h"
#include "SCPlayerInfoScence.h"
#include "SCScenceManager.h"
#include "SCMatchScence.h"
#include "GameLib/Game/Game/MissionWeiXin.h"
#include "SCExchangeScence.h"
#include "SCShopScence.h"

void SCHomeScence::init_Button()
{
	WidgetManager::addButtonCB("Button_GameRoom",this,button_selector(SCHomeScence::Button_GameRoom));
	WidgetManager::addButtonCB("Button_BackHome",this,button_selector(SCHomeScence::Button_BackHome));
	WidgetManager::addButtonCB("Button_ShowMatchSignup",this,button_selector(SCHomeScence::Button_ShowMatchSignup));
	WidgetManager::addButtonCB("Button_GameRoom_Quick",this,button_selector(SCHomeScence::Button_GameRoom_Quick));
	WidgetManager::addButtonCB("Button_DuiHuan",this,button_selector(SCHomeScence::Button_DuiHuan));
	WidgetManager::addButtonCB("Button_ShangCheng",this,button_selector(SCHomeScence::Button_ShangCheng));
}
void SCHomeScence::Button_GameRoom(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iServerID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"GameServerID"));
	SCGameManager::Instance().connectGameServerByServerID(iServerID);
}
void SCHomeScence::Button_BackHome(cocos2d::Ref*,WidgetUserInfo*)
{
	SCScenceManager::Instance().InHomeScence();
}
void SCHomeScence::Button_ShowMatchSignup(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iServerID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"GameServerID"));

	SCMatchScence::Instance().ShowSignupMatch(iServerID);
}
void SCHomeScence::Button_GameRoom_Quick(cocos2d::Ref*,WidgetUserInfo*)
{
	SCGameManager::Instance().StartXZDD();
}

void SCHomeScence::Button_DuiHuan(cocos2d::Ref*,WidgetUserInfo*)
{
	SCExchangeScence::Instance().EnterScence();
}
void SCHomeScence::Button_ShangCheng(cocos2d::Ref*,WidgetUserInfo*)
{
	SCShopScence::Instance().EnterScence();
}