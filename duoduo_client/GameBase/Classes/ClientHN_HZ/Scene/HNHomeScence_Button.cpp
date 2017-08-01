#include "HNHomeScence.h"

#include "Game/Widget/WidgetManager.h"

#include "GameLib/Game/GameLib.h"

#include "HNScenceManager.h"
#include "HNPrivateScenceHN.h"

void HNHomeScence::init_Button()
{
	WidgetManager::addButtonCB("Button_GameRoom",this,button_selector(HNHomeScence::Button_GameRoom));
	WidgetManager::addButtonCB("Button_BackHome",this,button_selector(HNHomeScence::Button_BackHome));
	WidgetManager::addButtonCB("Button_HomeHead",this,button_selector(HNHomeScence::Button_HomeHead));
	WidgetManager::addButtonCB("Button_HomeExit",this,button_selector(HNHomeScence::Button_HomeExit));


	WidgetManager::addButtonCB("HZButton_BuyFangKa",this,button_selector(HNHomeScence::HZButton_BuyFangKa));
	WidgetManager::addButtonCB("HZButton_Fuli",this,button_selector(HNHomeScence::HZButton_Fuli));
	WidgetManager::addButtonCB("HZButton_zhanji",this,button_selector(HNHomeScence::HZButton_zhanji));
	WidgetManager::addButtonCB("HZButton_fankui",this,button_selector(HNHomeScence::HZButton_fankui));
	WidgetManager::addButtonCB("HZButton_wanfa",this,button_selector(HNHomeScence::HZButton_wanfa));
	WidgetManager::addButtonCB("HZButton_FenQu",this,button_selector(HNHomeScence::HZButton_FenQu));
	WidgetManager::addButtonCB("HZButton_HongZhongCreate",this,button_selector(HNHomeScence::HZButton_HongZhongCreate));
	WidgetManager::addButtonCB("HZButton_HongZhongJoin",this,button_selector(HNHomeScence::HZButton_HongZhongJoin));
}
void HNHomeScence::Button_GameRoom(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iServerID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"GameServerID"));
	GameManagerBase::InstanceBase().connectGameServerByServerID(iServerID);
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
void HNHomeScence::Button_HomeExit(cocos2d::Ref*,WidgetUserInfo*)
{
	cocos2d::Director::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void HNHomeScence::HZButton_BuyFangKa(cocos2d::Ref*,WidgetUserInfo*)
{

}
void HNHomeScence::HZButton_Fuli(cocos2d::Ref*,WidgetUserInfo*)
{

}
void HNHomeScence::HZButton_zhanji(cocos2d::Ref*,WidgetUserInfo*)
{

}
void HNHomeScence::HZButton_fankui(cocos2d::Ref*,WidgetUserInfo*)
{

}
void HNHomeScence::HZButton_wanfa(cocos2d::Ref*,WidgetUserInfo*)
{

}
void HNHomeScence::HZButton_FenQu(cocos2d::Ref*,WidgetUserInfo*)
{

}
void HNHomeScence::HZButton_HongZhongCreate(cocos2d::Ref*,WidgetUserInfo*)
{
	HNPrivateScenceHN::Instance().Button_Show_Create_Private(NULL,NULL);
}
void HNHomeScence::HZButton_HongZhongJoin(cocos2d::Ref*,WidgetUserInfo*)
{
	HNPrivateScenceHN::Instance().Button_Show_Join_Private(NULL,NULL);
}