#include "DBShopScence.h"
#include "DBScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(DBShopScence);

DBShopScence::DBShopScence()
{
	init();
}
DBShopScence::~DBShopScence()
{

}
bool DBShopScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/DBShopScence.xml",this);

	WidgetManager::addButtonCB("Button_Rank",this,button_selector(DBShopScence::Button_Shop));
	return true;
}
void DBShopScence::Button_Shop( cocos2d::Ref*,WidgetUserInfo* )
{
	WidgetFun::ActionStart(this,"ShopPlane");
}