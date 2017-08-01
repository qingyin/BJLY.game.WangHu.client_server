#include "DBHomeScence.h"
#include "DBScenceManager.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Script/ActionEx.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"

#include "../Game/BJL/BJLGameScence.h"

bool BJLRoomSort(CGameServerItem* A,CGameServerItem* B)
{
	return A->m_GameServer.lEnterScore<B->m_GameServer.lEnterScore;
}
//完成通知
void DBHomeScence::OnGameItemFinish()
{
	CServerListData* pListData = CServerListData::shared();
	{
		cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this,"Game_HallList");
		pList->removeAllChildren();
		CGameServerItemMap::iterator itor = pListData->GetServerItemMapBegin();
		CGameServerItem* pListItem = pListData->EmunGameServerItem(itor);
		cocos2d::Node* pItemRootNode = NULL;
		std::vector<CGameServerItem*> kBJLRoomList;

		while (pListItem != NULL)
		{
			CGameServerItem* pActListItem = pListItem;
			if (pActListItem->m_GameServer.wKindID != BJLGameScence::KIND_ID)
			{
				pListItem = pListData->EmunGameServerItem(itor);
				continue;
			}
			kBJLRoomList.push_back(pListItem);
			pListItem = pListData->EmunGameServerItem(itor);
		}
		std::sort(kBJLRoomList.begin(),kBJLRoomList.end(),BJLRoomSort);

		std::string kName;
		for (int i=0;i<(int)kBJLRoomList.size();i++)
		{
			if (i<=3)
			{
				kName = utility::toString("HomeHallNode",i+1);
				pItemRootNode = WidgetManager::Instance().createWidget(kName,pList);
				WidgetFun::setWidgetUserInfo(pItemRootNode,"Button_BJLHall","GameServerID",utility::toString((int)kBJLRoomList[i]->m_GameServer.wServerID));
			}
			else
			{
				break;
			}
		}
		pList->refreshView();
	}
}
//完成通知
void DBHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//更新通知
void DBHomeScence::OnGameItemUpdateFinish()
{

}
//插入通知
void DBHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//更新通知
void DBHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//删除通知
void DBHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}