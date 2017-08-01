#include "NCHomeScence.h"

#include "Game/Widget/WidgetManager.h"

#include "GameLib/Game/GameLib.h"

#include "NCScenceManager.h"

void NCHomeScence::init_Button()
{
	WidgetManager::addButtonCB("Button_GameRoom",this,button_selector(NCHomeScence::Button_GameRoom));
	WidgetManager::addButtonCB("Button_BackHome",this,button_selector(NCHomeScence::Button_BackHome));
	WidgetManager::addButtonCB("Button_HomeHead",this,button_selector(NCHomeScence::Button_HomeHead));
	WidgetManager::addButtonCB("Button_HomeExit",this,button_selector(NCHomeScence::Button_HomeExit));
	WidgetManager::addButtonCB("Button_HomeGameHall",this,button_selector(NCHomeScence::Button_HomeGameHall));
}
void NCHomeScence::Button_GameRoom(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iServerID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"GameServerID"));
	GameManagerBase::InstanceBase().connectGameServerByServerID(iServerID);
}
void NCHomeScence::Button_BackHome(cocos2d::Ref*,WidgetUserInfo*)
{
	GameManagerBase::InstanceBase().disconnectServer();
	NCScenceManager::Instance().InHomeScence();
}
void NCHomeScence::Button_HomeHead(cocos2d::Ref*,WidgetUserInfo*)
{
	PopScence::Instance().showIPInfo(
		UserInfo::Instance().getUserNicName(),
		utility::paseInt(UserInfo::Instance().getUserID(),6),
		UserInfo::Instance().getUserIP(),
		UserInfo::Instance().getHeadHttp());
}
void NCHomeScence::Button_HomeExit(cocos2d::Ref*,WidgetUserInfo*)
{
	cocos2d::Director::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void NCHomeScence::Button_HomeGameHall(cocos2d::Ref*,WidgetUserInfo*)
{

}