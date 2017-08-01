#include "HNNewsScence.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"

FV_SINGLETON_STORAGE(HNNewsScence);

HNNewsScence::HNNewsScence()
	:m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kPublicNoticMission.setMissionSink(this);
}
HNNewsScence::~HNNewsScence()
{

}
bool HNNewsScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNNewsScence.xml",this);
	WidgetManager::addButtonCB("Button_XinWen",this,button_selector(HNNewsScence::Button_XinWen));
	WidgetManager::addButtonCB("Button_NewsShow",this,button_selector(HNNewsScence::Button_NewsShow));
	
	return true;
}
void HNNewsScence::EnterScence()
{
	showNews(0);
}
void HNNewsScence::hideAll()
{
	WidgetFun::setVisible(this,"NewsPlane",false);
}
void HNNewsScence::showNews(int iNews)
{
	for (int i = 0;i<5;i++)
	{
		WidgetFun::setVisible(this,utility::toString("NewsPlane",i),false);
	}
	WidgetFun::setVisible(this,utility::toString("NewsPlane",iNews),true);

	if (iNews != 2)
	{
		m_kPublicNoticMission.PublicNotice(utility::toString("HN_NEWS_TXT_",iNews));
	}
}
void HNNewsScence::Button_XinWen(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"NewsPlane",true);
	EnterScence();
}
void HNNewsScence::Button_NewsShow(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	std::string kIdex = WidgetFun::getUserInfoValue(pUserInfo,"Idex");
	showNews(utility::parseInt(kIdex));
}
void HNNewsScence::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	if (WidgetFun::getChildWidget(this,kKey) != NULL)
	{
		WidgetFun::setText(this,kKey,utility::a_u8(szDescription));
	}
}