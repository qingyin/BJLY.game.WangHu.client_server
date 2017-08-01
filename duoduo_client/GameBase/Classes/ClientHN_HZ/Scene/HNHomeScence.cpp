#include "HNHomeScence.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
#include "JniCross/JniFun.h"

FV_SINGLETON_STORAGE(HNHomeScence);

HNHomeScence::HNHomeScence()
{
	init();
	CServerListData::shared()->SetServerListDataSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(HNHomeScence::upPlayerInfo));
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(HNHomeScence::LogonSucess));
}
HNHomeScence::~HNHomeScence()
{

}
bool HNHomeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNHomeScence.xml",this);

	init_Button();

	return true;
}
void HNHomeScence::EnterScence()
{
	upPlayerInfo();
}
void HNHomeScence::upPlayerInfo()
{
	utility::log("SCHomeScence::upPlayerInfo()");
	WidgetFun::setText(this,"HomeName",UserInfo::Instance().getUserNicName());
	WidgetFun::setPlaceText(this,"HomeFangKa",utility::toString(UserInfo::Instance().getUserInsure()));
	ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(this,"HeadImagic"),
		UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());
}
void HNHomeScence::LogonSucess()
{
	UserInfo::Instance().reqAccountInfo();
}
void HNHomeScence::upGameSever()
{

}