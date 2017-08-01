#include "HNHomeScence.h"
#include "HNScenceManager.h"
#include "GameLib/Game/GameLib.h"


//完成通知
void HNHomeScence::OnGameItemFinish()
{
}
//完成通知
void HNHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//更新通知
void HNHomeScence::OnGameItemUpdateFinish()
{

}
//插入通知
void HNHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//更新通知
void HNHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//删除通知
void HNHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}
void HNHomeScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}