#include "SCHomeScence.h"
#include "SCScenceManager.h"
#include "GameLib/Game/GameLib.h"


//完成通知
void SCHomeScence::OnGameItemFinish()
{
	const static int XZDD_KIND_ID = 302;


	CServerListData* pListData = CServerListData::shared();
	{
		cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this,"XZDDListRoom1");
		pList->removeAllChildren();
		CGameServerItemMap::iterator itor = pListData->GetServerItemMapBegin();
		CGameServerItem* pListItem = pListData->EmunGameServerItem(itor);
		int iIdex1 = 0;
		cocos2d::Node* pItemRootNode = NULL;
		while (pListItem != NULL)
		{
			CGameServerItem* pActListItem = pListItem;
			if (pActListItem->m_GameServer.wKindID != XZDD_KIND_ID
				|| pActListItem->m_GameServer.wServerType != GAME_GENRE_GOLD)
			{
				pListItem = pListData->EmunGameServerItem(itor);
				continue;
			}
			int iTempIdex0 = iIdex1/2;
			int iTempIdex1 = iIdex1%2;
			if (iTempIdex1 == 0)
			{
				pItemRootNode = WidgetManager::Instance().createWidget("HomeGameRoomNode",pList);
				WidgetFun::setVisible(pItemRootNode,"Room1",false);
			}
			cocos2d::Node* pItemNode = WidgetFun::getChildWidget(pItemRootNode,utility::toString("Room",iTempIdex1));
			pItemNode->setVisible(true);
			WidgetFun::setWidgetUserInfo(pItemNode,"Button_GameRoom","GameServerID",utility::toString((int)pListItem->m_GameServer.wServerID));
			WidgetFun::setText(pItemNode,"Name",utility::a_u8(pListItem->m_GameServer.szServerName));
			iIdex1++;
			pListItem = pListData->EmunGameServerItem(itor);
		}
	}
	{
		cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this,"XZDDListRoom2");
		pList->removeAllChildren();
		CGameServerItemMap::iterator itor = pListData->GetServerItemMapBegin();
		CGameServerItem* pListItem = pListData->EmunGameServerItem(itor);
		cocos2d::Node* pItemRootNode = NULL;
		while (pListItem != NULL)
		{
			CGameServerItem* pActListItem = pListItem;
			if (pActListItem->m_GameServer.wKindID != XZDD_KIND_ID
				|| pActListItem->m_GameServer.wServerType != GAME_GENRE_MATCH)
			{
				pListItem = pListData->EmunGameServerItem(itor);
				continue;
			}
			pItemRootNode = WidgetManager::Instance().createWidget("HomeMacthRoomNode",pList);
			WidgetFun::setWidgetUserInfo(pItemRootNode,"Button_ShowMatchSignup","GameServerID",utility::toString((int)pListItem->m_GameServer.wServerID));
			WidgetFun::setText(pItemRootNode,"Name",utility::a_u8(pListItem->m_GameServer.szServerName));
			WidgetFun::setPlaceTextKey(pItemRootNode,"NeedGold","Txt",utility::toString((int)pListItem->m_GameMatch.lMatchEnterScore));
			if (pListItem->m_GameMatch.lMatchFee > 0)
			{
				std::string kButtonTxt = WidgetFun::getWidgetUserInfo(pItemRootNode,"Button_ShowMatchSignup","Txt");
				utility::StringReplace(kButtonTxt,"#",utility::toString((int)pListItem->m_GameMatch.lMatchFee));
				WidgetFun::setButtonTxt(pItemRootNode,"Button_ShowMatchSignup",kButtonTxt);
			}
			if (pActListItem->m_GameMatch.cbMatchType == MATCH_TYPE_LOCKTIME)
			{
				std::string kTxtTime = utility::toString((int)pActListItem->m_GameMatch.MatchStartTime.wHour,
					":",utility::paseInt(pActListItem->m_GameMatch.MatchStartTime.wMinute,2),
					"-",utility::paseInt(pActListItem->m_GameMatch.MatchEndTime.wHour,2),
					":",utility::paseInt(pActListItem->m_GameMatch.MatchEndTime.wMinute,2));
				WidgetFun::setPlaceTextKey(pItemRootNode,"StartTxt","Txt1",kTxtTime);
			}
			if (pActListItem->m_GameMatch.cbMatchType == MATCH_TYPE_IMMEDIATE)
			{
				WidgetFun::setPlaceTextKey(pItemRootNode,"StartTxt","Txt2",utility::toString((int)pListItem->m_GameMatch.wStartUserCount));
			}
			pListItem = pListData->EmunGameServerItem(itor);
		}
	}
}
//完成通知
void SCHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//更新通知
void SCHomeScence::OnGameItemUpdateFinish()
{

}
//插入通知
void SCHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//更新通知
void SCHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//删除通知
void SCHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}
void SCHomeScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}
