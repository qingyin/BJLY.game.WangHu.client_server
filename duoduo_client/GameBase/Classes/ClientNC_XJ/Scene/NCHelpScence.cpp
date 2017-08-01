#include "NCHelpScence.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"

FV_SINGLETON_STORAGE(NCHelpScence);

NCHelpScence::NCHelpScence()
{
	init();
}
NCHelpScence::~NCHelpScence()
{

}
bool NCHelpScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNHelpScence.xml",this);
	WidgetManager::addButtonCB("Button_WanFaGuiZhe",this,button_selector(NCHelpScence::Button_WanFaGuiZhe));
	
	return true;
}
void NCHelpScence::EnterScence()
{
}
void NCHelpScence::hideAll()
{
	WidgetFun::setVisible(this,"HelpPlane",false);
}
void NCHelpScence::Button_WanFaGuiZhe(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"HelpPlane",true);
}