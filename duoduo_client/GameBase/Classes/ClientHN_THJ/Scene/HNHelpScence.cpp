#include "HNHelpScence.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"

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
	WidgetManager::addButtonCB("Button_WanFaGuiZhe",this,button_selector(HNHelpScence::Button_WanFaGuiZhe));
	
	return true;
}
void HNHelpScence::EnterScence()
{
}
void HNHelpScence::hideAll()
{
	WidgetFun::setVisible(this,"HelpPlane",false);
}
void HNHelpScence::Button_WanFaGuiZhe(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"HelpPlane",true);
}