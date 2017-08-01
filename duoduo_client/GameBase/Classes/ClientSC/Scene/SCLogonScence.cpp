#include "SCLogonScence.h"
#include "SCScenceManager.h"
#include "Game/GameLib.h"
#include "base/ccUtils.h"
#include "GameLib/JniCross/JniFun.h"
#include "GameLib/Tools/core/MD5.h"
FV_SINGLETON_STORAGE(SCLogonScence);

std::string SCLogonScence::WX_APPID = "wx79ad4c77ea07f41e";
std::string SCLogonScence::WX_APPSECRET = "66ad79231c6954ba68811afb14fad5e2";

#define  SC_PHONEBIND_URL  "http://www.bosengame.com:8088/BoSenWebServer.aspx?"
#define  SC_PHONEBIND_KEY  "89b5b987124d2ec3"

SCLogonScence::SCLogonScence()
	:m_kLoginMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kLoginMission.setMissionSink(this);
	MissionWeiXin::Instance().setMissionSink(this);
}
SCLogonScence::~SCLogonScence()
{

}
bool SCLogonScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCLogonScence.xml",this);
	WidgetManager::addButtonCB("Button_HomeBack",this,button_selector(SCLogonScence::Button_HomeBack));
	WidgetManager::addButtonCB("Button_Visitor",this,button_selector(SCLogonScence::Button_Visitor));
	WidgetManager::addButtonCB("Button_LogonAccount",this,button_selector(SCLogonScence::Button_LogonAccount));
	WidgetManager::addButtonCB("Button_BindPhoneAction",this,button_selector(SCLogonScence::Button_BindPhoneAction));

	WidgetManager::addButtonCB("Button_GerPhoneAccept",this,button_selector(SCLogonScence::Button_GerPhoneAccept));
	WidgetManager::addButtonCB("Button_BindPhone",this,button_selector(SCLogonScence::Button_BindPhone));

	
	return true;
}
void SCLogonScence::HideAll()
{
	WidgetFun::setVisible(this,"AcountPlaneVisitor",false);
	WidgetFun::setVisible(this,"AcountPlanePhone",false);
	WidgetFun::setVisible(this,"BindPhonePlane",false);
	WidgetFun::setVisible(this,"AccountLogonPlane",false);
}
void SCLogonScence::EnterScence()
{
	std::string kAccounts = cocos2d::UserDefault::getInstance()->getStringForKey("Accounts");
	std::string kPassword = cocos2d::UserDefault::getInstance()->getStringForKey("Password");
	//kPassword = "111111";
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
		Button_Visitor(NULL,NULL);
	}
}
void SCLogonScence::onGPLoginSuccess()
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	cocos2d::UserDefault::getInstance()->setStringForKey("Accounts",pGlobalUserData->szAccounts);
	cocos2d::UserDefault::getInstance()->setStringForKey("Password",m_kPssword);

	SCScenceManager::Instance().InHomeScence();
}
void SCLogonScence::onGPLoginComplete()
{
	SCScenceManager::Instance().InHomeScence();
}
void SCLogonScence::onGPLoginFailure(unsigned int iErrorCode,const char* szDescription)
{
	if (iErrorCode == 3 || iErrorCode == 1)
	{
		Button_Visitor(NULL,NULL);
	}
	else
	{
		NoticeMsg::Instance().ShowTopMsg((szDescription));
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SCScenceManager::InHomeScence,SCScenceManager::pInstance()),3.0f);
	}
}
void SCLogonScence::onGPError(int err)
{
	NoticeMsg::Instance().ShowTopMsg(utility::getScriptString("NetError"));
}
void SCLogonScence::Button_LogonAccount(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kAccounts = WidgetFun::getEditeText(this,"PhoneEdit");
	std::string kPassword = WidgetFun::getEditeText(this,"PasswordEdit");
	CMD_GP_LogonAccounts loginAccount;
	loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();
	loginAccount.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	strcpy(loginAccount.szAccounts, kAccounts.c_str());
	strcpy(loginAccount.szPassword, kPassword.c_str());
	m_kLoginMission.loginAccount(loginAccount);
	m_kPssword = kPassword;
}
void SCLogonScence::Button_HomeBack(cocos2d::Ref*,WidgetUserInfo*)
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pGlobalUserData->szAccounts[0] == 'V')
	{
		WidgetFun::ActionStart(this,"AcountPlaneVisitor");
	}
	else
	{
		WidgetFun::ActionStart(this,"AcountPlanePhone");
	}
}
void SCLogonScence::Button_Visitor(cocos2d::Ref*,WidgetUserInfo*)
{
	srand(time(NULL));
	utility::log("Button_Visitor");
	std::string kPassword = utility::toString(rand()%1000000+100000);
	std::string kAddress = "";
	CMD_GP_VisitorLogon loginAccount;
	memset(&loginAccount,0,sizeof(loginAccount));
	loginAccount.cbValidateFlags = DF::shared()->GetPlazaVersion();
	loginAccount.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	loginAccount.cbGender = 0;
	loginAccount.wFaceID = 0;
	strcpy(loginAccount.szPassWord,(kPassword.c_str()) );
	strcpy(loginAccount.szNickName,utility::toString(utility::toString(utility::getScriptString("YouKe"),
		rand()%10000+rand()%10000*10000+10000000).c_str()).c_str());
	m_kLoginMission.loginVisitor(loginAccount);

	m_kPssword = loginAccount.szPassWord;
}
void SCLogonScence::Button_BindPhoneAction(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::ActionStart(this,"BindPhonePlane");
}
void SCLogonScence::onWxLoginSuccess(WxUserInfo kWxUserInfo)
{

}

void SCLogonScence::onWxLoginFail(std::string kError)
{

}


void SCLogonScence::Button_GerPhoneAccept(cocos2d::Ref*,WidgetUserInfo*)
{

	Req_PhoneBinding();
}

void SCLogonScence::Req_PhoneBinding()
{
	std::string kType = "A2";
	std::string kAccount = UserInfo::Instance().getAccounts();
	std::string kPhoneNo = WidgetFun::getEditeText(this,"PhoneEdit");
	if (kPhoneNo.length()!= 11)
	{
		NoticeMsg::Instance().ShowTopMsgByScript("PhoneNumError");
		return;
	}

	std::string kSign = md5(kAccount+kPhoneNo+SC_PHONEBIND_KEY);

	std::string kUrl = SC_PHONEBIND_URL;

	MCWebReq::instance().pushValue(kUrl,"Type",kType);
	MCWebReq::instance().pushValue(kUrl,"Account",kAccount);
	MCWebReq::instance().pushValue(kUrl,"PhoneNo",kPhoneNo);
	MCWebReq::instance().pushValue(kUrl,"Messages",setPhoneCheakCode());
	MCWebReq::instance().pushValue(kUrl,"Sign",kSign);

	MCWebReq::instance().sendRequestDocumentUrl(kUrl,CC_CALLBACK_1(SCLogonScence::Rsp_PhoneBinding,this),nullptr);

	m_kPhoneNumber = kPhoneNo;
}
void SCLogonScence::Rsp_PhoneBinding(rapidjson::Document* pDoc)
{
	std::string kRet;
	std::string kMsg;

	if (pDoc->IsObject())
	{
		auto item = pDoc->MemberBegin();
		auto iend = pDoc->MemberEnd();
		int _id = 0;

		for (;item!=iend;item++)
		{
			auto data = &((*item).value);

			if (data->IsObject())
			{
				kRet = MCWebReq::getDataValueStr(data,"Status");
				kMsg = MCWebReq::getDataValueStr(data,"Msg");
			}

		}
	}
}

std::string SCLogonScence::setPhoneCheakCode()
{
	int maxNum = 9999;
	int minNum = 1000;
	srand( (unsigned)time(NULL));
	int num = rand()%(maxNum-minNum) + minNum;

	m_kPhoneCheakCode = utility::toString(num);

	return m_kPhoneCheakCode;
}

void SCLogonScence::Button_BindPhone( cocos2d::Ref*,WidgetUserInfo* )
{
	std::string kPhoneNum = WidgetFun::getEditeText(this,"PhoneEdit");
	std::string kCheakCode = WidgetFun::getEditeText(this,"AcceptEdit");

	if (kPhoneNum.length() != 11)
	{
		NoticeMsg::Instance().ShowTopMsgByScript("PhoneNumError");
		return;
	}
	if (kCheakCode != m_kPhoneCheakCode )
	{
		NoticeMsg::Instance().ShowTopMsgByScript("CheakCodeError");
		return;
	}
	UserInfo::Instance().setPhoneNumber(kPhoneNum);
}
