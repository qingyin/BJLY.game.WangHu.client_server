#include "HNLogonScence.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(HNLogonScence);

HNLogonScence::HNLogonScence()
	:m_kLoginMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	MissionWeiXin::Instance().setMissionSink(this);
	m_kLoginMission.setMissionSink(this);
}
HNLogonScence::~HNLogonScence()
{

}
bool HNLogonScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNLogonScence.xml",this);
	WidgetManager::addButtonCB("Button_WeiXinLogon",this,button_selector(HNLogonScence::Button_WeiXinLogon));
	WidgetFun::setChecked(this,"Button_UserXieYiCheak",true);
	return true;
}
void HNLogonScence::EnterScence()
{
	if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	{
		return ;
	}
	std::string kAccounts = cocos2d::UserDefault::getInstance()->getStringForKey("Accounts");
	std::string kPassword = cocos2d::UserDefault::getInstance()->getStringForKey("Password");
	if (kAccounts == "111111")
	{
		kAccounts = "222222";
	}
	else if (kAccounts == "222222")
	{
		kAccounts = "333333";
	}
	else if (kAccounts == "333333")
	{
		kAccounts = "444444";
	}
	else if (kAccounts == "444444")
	{
		kAccounts = "111111";
	}
	else
	{
		kAccounts = "111111";
	}
	kPassword = "111111";
	if (kAccounts != "" && kPassword != "")
	{
		m_kPssword = kPassword;
		CMD_GP_LogonAccounts loginAccount;
		loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();
		loginAccount.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
		strcpy(loginAccount.szAccounts, kAccounts.c_str());
		strcpy(loginAccount.szPassword, kPassword.c_str());
		m_kLoginMission.loginAccount(loginAccount);
	}
	else
	{
		m_kPssword = kPassword;
		CMD_GP_RegisterAccounts kRegister;
		zeromemory(&kRegister,sizeof(kRegister));
		kRegister.dwPlazaVersion = DF::shared()->GetPlazaVersion();
		kRegister.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
		kRegister.cbGender = 0;
		kRegister.wFaceID = 0;
		strncpy(kRegister.szAccounts,kAccounts.c_str(),kAccounts.size());
		strncpy(kRegister.szLogonPass,m_kPssword.c_str(),m_kPssword.size());
		std::string kNickName = (m_kWeiXinUserInfo.nickname);
		strncpy(kRegister.szNickName,kNickName.c_str(),kNickName.size()); 
		m_kLoginMission.registerServer(kRegister);
	}
}
void HNLogonScence::RegisterAccount()
{
	CCAssert(m_kWeiXinUserInfo.openid != "","");
	if (m_kWeiXinUserInfo.openid == "")
	{
		return;
	}
	std::string kAccounts = "WeiXin"+m_kWeiXinUserInfo.openid;
	m_kPssword = "WeiXinPassword";
	CMD_GP_RegisterAccounts kRegister;
	zeromemory(&kRegister,sizeof(kRegister));
	kRegister.dwPlazaVersion = DF::shared()->GetPlazaVersion();
	kRegister.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	kRegister.cbGender = 0;
	kRegister.wFaceID = 0;
	strncpy(kRegister.szAccounts,kAccounts.c_str(),kAccounts.size());
	strncpy(kRegister.szLogonPass,m_kPssword.c_str(),m_kPssword.size());
	std::string kNickName = (m_kWeiXinUserInfo.nickname);
	strncpy(kRegister.szNickName,kNickName.c_str(),kNickName.size()); 
	m_kLoginMission.registerServer(kRegister);
}
void HNLogonScence::onGPLoginSuccess()
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	cocos2d::UserDefault::getInstance()->setStringForKey("Accounts",pGlobalUserData->szAccounts);
	cocos2d::UserDefault::getInstance()->setStringForKey("Password",m_kPssword);

	HNScenceManager::Instance().InHomeScence();

	UserInfo::Instance().modeHeadHttp(UserInfo::Instance().getHeadHttp());
}
void HNLogonScence::onGPLoginComplete()
{
	HNScenceManager::Instance().InHomeScence();
}
void HNLogonScence::onGPLoginFailure(unsigned int iErrorCode,const char* szDescription)
{
	if (iErrorCode == 3 || iErrorCode == 1)
	{
		RegisterAccount();
	}
	else
	{
		NoticeMsg::Instance().ShowTopMsg((szDescription));
	}
}
void HNLogonScence::onGPError(int err)
{
	NoticeMsg::Instance().ShowTopMsg(utility::getScriptString("NetError"));
}
void HNLogonScence::onWxLoginSuccess(WxUserInfo kWxUserInfo )
{
	m_kWeiXinUserInfo = kWxUserInfo;

	std::string kAccounts = "WeiXin"+m_kWeiXinUserInfo.openid;
	m_kPssword = "WeiXinPassword";
	CMD_GP_LogonAccounts loginAccount;
	loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();
	loginAccount.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	strcpy(loginAccount.szAccounts, kAccounts.c_str());
	strcpy(loginAccount.szPassword, m_kPssword.c_str());
	m_kLoginMission.loginAccount(loginAccount);

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	strncpy(pGlobalUserData->szHeadHttp,kWxUserInfo.headimgurl.c_str(), countarray(pGlobalUserData->szHeadHttp));
}
void HNLogonScence::onWxLoginFail(std::string kError)
{

}
void HNLogonScence::Button_WeiXinLogon(cocos2d::Ref*,WidgetUserInfo*)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	//std::string kNickName = "12145";//utility::toString(time(NULL));
	std::string kNickName = utility::toString(time(NULL));
	m_kWeiXinUserInfo.openid = kNickName;
	m_kWeiXinUserInfo.nickname = kNickName;
	m_kWeiXinUserInfo.headimgurl = "http:\\assssssssssssssssssssssss";
	onWxLoginSuccess(m_kWeiXinUserInfo);
#else
	MissionWeiXin::Instance().logonWeiXin();
#endif
}