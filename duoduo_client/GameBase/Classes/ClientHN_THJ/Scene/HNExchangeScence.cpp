#include "HNExchangeScence.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(HNExchangeScence);


HNExchangeScence::HNExchangeScence()
	:m_kExchangeHuaFeiMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kExchangeHuaFeiMission.setMissionSink(this);
}
HNExchangeScence::~HNExchangeScence()
{

}
bool HNExchangeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCExchangeScence.xml",this);

	WidgetManager::addButtonCB("Button_DuiHuan",this,button_selector(HNExchangeScence::HNButton_ShowExchange));
	WidgetManager::addButtonCB("HNButton_ExchangeItem",this,button_selector(HNExchangeScence::HNButton_ExchangeItem));
	return true;
}
void HNExchangeScence::HNButton_ShowExchange(cocos2d::Ref*,WidgetUserInfo*)
{
	setVisible(true);
	m_kExchangeHuaFeiMission.GetExchangeList();
}
void HNExchangeScence::onGPExchangeHuaFeiListResult( CMD_GP_BackExchangeHuaFei* pNetInfo )
{
	setVisible(true);

	m_kExchangeNetInfo = *pNetInfo;

	m_kExchangeHuaFeiMission.ExchangeItem(m_kExchangeNetInfo.kExchangeID[0]);
	//cocos2d::ListViewEx* pExchangeList = WidgetFun::getListViewWidget(this,"SCExchangeList");
	//pExchangeList->removeAllChildren();

	//int nSize = pNetInfo->kExchangeID.size();

	//for (int i=0;i<nSize;i++)
	//{
	//	cocos2d::Node* pItem = WidgetManager::Instance().createWidget("SCExchangeItemNode",pExchangeList);

	//	std::string strTemp = utility::toString("ExChangeType",pNetInfo->kUseType[i]);
	//	WidgetFun::setText(pItem,"SCExchange_Titile",utility::toString(pNetInfo->kUseNum[i],ScriptData<std::string>(strTemp).Value()));

	//	strTemp = utility::toString("ExChangeType",pNetInfo->kGetType[i]);
	//	WidgetFun::setText(pItem,"SCExchange_Name",utility::toString(pNetInfo->kGetNum[i],ScriptData<std::string>(strTemp).Value()));
	//	WidgetFun::setVisible(pItem,"ItemTip",false);

	//	WidgetFun::setWidgetUserInfo(pItem,"Button_SCExchangeItem","ExchangeIndex",utility::toString(i));
	//}
}
void HNExchangeScence::onGPExchangeDoneResult(bool bSucess,const char* pChar)
{
	if (bSucess)
	{
		UserInfo::Instance().reqAccountInfo();
	}
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pChar));
}

void HNExchangeScence::HNButton_ExchangeItem( cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	int nExchangeIndex = utility::parseInt(WidgetFun::getUserInfoValue(pInfo,"ExchangeIndex"));
	m_kExchangeHuaFeiMission.ExchangeItem(m_kExchangeNetInfo.kExchangeID[nExchangeIndex]);
}
