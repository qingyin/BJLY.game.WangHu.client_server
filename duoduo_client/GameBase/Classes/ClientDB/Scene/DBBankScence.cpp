#include "DBBankScence.h"
#include "DBScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(DBBankScence);

DBBankScence::DBBankScence()
	:m_kInsureMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_kPasswordMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kInsureMission.setMissionSink(this);
	m_kPasswordMission.setMissionSink(this);
}
DBBankScence::~DBBankScence()
{

}
bool DBBankScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/DBBankScence.xml",this);

	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(DBBankScence::upPlayerInfo));

	WidgetManager::addButtonCB("Button_Bank",this,button_selector(DBBankScence::Button_Bank));
	WidgetManager::addButtonCB("Button_SetBankPasswordAction",this,button_selector(DBBankScence::Button_SetBankPasswordAction));
	WidgetManager::addButtonCB("Button_SaveGoldAction",this,button_selector(DBBankScence::Button_SaveGoldAction));
	WidgetManager::addButtonCB("Button_TakeGoldAction",this,button_selector(DBBankScence::Button_TakeGoldAction));
	WidgetManager::addButtonCB("Button_ChangeBankPasswordAction",this,button_selector(DBBankScence::Button_ChangeBankPasswordAction));
	
	
	return true;
}
void DBBankScence::HideAll()
{
	WidgetFun::setVisible(this,"BankPlane",false);
	WidgetFun::setVisible(this,"BankSavePlane",false);
	WidgetFun::setVisible(this,"BankTakePlane",false);
	WidgetFun::setVisible(this,"BankSetPasswordPlane",false);
	WidgetFun::setVisible(this,"BankChangePasswordPlane",false);

}
void DBBankScence::upPlayerInfo()
{
	utility::log("DBBankScence::upPlayerInfo()");
	WidgetFun::setText(this,"BankTakeScore0",UserInfo::Instance().getUserScore());
	WidgetFun::setText(this,"BankTakeScore1",UserInfo::Instance().getUserScore());
	WidgetFun::setText(this,"BankSaveScore0",UserInfo::Instance().getUserInsure());
	WidgetFun::setText(this,"BankSaveScore1",UserInfo::Instance().getUserInsure());
}
void DBBankScence::Button_Bank(cocos2d::Ref*,WidgetUserInfo*)
{
	HideAll();
	m_kInsureMission.query();
	if (UserInfo::Instance().getUserData()->cbInsureEnabled)
	{
		WidgetFun::setVisible(this,"BankPlane",true);
	}
	else
	{
		WidgetFun::setVisible(this,"BankSetPasswordPlane",true);
	}
}
void DBBankScence::Button_SetBankPasswordAction(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kPassword = WidgetFun::getEditeText(this,"SetBankPasswordEdit");
	if (kPassword.size() > 10 || kPassword.size() < 4)
	{
		NoticeMsg::Instance().ShowTopMsg("Password error");
		return;
	}
	m_kPasswordMission.modifyInsure("",kPassword.c_str());
}
void DBBankScence::Button_SaveGoldAction(cocos2d::Ref*,WidgetUserInfo*)
{
	int iSaveGold = utility::parseInt(WidgetFun::getEditeText(this,"BankSaveEdit"));
	if (iSaveGold > UserInfo::Instance().getUserScore())
	{
		NoticeMsg::Instance().ShowTopMsgByScript("BankSaveOut");
		return;
	}
	m_kInsureMission.save(iSaveGold);
}
void DBBankScence::Button_TakeGoldAction(cocos2d::Ref*,WidgetUserInfo*)
{
	int iTakeGold = utility::parseInt(WidgetFun::getEditeText(this,"BankTakeEdit"));
	if (iTakeGold > UserInfo::Instance().getUserInsure())
	{
		NoticeMsg::Instance().ShowTopMsgByScript("BankTalkOut");
		return;
	}
	std::string kPassword = WidgetFun::getEditeText(this,"BankTalkPasswordEdit");
	m_kInsureMission.take(iTakeGold,kPassword.c_str());
}
void DBBankScence::Button_ChangeBankPasswordAction(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kSrcBankPasswordEdit = WidgetFun::getEditeText(this,"SrcBankPasswordEdit");
	std::string kDestBankPasswordEdit = WidgetFun::getEditeText(this,"DestBankPasswordEdit");
	m_kPasswordMission.modifyInsure(kSrcBankPasswordEdit.c_str(),kDestBankPasswordEdit.c_str());
}
void DBBankScence::onInsureInfo()
{
	WidgetFun::setText(this,"RevenueTakeTxt",
		utility::paseFloat((float)UserInfo::Instance().GetUserInsureInfo()->wRevenueTake/10.f,1)+"%");
}																									
void DBBankScence::onInsureSuccess(int type, const char* szDescription)
{
	WidgetFun::setVisible(this,"BankSavePlane",false);
	WidgetFun::setVisible(this,"BankTakePlane",false);
	WidgetFun::setEditeText(this,"SetBankPasswordEdit","");
	WidgetFun::setEditeText(this,"BankTalkPasswordEdit","");
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}					
void DBBankScence::onInsureFailure(int type, const char* szDescription)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}					
bool DBBankScence::onInsureTransferConfirm(const char* szMessage)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szMessage));
	return false; 
}		
void DBBankScence::onInsureEnableResult(int type, const char* szMessage) 
{
}
void DBBankScence::onGPPasswordSuccess(int type, const char* szDescription)
{
	WidgetFun::setVisible(this,"BankSetPasswordPlane",false);
	WidgetFun::setVisible(this,"BankChangePasswordPlane",false);
	WidgetFun::setVisible(this,"BankPlane",true);
	WidgetFun::setEditeText(this,"SrcBankPasswordEdit","");
	WidgetFun::setEditeText(this,"DestBankPasswordEdit","");
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}
void DBBankScence::onGPPasswordFailure(int type, const char* szDescription)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}