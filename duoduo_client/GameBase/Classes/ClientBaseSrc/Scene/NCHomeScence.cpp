#include "NCHomeScence.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "JniCross/JniFun.h"

FV_SINGLETON_STORAGE(NCHomeScence);

NCHomeScence::NCHomeScence()
{
	init();
	CServerListData::shared()->SetServerListDataSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(NCHomeScence::upPlayerInfo));
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(NCHomeScence::LogonSucess));
}
NCHomeScence::~NCHomeScence()
{

}
bool NCHomeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNHomeScence.xml",this);

	init_Button();

	return true;
}
void NCHomeScence::EnterScence()
{
	upPlayerInfo();
}
void NCHomeScence::upPlayerInfo()
{
	utility::log("SCHomeScence::upPlayerInfo()");
	WidgetFun::setText(this,"HomeID",utility::paseInt(UserInfo::Instance().getUserID(),6));
	WidgetFun::setText(this,"HomeName",UserInfo::Instance().getUserNicName());
	WidgetFun::setText(this,"HomeGold",UserInfo::Instance().getUserScore());
	WidgetFun::setText(this,"HomeFangKa",UserInfo::Instance().getUserInsure());
	ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(this,"HeadImagic"),
		UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());
}
void NCHomeScence::LogonSucess()
{
	UserInfo::Instance().reqAccountInfo();
}
void NCHomeScence::upGameSever()
{

}

