#include "SCShopScence.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Script/ActionEx.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"

#include "Game/GameLib.h"

#include "SCScenceManager.h"

FV_SINGLETON_STORAGE(SCShopScence);

SCShopScence::SCShopScence()
	:m_kShopInfoMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kShopInfoMission.setMissionSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(SCShopScence::upPlayerInfo));
}
SCShopScence::~SCShopScence()
{

}
bool SCShopScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCShopScence.xml",this);
	WidgetManager::addButtonCB("Button_SCShopItem",this,button_selector(SCShopScence::Button_SCShopItem));
	return true;
}
void SCShopScence::EnterScence()
{
	m_kShopInfoMission.GetShopInfoList();
}

void SCShopScence::onGPShopInfoListResult(CMD_GP_BackShopInfo* pNetInfo)
{
	setVisible(true);

	cocos2d::ListViewEx* pShopList = WidgetFun::getListViewWidget(this,"SCShopList");
	pShopList->removeAllChildren();

	int nSize = pNetInfo->kItemID.size();

	for (int i=0;i<nSize;i++)
	{
		cocos2d::Node* pItem = WidgetManager::Instance().createWidget("SCShopItemNode",pShopList);

		WidgetFun::setText(pItem,"SCShop_Titile",utility::a_u8(pNetInfo->kItemTitle[i]));
		WidgetFun::setText(pItem,"SCShop_GoldNum",utility::a_u8(pNetInfo->kItemName[i]));
		WidgetFun::setVisible(pItem,"ItemTip",false);

		WidgetFun::setWidgetUserInfo(pItem,"Button_SCShopItem","ShopID",utility::toString(pNetInfo->kItemID[i]));
	}
	pShopList->refreshView();
}

void SCShopScence::Button_SCShopItem( cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	int nShopID = utility::parseInt(WidgetFun::getUserInfoValue(pInfo,"ShopID"));
}

void SCShopScence::upPlayerInfo()
{
	WidgetFun::setText(this,"SCShopUserGold",UserInfo::Instance().getUserScore());
}
