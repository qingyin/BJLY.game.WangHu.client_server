#include "HNHomeScence.h"

#include "Game/Widget/WidgetManager.h"

#include "GameLib/Game/GameLib.h"

#include "HNScenceManager.h"

void HNHomeScence::init_Button()
{
	WidgetManager::addButtonCB("Button_GameRoom",this,button_selector(HNHomeScence::Button_GameRoom));
	WidgetManager::addButtonCB("Button_BackHome",this,button_selector(HNHomeScence::Button_BackHome));
	WidgetManager::addButtonCB("Button_HomeHead",this,button_selector(HNHomeScence::Button_HomeHead));
	WidgetManager::addButtonCB("Button_HomeAddGold",this,button_selector(HNHomeScence::Button_HomeAddGold));
	WidgetManager::addButtonCB("Button_HomeExit",this,button_selector(HNHomeScence::Button_HomeExit));
	WidgetManager::addButtonCB("Button_SetChannel",this,button_selector(HNHomeScence::Button_SetChannel));
	
}
void HNHomeScence::Button_GameRoom(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iServerID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"GameServerID"));
}
void HNHomeScence::Button_BackHome(cocos2d::Ref*,WidgetUserInfo*)
{
	GameManagerBase::InstanceBase().disconnectServer();
	HNScenceManager::Instance().InHomeScence();
}
void HNHomeScence::Button_HomeHead(cocos2d::Ref*,WidgetUserInfo*)
{
	PopScence::Instance().showIPInfo(
		UserInfo::Instance().getUserNicName(),
		utility::paseInt(UserInfo::Instance().getUserID(),6),
		UserInfo::Instance().getUserIP(),
		UserInfo::Instance().getHeadHttp());
}
void HNHomeScence::Button_HomeAddGold(cocos2d::Ref*,WidgetUserInfo*)
{
	if (UserInfo::Instance().getUserChannel() == "")
	{
		WidgetFun::setVisible(this,"ChannelPlane",true);
	}
	else
	{
		WidgetFun::setVisible(this,"BuyPlane",true);
	}
}
void HNHomeScence::Button_HomeExit(cocos2d::Ref*,WidgetUserInfo*)
{

}
void HNHomeScence::Button_SetChannel(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kStr = WidgetFun::getEditeText(this,"Edit_HomeChannel");
	Req_UserInfo(kStr);
}