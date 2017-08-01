#include "DBHomeScence.h"
#include "Game/Widget/WidgetManager.h"
#include "ClientDB/DBGameManager.h"
#include "DBScenceManager.h"
#include "Game/GameLib.h"

void DBHomeScence::init_Button()
{
	WidgetManager::addButtonCB("Button_EnterGame",this,button_selector(DBHomeScence::Button_EnterGame));
	WidgetManager::addButtonCB("Button_BackHome",this,button_selector(DBHomeScence::Button_BackHome));
	WidgetManager::addButtonCB("Button_Rank",this,button_selector(DBHomeScence::Button_Shop));
	WidgetManager::addButtonCB("Button_BJL",this,button_selector(DBHomeScence::Button_BJL));
	WidgetManager::addButtonCB("Button_CAR",this,button_selector(DBHomeScence::Button_CAR));
	WidgetManager::addButtonCB("Button_LP",this,button_selector(DBHomeScence::Button_LP));

	WidgetManager::addButtonCB("Button_BJLHall",this,button_selector(DBHomeScence::Button_BJLHall));
	WidgetManager::addButtonCB("Button_HallReturn",this,button_selector(DBHomeScence::Button_HallReturn));
}
void DBHomeScence::Button_EnterGame(cocos2d::Ref*,WidgetUserInfo*)
{
//	DBGameManager::Instance().StartBaccarat();
}
void DBHomeScence::Button_BackHome(cocos2d::Ref*,WidgetUserInfo*)
{
	DBScenceManager::Instance().InHomeScence();
}
void DBHomeScence::Button_Shop( cocos2d::Ref*,WidgetUserInfo* )
{
}
void DBHomeScence::Button_BJL( cocos2d::Ref*,WidgetUserInfo* )
{
	WidgetFun::runWidgetAction(this,"Game_HallList","MoveIn");
	WidgetFun::setVisible(this,"Button_HallReturn",true);
	WidgetFun::setVisible(this,"Game_HallList",true);
	WidgetFun::setVisible(this,"Game_List",false);
}

void DBHomeScence::Button_CAR( cocos2d::Ref*,WidgetUserInfo* )
{
	DBGameManager::Instance().StartCar();
}
void DBHomeScence::Button_LP( cocos2d::Ref*,WidgetUserInfo* )
{
	DBGameManager::Instance().StartRound();
}

void DBHomeScence::Button_BJLHall(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iServerID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"GameServerID"));
	DBGameManager::Instance().connectGameServerByServerID(iServerID);
}
void DBHomeScence::Button_HallReturn( cocos2d::Ref*,WidgetUserInfo* )
{
	WidgetFun::runWidgetAction(this,"Game_List","MoveIn");
	WidgetFun::setVisible(this,"Button_HallReturn",false);
	WidgetFun::setVisible(this,"Game_HallList",false);
	WidgetFun::setVisible(this,"Game_List",true);
}