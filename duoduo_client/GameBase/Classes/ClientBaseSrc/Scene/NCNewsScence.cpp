#include "NCNewsScence.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"

FV_SINGLETON_STORAGE(NCNewsScence);

NCNewsScence::NCNewsScence()
	:m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kPublicNoticMission.setMissionSink(this);
}
NCNewsScence::~NCNewsScence()
{

}
bool NCNewsScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNNewsScence.xml",this);
	WidgetManager::addButtonCB("Button_XinWen",this,button_selector(NCNewsScence::Button_XinWen));
	WidgetManager::addButtonCB("Button_NewsShow",this,button_selector(NCNewsScence::Button_NewsShow));
	
	return true;
}
void NCNewsScence::EnterScence()
{
	showNews(0);
}
void NCNewsScence::hideAll()
{
	WidgetFun::setVisible(this,"NewsPlane",false);
}
void NCNewsScence::showNews(int iNews)
{
	for (int i = 0;i<5;i++)
	{
		WidgetFun::setVisible(this,utility::toString("NewsPlane",i),false);
	}
	WidgetFun::setVisible(this,utility::toString("NewsPlane",iNews),true);

	if (iNews != 2)
	{
		m_kPublicNoticMission.PublicNotice(utility::toString("NC_NEWS_TXT_",iNews));
	}
}
void NCNewsScence::Button_XinWen(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"NewsPlane",true);
	EnterScence();
}
void NCNewsScence::Button_NewsShow(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	std::string kIdex = WidgetFun::getUserInfoValue(pUserInfo,"Idex");
	showNews(utility::parseInt(kIdex));
}
void NCNewsScence::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	if (WidgetFun::getChildWidget(this,kKey) != NULL)
	{
		WidgetFun::setText(this,kKey,utility::a_u8(szDescription));
	}
}