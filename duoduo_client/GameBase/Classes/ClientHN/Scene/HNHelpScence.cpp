#include "HNHelpScence.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(HNHelpScence);

HNHelpScence::HNHelpScence()
{
	init();
}
HNHelpScence::~HNHelpScence()
{

}
bool HNHelpScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNHelpScence.xml",this);

	WidgetManager::addButtonCB("Button_BangZhu",this,button_selector(HNHelpScence::Button_BangZhu));
	WidgetFun::setEnable(this,"ButtonRule_ZhuanZhuanMj",false);
	WidgetFun::setEnable(this,"ButtonRule_ChangShaMj",true);
	WidgetFun::setVisible(this,"RuleList_ZZ",true);
	WidgetFun::setVisible(this,"RuleList_CS",false);
	return true;
}
void HNHelpScence::EnterScence()
{
}
void HNHelpScence::hideAll()
{
	WidgetFun::setVisible(this,"HelpPlane",false);
}
void HNHelpScence::Button_BangZhu(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"HelpPlane",true);
}