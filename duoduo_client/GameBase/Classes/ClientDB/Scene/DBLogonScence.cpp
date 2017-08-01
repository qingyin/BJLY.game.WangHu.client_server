#include "DBLogonScence.h"
#include "DBScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(DBLogonScence);

DBLogonScence::DBLogonScence()
	:m_kLoginMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kLoginMission.setMissionSink(this);
}
DBLogonScence::~DBLogonScence()
{

}
bool DBLogonScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/DBLogonScence.xml",this);
	WidgetManager::addButtonCB("Button_Login",this,button_selector(DBLogonScence::Button_Login));
	WidgetManager::addButtonCB("Button_Visitor",this,button_selector(DBLogonScence::Button_Visitor));
	
	return true;
}
void DBLogonScence::onGPLoginSuccess()
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	cocos2d::UserDefault::getInstance()->setStringForKey("Accounts",pGlobalUserData->szAccounts);
	cocos2d::UserDefault::getInstance()->setStringForKey("Password",m_kPssword);

	DBScenceManager::Instance().InHomeScence();
}
void DBLogonScence::onGPLoginComplete()
{

}
void DBLogonScence::onGPLoginFailure(unsigned int iErrorCode,const char* szDescription)
{
	NoticeMsg::Instance().ShowTopMsg((szDescription));
}
void DBLogonScence::onGPError(int err)
{

}
void DBLogonScence::Button_Login(cocos2d::Ref*,WidgetUserInfo*)
{
}
void DBLogonScence::Button_Visitor(cocos2d::Ref*,WidgetUserInfo*)
{
	utility::log("Button_Visitor");
	std::string kPassword = utility::toString(rand()%1000000+100000);
	std::string kAddress = "";
	CMD_GP_VisitorLogon loginAccount;
	memset(&loginAccount,0,sizeof(loginAccount));
	loginAccount.cbGender = 0;
	loginAccount.wFaceID = 0;
	strcpy(loginAccount.szPassWord,(kPassword.c_str()) );
	strcpy(loginAccount.szNickName,"Visitor" );
	m_kLoginMission.loginVisitor(loginAccount);
	m_kPssword = kPassword;
}
void DBLogonScence::EnterScence()
{
	return;
	std::string kAccounts = cocos2d::UserDefault::getInstance()->getStringForKey("Accounts");
	std::string kPassword = cocos2d::UserDefault::getInstance()->getStringForKey("Password");
	if (kAccounts != "" && kPassword != "")
	{
		CMD_GP_LogonAccounts loginAccount;
		loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();
		loginAccount.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
		strcpy(loginAccount.szAccounts, kAccounts.c_str());
		strcpy(loginAccount.szPassword, kPassword.c_str());
		m_kLoginMission.loginAccount(loginAccount);
		m_kPssword = kPassword;
	}
	else
	{
		Button_Visitor(NULL,NULL);
	}
}