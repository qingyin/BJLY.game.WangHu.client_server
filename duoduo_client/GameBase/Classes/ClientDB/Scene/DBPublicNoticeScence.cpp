#include "DBPublicNoticeScence.h"
#include "DBScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(DBPublicNoticeScence);

DBPublicNoticeScence::DBPublicNoticeScence()
	:m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kPublicNoticMission.setMissionSink(this);
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(DBPublicNoticeScence::LogonSucess));
}
DBPublicNoticeScence::~DBPublicNoticeScence()
{

}
bool DBPublicNoticeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/DBPublicNoticeScence.xml",this);

	return true;
}
void DBPublicNoticeScence::LogonSucess()
{
	m_kPublicNoticMission.PublicNotice("PublicNotice");
}
void DBPublicNoticeScence::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	std::string kShowTxt = utility::a_u8(szDescription);
	std::string kNotice = cocos2d::UserDefault::getInstance()->getStringForKey("PublicNotice");
	if (kNotice == kShowTxt)
	{
		return;
	}
	cocos2d::UserDefault::getInstance()->setStringForKey("PublicNotice",kShowTxt);
	WidgetFun::setVisible(this,true);
	std::string kTxt = utility::a_u8(szDescription);
	utility::StringReplace(kTxt,"\\n","\n",true);
	WidgetFun::setText(this,"NoticeTxt",kTxt);
}