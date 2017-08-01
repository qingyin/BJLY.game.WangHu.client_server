#include "SCPlayerInfoScence.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Script/ActionEx.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"

#include "Game/GameLib.h"

#include "SCScenceManager.h"

FV_SINGLETON_STORAGE(SCPlayerInfoScence);

SCPlayerInfoScence::SCPlayerInfoScence()
	:m_kIndividualMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_iGender(0)
{
	init();
	m_kIndividualMission.setMissionSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(SCPlayerInfoScence::upPlayerInfo));
}
SCPlayerInfoScence::~SCPlayerInfoScence()
{

}
bool SCPlayerInfoScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCPlayerInfoScence.xml",this);

	WidgetManager::addButtonCB("Button_PlayerInfoScence",this,button_selector(SCPlayerInfoScence::Button_PlayerInfoScence));
	WidgetManager::addButtonCB("Button_SetSpreader",this,button_selector(SCPlayerInfoScence::Button_SetSpreader));
	WidgetManager::addButtonCB("Button_Gender1",this,button_selector(SCPlayerInfoScence::Button_ChangeGender));
	WidgetManager::addButtonCB("Button_Gender2",this,button_selector(SCPlayerInfoScence::Button_ChangeGender));
	WidgetManager::addButtonCB("Button_ChangePlayerInfo",this,button_selector(SCPlayerInfoScence::Button_ChangePlayerInfo));
	WidgetFun::SetEditReturnAction(this,"PlayerInfoName",CC_CALLBACK_0(SCPlayerInfoScence::ChangePlayerName,this));
	return true;
}
void SCPlayerInfoScence::EnterScence()
{
	UserInfo::Instance().reqIndividual();
	upPlayerInfo();
	WidgetFun::setVisible(this,"Button_ChangePlayerInfo",false);
}
void SCPlayerInfoScence::upPlayerInfo()
{
	utility::log("SCPlayerInfoScence::upPlayerInfo()");
	WidgetFun::setEditeText(this,"PlayerInfoName",UserInfo::Instance().getUserNicName());
	WidgetFun::setText(this,"PlayerInfoGold",UserInfo::Instance().getUserScore());
	WidgetFun::setText(this,"PlayerInfoAccount",UserInfo::Instance().getAccounts());
	char acHour[8]={0};
	sprintf(acHour, "%06d", UserInfo::Instance().getUserID());
	WidgetFun::setText(this,"PlayerInfoID",acHour);
	WidgetFun::setEnable(this,"Button_Gender1",(bool)UserInfo::Instance().getGender());
	WidgetFun::setEnable(this,"Button_Gender2",!(bool)UserInfo::Instance().getGender());
	m_iGender = UserInfo::Instance().getGender();

	std::string kPhoneNumer = UserInfo::Instance().getPhoneNumber();
	if (kPhoneNumer.length() == 11)
	{
		WidgetFun::setText(this,"PlayerInfo_PhoneNumber",kPhoneNumer);
		WidgetFun::setVisible(this,"Button_BindPhoneAction",false);
	}
	else
	{
		WidgetFun::setText(this,"PlayerInfo_PhoneNumber",ScriptData<std::string>("NoPhoneNum").Value());
		WidgetFun::setVisible(this,"Button_BindPhoneAction",true);
	}

}
void SCPlayerInfoScence::Button_PlayerInfoScence(cocos2d::Ref*,WidgetUserInfo*)
{
	SCScenceManager::Instance().InPlayerInfoScence();
	WidgetFun::setVisible(this,"PlayerInfoPlane",true);
}
void SCPlayerInfoScence::ChangePlayerName()
{
	std::string kName = (WidgetFun::getEditeText(this,"PlayerInfoName"));
	if (utility::utf8_len(kName) < 2)
	{
		NoticeMsg::Instance().ShowTopMsg("need 2");
		return;
	}

	WidgetFun::setVisible(this,"Button_ChangePlayerInfo",true);
}
void SCPlayerInfoScence::Button_SetSpreader(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kName = WidgetFun::getEditeText(this,"SpreaderEdit");
	if (utility::utf8_len(kName) < 2)
	{
		NoticeMsg::Instance().ShowTopMsg("need 2");
		return;
	}
	m_kIndividualMission.modifySpreader(kName);
}
void SCPlayerInfoScence::Button_ChangeGender(cocos2d::Ref*,WidgetUserInfo* pUerInfo)
{
	m_iGender = utility::parseInt(WidgetFun::getUserInfoValue(pUerInfo,"Idex"));

	WidgetFun::setVisible(this,"Button_ChangePlayerInfo",true);
}
void SCPlayerInfoScence::Button_ChangePlayerInfo(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kName = (WidgetFun::getEditeText(this,"PlayerInfoName"));
	if (utility::utf8_len(kName) < 2)
	{
		NoticeMsg::Instance().ShowTopMsg("need 2");
		return;
	}
	kName = utility::u8_a(WidgetFun::getEditeText(this,"PlayerInfoName"));
//	m_kIndividualMission.modify(kName,m_iGender);
	WidgetFun::setVisible(this,"Button_ChangePlayerInfo",false); 
}
void SCPlayerInfoScence::onGPIndividualInfo(int type)
{

}
void SCPlayerInfoScence::onGPIndividualSuccess(int type, const char* szDescription)
{
	std::string kShowTxt = utility::a_u8(szDescription);
	NoticeMsg::Instance().ShowTopMsg(kShowTxt);
}
void SCPlayerInfoScence::onGPIndividualFailure(int type, const char* szDescription)
{
	upPlayerInfo();
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}