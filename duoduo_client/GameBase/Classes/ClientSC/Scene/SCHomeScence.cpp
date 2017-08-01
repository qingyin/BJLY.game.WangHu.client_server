#include "SCHomeScence.h"
#include "SCScenceManager.h"
#include "Game/GameLib.h"

#include "SCSignInScence.h"

FV_SINGLETON_STORAGE(SCHomeScence);

SCHomeScence::SCHomeScence()
{
	init();
	CServerListData::shared()->SetServerListDataSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(SCHomeScence::upPlayerInfo));
}
SCHomeScence::~SCHomeScence()
{

}
bool SCHomeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCHomeScence.xml",this);

	init_Button();

	return true;
}
void SCHomeScence::ShowEffect()
{
	WidgetFun::runWidgetAction(this,"TopPlane","Come");
	WidgetFun::runWidgetAction(this,"PeoplePlane","Come");
	WidgetFun::runWidgetAction(this,"ButtomPlane","Come");
	WidgetFun::runWidgetAction(this,"LeftPlane","Come");
	WidgetFun::runWidgetAction(this,"RightPlane","Come");
}
void SCHomeScence::EnterScence()
{
	static bool bStatic = false;
	if (!bStatic)
	{
		ShowEffect();
		bStatic = true;
	}
	upPlayerInfo();
}
void SCHomeScence::upPlayerInfo()
{
	utility::log("SCHomeScence::upPlayerInfo()");
	WidgetFun::setText(this,"HomeName",UserInfo::Instance().getUserNicName());
	WidgetFun::setText(this,"HomeGold",UserInfo::Instance().getUserScore());
}
void SCHomeScence::upGameSever()
{

}