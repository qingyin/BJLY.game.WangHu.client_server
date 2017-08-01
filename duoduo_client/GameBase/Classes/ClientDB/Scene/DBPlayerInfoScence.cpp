#include "DBPlayerInfoScence.h"
#include "DBScenceManager.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Script/ActionEx.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"

#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(DBPlayerInfoScence);

DBPlayerInfoScence::DBPlayerInfoScence()
	:m_kIndividualMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kIndividualMission.setMissionSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(DBPlayerInfoScence::upPlayerInfo));
}
DBPlayerInfoScence::~DBPlayerInfoScence()
{

}
bool DBPlayerInfoScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/DBPlayerInfoScence.xml",this);

	WidgetManager::addButtonCB("Button_PlayerInfoScence",this,button_selector(DBPlayerInfoScence::Button_PlayerInfoScence));
	WidgetManager::addButtonCB("Button_SetSpreader",this,button_selector(DBPlayerInfoScence::Button_SetSpreader));
	WidgetManager::addButtonCB("Button_Gender1",this,button_selector(DBPlayerInfoScence::Button_ChangeGender));
	WidgetManager::addButtonCB("Button_Gender2",this,button_selector(DBPlayerInfoScence::Button_ChangeGender));
	WidgetFun::SetEditReturnAction(this,"PlayerInfoName",CC_CALLBACK_0(DBPlayerInfoScence::ChangePlayerName,this));
	return true;
}
void DBPlayerInfoScence::EnterScence()
{
}
void DBPlayerInfoScence::upPlayerInfo()
{
	utility::log("DBPlayerInfoScence::upPlayerInfo()");
	WidgetFun::setEditeText(this,"PlayerInfoName",UserInfo::Instance().getUserNicName());
	WidgetFun::setText(this,"PlayerInfoGold",UserInfo::Instance().getUserScore());
	WidgetFun::setText(this,"PlayerInfoBank",UserInfo::Instance().getUserInsure());
	WidgetFun::setText(this,"PlayerInfoAccount",UserInfo::Instance().getAccounts());
	WidgetFun::setEnable(this,"Button_Gender1",UserInfo::Instance().getGender());
	WidgetFun::setEnable(this,"Button_Gender2",!UserInfo::Instance().getGender());
}
void DBPlayerInfoScence::Button_PlayerInfoScence(cocos2d::Ref*,WidgetUserInfo*)
{
	setVisible(true);
	WidgetFun::setVisible(this,"PlayerInfoPlane",true);
	WidgetFun::setVisible(this,"SpreaderPlane",false);
}
void DBPlayerInfoScence::ChangePlayerName()
{
	std::string kName = (WidgetFun::getEditeText(this,"PlayerInfoName"));
	if (utility::utf8_len(kName) < 2)
	{
		NoticeMsg::Instance().ShowTopMsg("need 2");
		return;
	}
	kName = utility::u8_a(WidgetFun::getEditeText(this,"PlayerInfoName"));
	m_kIndividualMission.modifyName(kName);
}
void DBPlayerInfoScence::Button_SetSpreader(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kName = WidgetFun::getEditeText(this,"SpreaderEdit");
	if (utility::utf8_len(kName) < 2)
	{
		NoticeMsg::Instance().ShowTopMsg("need 2");
		return;
	}
	m_kIndividualMission.modifySpreader(kName);
}
void DBPlayerInfoScence::Button_ChangeGender(cocos2d::Ref*,WidgetUserInfo* pUerInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUerInfo,"Idex"));

	m_kIndividualMission.modifyGender(iIdex);
}
void DBPlayerInfoScence::onGPIndividualInfo(int type)
{

}
void DBPlayerInfoScence::onGPIndividualSuccess(int type, const char* szDescription)
{
	std::string kShowTxt = utility::a_u8(szDescription);
	WidgetFun::setVisible(this,"SpreaderPlane",false);
	NoticeMsg::Instance().ShowTopMsg(kShowTxt);
}
void DBPlayerInfoScence::onGPIndividualFailure(int type, const char* szDescription)
{
	upPlayerInfo();
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}