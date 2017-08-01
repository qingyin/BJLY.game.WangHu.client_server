#include "SCMatchScence.h"
#include "SCScenceManager.h"
#include "Game/GameLib.h"
#include "../SCGameManager.h"
#include "kernel/kernel/server/CServerItem.h"

FV_SINGLETON_STORAGE(SCMatchScence);

SCMatchScence::SCMatchScence()
	:m_kMactchMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_iActServerID(0)
{
	init();
	m_kMactchMission.setMissionSink(this);
	CServerItem::get()->SetServerMatchSink(this);
}
SCMatchScence::~SCMatchScence()
{

}
void SCMatchScence::sendPayMatchCost(cocos2d::Ref*,WidgetUserInfo*)
{
	CServerListData* pListData = CServerListData::shared();
	CGameServerItem* pGameServer = pListData->SearchGameServer(m_iActServerID);
	if (!pGameServer)
	{
		return;
	}
	CCAssert(pGameServer->m_GameServer.wServerType == GAME_GENRE_MATCH,"");
	if (pGameServer->m_GameServer.wServerType != GAME_GENRE_MATCH)
	{
		return;
	}
	if (pGameServer->m_GameMatch.cbMatchType == MATCH_TYPE_LOCKTIME)
	{
		m_kMactchMission.PerformSignupMatch(pGameServer->m_GameServer.wServerID);
	}
	if (pGameServer->m_GameMatch.cbMatchType == MATCH_TYPE_IMMEDIATE)
	{;
		SCGameManager::Instance().connectGameServerByServerID(m_iActServerID);
	}
}
bool SCMatchScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/SCMatchScence.xml",this);

	WidgetManager::addButtonCB("Button_ExitImmediateMatch",this,button_selector(SCMatchScence::Button_ExitImmediateMatch));
	WidgetManager::addButtonCB("Button_ExitWaiteMatch",this,button_selector(SCMatchScence::Button_ExitWaiteMatch));
	WidgetManager::addButtonCB("Button_ExitMatchResoult",this,button_selector(SCMatchScence::Button_ExitMatchResoult));
	WidgetManager::addButtonCB("Button_ActSignupMatch",this,button_selector(SCMatchScence::Button_ActSignupMatch));
	return true;
}
void SCMatchScence::hideAll()
{
	WidgetFun::setVisible(this,"MatchWaitePeoplePlane",false);
	WidgetFun::setVisible(this,"MatchPopInfoPlane",false);
	WidgetFun::setVisible(this,"MatchWaitePlane",false);
	WidgetFun::setVisible(this,"MatchResoultPlane",false);
}
void SCMatchScence::reqMatch(int iServerID)
{
	m_iActServerID = iServerID;
	CServerListData* pListData = CServerListData::shared();
	CGameServerItem* pGameServer = pListData->SearchGameServer(iServerID);
	CCAssert(pGameServer,"");
	if (!pGameServer)
	{
		return;
	}
	CCAssert(pGameServer->m_GameServer.wServerType == GAME_GENRE_MATCH,"");
	if (pGameServer->m_GameServer.wServerType != GAME_GENRE_MATCH)
	{
		return;
	}
	if (pGameServer->m_bSignuped)
	{
		SCGameManager::Instance().connectGameServerByServerID(m_iActServerID);
		return;
	}
	if (pGameServer->m_GameMatch.lMatchFee != 0)
	{
		PopScence::Instance().showAccessPlane(utility::getScriptReplaceValue("Match_Free1",(int)pGameServer->m_GameMatch.lMatchFee),
			this,button_selector(SCMatchScence::sendPayMatchCost));
	}
	else
	{
		sendPayMatchCost(NULL,NULL);
	}
}
void SCMatchScence::ShowSignupMatch(int iServerID)
{
	setVisible(true);
	m_iActServerID = iServerID;
	CServerListData* pListData = CServerListData::shared();
	CGameServerItem* pGameServer = pListData->SearchGameServer(iServerID);
	CCAssert(pGameServer,"");
	if (!pGameServer)
	{
		return;
	}
	m_kMactchMission.PerformMatchAward(iServerID);
	WidgetFun::setVisible(this,"MatchInfoPlane",true);
	WidgetFun::setText(this,"Info0Name",utility::a_u8(pGameServer->m_GameServer.szServerName));
	WidgetFun::setPlaceTextKey(this,"Info0Need","Txt",utility::toString((int)pGameServer->m_GameMatch.lMatchEnterScore));
	WidgetFun::setText(this,"Info0Cost",utility::a_u8(pGameServer->m_GameServer.szServerName));
	if (pGameServer->m_GameMatch.lMatchFee > 0)
	{
		WidgetFun::setPlaceTextKey(this,"Info0Cost","Txt2",utility::toString(pGameServer->m_GameMatch.lMatchFee));
	}
	else
	{
		WidgetFun::setPlaceTextKey(this,"Info0Cost","Txt1",utility::toString(pGameServer->m_GameMatch.lMatchFee));
	}
	if (pGameServer->m_GameMatch.cbMatchType == MATCH_TYPE_LOCKTIME)
	{
		std::string kTxtTime = utility::toString((int)pGameServer->m_GameMatch.MatchStartTime.wHour,
			":",utility::paseInt(pGameServer->m_GameMatch.MatchStartTime.wMinute,2),
			"-",utility::paseInt(pGameServer->m_GameMatch.MatchEndTime.wHour,2),
			":",utility::paseInt(pGameServer->m_GameMatch.MatchEndTime.wMinute,2));
		WidgetFun::setPlaceTextKey(this,"Info0Rule","Txt1",kTxtTime);
	}
	if (pGameServer->m_GameMatch.cbMatchType == MATCH_TYPE_IMMEDIATE)
	{
		WidgetFun::setPlaceTextKey(this,"Info0Rule","Txt2",utility::toString((int)pGameServer->m_GameMatch.wStartUserCount));
	}
	WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,"MacthButton0"));
}
//时间比赛报名结果
void SCMatchScence::onSignUpResoult(tagGameMatch&	pGameMatchInfo,bool bSucess,bool bSignUp,const char* pStr )
{
	if (bSucess && bSignUp)
	{
		hideAll();
		setVisible(true);
		SCGameManager::Instance().connectGameServerByServerID(m_iActServerID);
	}
	if (!bSucess)
	{
		NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
	}
	if (bSucess && !bSignUp)
	{
		GameManagerBase::InstanceBase().disconnectServer();
	}
	UserInfo::Instance().reqAccountInfo();
}
void SCMatchScence::onMatchAwardList(CMD_GR_MatchAwardList* pList)
{
	for (int i = 0;i<6;i++)
	{
		WidgetFun::setVisible(this,utility::toString("AwardInfoRank",i),false);
	}
	for (int i = 0;i<(int)pList->kAwards.size();i++)
	{
		WidgetFun::setVisible(this,utility::toString("AwardInfoRank",i),true);
		tagMatchAwardkInfo& kAwardInfo = pList->kAwards[i];
		cocos2d::Node* pNode =WidgetFun::getChildWidget(this,utility::toString("AwardInfoRank",i));
		WidgetFun::setPlaceTextKey(pNode,"Txt",utility::toString(i+1));
		WidgetFun::setPlaceTextKey(pNode,"AwardInfo","Txt",utility::toString((int)kAwardInfo.RewardGold));
	}
}
void SCMatchScence::OnSocketSubMatchFee(CMD_GR_Match_Fee* pNetInfo)
{
	hideAll();
	setVisible(true);
	((CServerItem*)CServerItem::get())->sendMacthFree();
}
void SCMatchScence::OnSocketSubMatchNum(CMD_GR_Match_Num* pNetInfo) 
{
	CServerListData* pListData = CServerListData::shared();
	CGameServerItem* pGameServer = pListData->SearchGameServer(m_iActServerID);
	if (!pGameServer)
	{
		return;
	}
	if (pGameServer->m_GameMatch.cbMatchType == MATCH_TYPE_LOCKTIME)
	{
		WidgetFun::setPlaceTextKey2(this,"WaiteTxt","Txt2",utility::toString((int)pNetInfo->dwWaitting),utility::toString((int)pNetInfo->dwTotal));
	}
	else
	{
		WidgetFun::setPlaceTextKey2(this,"WaiteTxt","Txt1",utility::toString((int)pNetInfo->dwWaitting),utility::toString((int)pNetInfo->dwTotal));
	}
}
void SCMatchScence::OnSocketSubMatchInfo(CMD_GR_Match_Info* pNetInfo)
{
	hideAll();
	setVisible(true);
	WidgetFun::ActionStart(this,"MatchPopInfoPlane");
	for (int i = 0;i<4;i++)
	{
		WidgetFun::setText(this,utility::toString("MatchPopInfoTxt",i),utility::a_u8(pNetInfo->szTitle[i]));
	}
}
void SCMatchScence::OnSocketSubMatchWaitTip(bool bShow,CMD_GR_Match_Wait_Tip* pNetInfo)
{
	hideAll();
	if (bShow)
	{
		WidgetFun::ActionStart(this,"MatchWaitePlane");
		setVisible(true);
	}
	if (pNetInfo)
	{
		WidgetFun::setText(this,"Name",utility::a_u8(pNetInfo->szMatchName));
		WidgetFun::setPlaceTextKey2(this,"MatchWaiteTxt0","Txt",utility::toString((int)pNetInfo->lScore),utility::toString((int)pNetInfo->wRank));
		WidgetFun::setPlaceTextKey(this,"MatchWaiteTxt1","Txt",utility::toString((int)pNetInfo->wPlayingTable));
		WidgetFun::setPlaceTextKey2(this,"MatchWaiteTxt2","Txt",utility::toString((int)pNetInfo->wCurGameCount,"/",(int)pNetInfo->wGameCount)
			,utility::toString((int)pNetInfo->wCurTableRank));
	}
}
void SCMatchScence::OnSocketSubMatchResult(CMD_GR_MatchResult* pNetInfo)
{
	setVisible(true);
	hideAll();
	WidgetFun::setVisible(this,"MatchResoultPlane",true);
	WidgetFun::setText(this,"MatchResoult0",utility::a_u8(pNetInfo->szDescribe));
	WidgetFun::setPlaceTextKey(this,"MatchResoultTxt1","Txt",utility::toString((int)pNetInfo->lGold));

}
void SCMatchScence::OnSocketSubMatchStatus(byte pNetInfo)
{
	if (pNetInfo == MS_MATCHING)
	{
		setVisible(false);
	}
}
void SCMatchScence::OnSocketSubMatchGoldUpdate(CMD_GR_MatchGoldUpdate* pNetInfo)
{

}
void SCMatchScence::OnSocketSubMatchEliminate()
{

}
void SCMatchScence::OnSocketSubMatchJoinResolt(bool bSucess)
{
	CServerListData* pListData = CServerListData::shared();
	CGameServerItem* pGameServer = pListData->SearchGameServer(m_iActServerID);
	if (!pGameServer)
	{
		return;
	}
	setVisible(false);
	hideAll();
	if (!bSucess)
	{
		if (pGameServer->m_GameMatch.cbMatchType == MATCH_TYPE_LOCKTIME)
		{
			m_kMactchMission.PerformUnSignupMatch(m_iActServerID);
		}
		return;
	}
	setVisible(true);
	WidgetFun::setVisible(this,"MatchWaitePeoplePlane",true);
}
void SCMatchScence::Button_ExitImmediateMatch(cocos2d::Ref*,WidgetUserInfo*)
{
	UserInfo::Instance().reqAccountInfo(0.5f);
	((CServerItem*)CServerItem::get())->sendExitMacth();
}
void SCMatchScence::Button_ExitWaiteMatch(cocos2d::Ref*,WidgetUserInfo*)
{
	m_kMactchMission.PerformUnSignupMatch(m_iActServerID);
}
void SCMatchScence::Button_ExitMatchResoult(cocos2d::Ref*,WidgetUserInfo*)
{
	GameManagerBase::InstanceBase().disconnectServer();
}
void SCMatchScence::Button_ActSignupMatch(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"MatchInfoPlane",false);
	reqMatch(m_iActServerID);
}