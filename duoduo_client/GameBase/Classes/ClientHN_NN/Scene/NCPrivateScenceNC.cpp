#include "NCPrivateScenceNC.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "kernel/kernel/server/CServerItem.h"
#include "GameLib/Game/Game/NIUNIU/CMD_Ox.h"
#include "GameLib/Game/Game/NIUNIU/NNGameScence.h"

FV_SINGLETON_STORAGE(NCPrivateScenceNC);

NCPrivateScenceNC::NCPrivateScenceNC()
	:m_iPlayCoutIdex(0)
	,m_dwPlayRule(0)
{
	init();
}
NCPrivateScenceNC::~NCPrivateScenceNC()
{

}
bool NCPrivateScenceNC::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/HNPrivateScenceHN.xml",this);

	WidgetManager::addButtonCB("Button_JoinRoom",this,button_selector(NCPrivateScenceNC::Button_JoinNumReset));
	WidgetManager::addButtonCB("Button_JoinNum",this,button_selector(NCPrivateScenceNC::Button_JoinNum));
	WidgetManager::addButtonCB("Button_JoinNumDel",this,button_selector(NCPrivateScenceNC::Button_JoinNumDel));
	WidgetManager::addButtonCB("Button_JoinNumReset",this,button_selector(NCPrivateScenceNC::Button_JoinNumReset));
	
	WidgetManager::addButtonCB("Button_Show_Create_Private",this,button_selector(NCPrivateScenceNC::Button_Show_Create_Private));
	WidgetManager::addButtonCB("Button_Show_Create_Public",this,button_selector(NCPrivateScenceNC::Button_Show_Create_Public));
	WidgetManager::addButtonCB("Button_Show_Join_Private",this,button_selector(NCPrivateScenceNC::Button_Show_Join_Private));
	WidgetManager::addButtonCB("Button_CreateRoom",this,button_selector(NCPrivateScenceNC::Button_CreateRoom));
	WidgetManager::addButtonCB("Button_JoinPublic",this,button_selector(NCPrivateScenceNC::Button_JoinPublic));
	WidgetManager::addButtonCB("Button_DismissPrivate",this,button_selector(NCPrivateScenceNC::Button_DismissPrivate));
	WidgetManager::addButtonCB("Button_DismissPrivateNot",this,button_selector(NCPrivateScenceNC::Button_DismissPrivateNot));
	WidgetManager::addButtonCB("Button_PrivateAgagin",this,button_selector(NCPrivateScenceNC::Button_PrivateAgagin));

	WidgetManager::addButtonCB("Button_GameCout0",this,button_selector(NCPrivateScenceNC::Button_PrivatePlayCout));
	WidgetManager::addButtonCB("Button_GameCout1",this,button_selector(NCPrivateScenceNC::Button_PrivatePlayCout));

	WidgetManager::addButtonCB("Button_GameRule0",this,button_selector(NCPrivateScenceNC::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule1",this,button_selector(NCPrivateScenceNC::ButtonGameRule));
	
	setPlayerCoutIdex(0);
	setGameRuleIdex(GAME_RULE_SUIJIZHUANG);
	return true;
}
void NCPrivateScenceNC::hideAll()
{
	WidgetFun::setVisible(this,"CreateRoomPlane",false);
	WidgetFun::setVisible(this,"JoinRoomPlane",false);
}
void NCPrivateScenceNC::setPlayerCoutIdex(int iIdex)
{
	for (int i = 0;i<2;i++)
	{
		WidgetFun::setChecked(this,utility::toString("Button_GameCout",i),iIdex == i);
	}
	m_iPlayCoutIdex = iIdex;
}
void NCPrivateScenceNC::setGameRuleIdex(int iIdex)
{
	m_dwPlayRule = iIdex;
	
	WidgetFun::setChecked(this,"Button_GameRule0",iIdex == GAME_RULE_SUIJIZHUANG);
	WidgetFun::setChecked(this,"Button_GameRule1",iIdex == GAME_RULE_QIANGZHUANG);

}
void NCPrivateScenceNC::Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	UserInfo::Instance().checkInGameServer();
	WidgetFun::setVisible(this,"PlaneCreate",true);
	WidgetFun::setVisible(this,"PlanePublic",false);
}
void NCPrivateScenceNC::Button_Show_Create_Public(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	UserInfo::Instance().checkInGameServer();
	WidgetFun::setVisible(this,"PlaneCreate",false);
	WidgetFun::setVisible(this,"PlanePublic",true);
}
void NCPrivateScenceNC::Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	WidgetFun::setVisible(this,"JoinRoomPlane",true);
	Button_JoinNumReset(NULL,NULL);
	UserInfo::Instance().checkInGameServer();
}

void NCPrivateScenceNC::Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Again_Private kNetInfo;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_RIVATE_AGAIN,&kNetInfo,sizeof(kNetInfo));
}
void NCPrivateScenceNC::Button_CreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet,sizeof(kSendNet));
	kSendNet.cbGameType = Type_Private;
	kSendNet.bGameRuleIdex = m_dwPlayRule;
//	kSendNet.bGameTypeIdex = GAME_TYPE_XJ;
	kSendNet.bPlayCoutIdex = m_iPlayCoutIdex;
	ConnectAndCreatePrivateByKindID(NNGameScence::KIND_ID,kSendNet);
}
void NCPrivateScenceNC::Button_JoinPublic(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet,sizeof(kSendNet));
	kSendNet.cbGameType = Type_Public;
	kSendNet.bGameRuleIdex = m_dwPlayRule;
//	kSendNet.bGameTypeIdex = GAME_TYPE_XJ;
	kSendNet.bPlayCoutIdex = m_iPlayCoutIdex;
	ConnectAndCreatePrivateByKindID(NNGameScence::KIND_ID,kSendNet);
}
void NCPrivateScenceNC::Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setPlayerCoutIdex(iIdex);
}
void NCPrivateScenceNC::ButtonGameRule(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setGameRuleIdex(iIdex);
}