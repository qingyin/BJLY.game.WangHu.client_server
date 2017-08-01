#include "DBHomeScence.h"
#include "DBScenceManager.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Script/ActionEx.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"

#include "Game/Game/UserInfo.h"
#include "Game/Script/SoundFun.h"

FV_SINGLETON_STORAGE(DBHomeScence);

DBHomeScence::DBHomeScence()
{
	init();
	CServerListData::shared()->SetServerListDataSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(DBHomeScence::upPlayerInfo));
}
DBHomeScence::~DBHomeScence()
{

}
bool DBHomeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/DBHomeScence.xml",this);

	init_Button();

	return true;
}
void DBHomeScence::EnterScence()
{
	upPlayerInfo();
	WidgetFun::runWidgetAction(this,"Game_List","MoveIn");
	WidgetFun::setVisible(this,"Button_HallReturn",false);
	WidgetFun::setVisible(this,"Game_List",true);
	WidgetFun::setVisible(this,"Game_HallList",false);
	SoundFun::Instance().playBackMusic("B3.mp3");
}
void DBHomeScence::upPlayerInfo()
{
	WidgetFun::setText(this,"HomeName",UserInfo::Instance().getUserNicName());
	WidgetFun::setText(this,"HomeGold",UserInfo::Instance().getUserScore());
}
void DBHomeScence::upGameSever()
{

}




