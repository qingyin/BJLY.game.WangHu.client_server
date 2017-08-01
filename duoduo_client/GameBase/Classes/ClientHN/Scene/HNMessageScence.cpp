#include "HNMessageScence.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(HNMessageScence);

HNMessageScence::HNMessageScence()
{
	init();
}
HNMessageScence::~HNMessageScence()
{

}
bool HNMessageScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNMessageScence.xml",this);

	WidgetManager::addButtonCB("Button_Message",this,button_selector(HNMessageScence::Button_Message));
	return true;
}
void HNMessageScence::EnterScence()
{
}
void HNMessageScence::hideAll()
{
	WidgetFun::setVisible(this,"MessagePlane",false);
}
void HNMessageScence::Button_Message(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"MessagePlane",true);
}