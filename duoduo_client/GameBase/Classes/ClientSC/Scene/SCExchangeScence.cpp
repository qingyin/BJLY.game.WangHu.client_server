#include "SCExchangeScence.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Script/ActionEx.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"

#include "Game/GameLib.h"
#include "GameLib/Tools/core/MD5.h"

#include "SCScenceManager.h"

FV_SINGLETON_STORAGE(SCExchangeScence);

#define  SC_EXCHANGE_URL  "http://www.bosengame.com:8088/BoSenWebServer.aspx?"
#define  SC_EXCHANGE_KEY  "89b5b987124d2ec3"

SCExchangeScence::SCExchangeScence()
	:m_kExchangeHuaFeiMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kExchangeHuaFeiMission.setMissionSink(this);
}
SCExchangeScence::~SCExchangeScence()
{

}
bool SCExchangeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCExchangeScence.xml",this);

	WidgetManager::addButtonCB("Button_SCExchangeItem",this,button_selector(SCExchangeScence::Button_SCExchangeItem));
	return true;
}
void SCExchangeScence::EnterScence()
{
	m_kExchangeHuaFeiMission.GetExchangeList();
}

void SCExchangeScence::onGPExchangeHuaFeiListResult( CMD_GP_BackExchangeHuaFei* pNetInfo )
{
	setVisible(true);

	m_kExchangeNetInfo = *pNetInfo;

	cocos2d::ListViewEx* pExchangeList = WidgetFun::getListViewWidget(this,"SCExchangeList");
	pExchangeList->removeAllChildren();

	int nSize = pNetInfo->kExchangeID.size();

	for (int i=0;i<nSize;i++)
	{
		cocos2d::Node* pItem = WidgetManager::Instance().createWidget("SCExchangeItemNode",pExchangeList);

		std::string strTemp = utility::toString("ExChangeType",pNetInfo->kUseType[i]);
		WidgetFun::setText(pItem,"SCExchange_Titile",utility::toString(pNetInfo->kUseNum[i],ScriptData<std::string>(strTemp).Value()));

		strTemp = utility::toString("ExChangeType",pNetInfo->kGetType[i]);
		WidgetFun::setText(pItem,"SCExchange_Name",utility::toString(pNetInfo->kGetNum[i],ScriptData<std::string>(strTemp).Value()));
		WidgetFun::setVisible(pItem,"ItemTip",false);

		WidgetFun::setWidgetUserInfo(pItem,"Button_SCExchangeItem","ExchangeIndex",utility::toString(i));
	}
}

void SCExchangeScence::Button_SCExchangeItem( cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	int nExchangeIndex = utility::parseInt(WidgetFun::getUserInfoValue(pInfo,"ExchangeIndex"));
	Req_Exchange(nExchangeIndex);
}

void SCExchangeScence::Req_Exchange(int nExchangeIndex)
{
	if (m_kExchangeNetInfo.kExchangeID.size()<=nExchangeIndex)
	{
		ASSERT(false);
		return;
	}
	std::string kType = "A1";
	std::string kAccount = UserInfo::Instance().getAccounts();
	int nCardNum = m_kExchangeNetInfo.kGetNum[nExchangeIndex];
	std::string kPhoneNo = UserInfo::Instance().getPhoneNumber();
	std::string kOrderId = utility::toString(m_kExchangeNetInfo.kExchangeID[nExchangeIndex]);

	std::string kSign = md5(kAccount+kPhoneNo+kOrderId+SC_EXCHANGE_KEY);

	std::string kUrl = SC_EXCHANGE_URL;

	MCWebReq::instance().pushValue(kUrl,"Type",kType);
	MCWebReq::instance().pushValue(kUrl,"Account",kAccount);
	MCWebReq::instance().pushValue(kUrl,"CardNum",nCardNum);
	MCWebReq::instance().pushValue(kUrl,"PhoneNo",kPhoneNo);
	MCWebReq::instance().pushValue(kUrl,"OrderId",kOrderId);
	MCWebReq::instance().pushValue(kUrl,"Sign",kSign);

	MCWebReq::instance().sendRequestDocumentUrl(kUrl,CC_CALLBACK_1(SCExchangeScence::Rsp_Exchange,this),nullptr);
}

void SCExchangeScence::Rsp_Exchange(rapidjson::Document* pDoc)
{
	std::string kRet;
	std::string kMsg;

	if (pDoc->IsObject())
	{
		auto item = pDoc->MemberBegin();
		auto iend = pDoc->MemberEnd();
		int _id = 0;

		for (;item!=iend;item++)
		{
			auto data = &((*item).value);
			
			if (data->IsObject())
			{
				kRet = MCWebReq::getDataValueStr(data,"Status");
				kMsg = MCWebReq::getDataValueStr(data,"Msg");
			}
			
		}
	}
}
