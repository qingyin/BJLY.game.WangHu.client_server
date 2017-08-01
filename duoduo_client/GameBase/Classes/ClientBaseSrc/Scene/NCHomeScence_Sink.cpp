#include "NCHomeScence.h"
#include "NCScenceManager.h"
#include "GameLib/Game/GameLib.h"


//完成通知
void NCHomeScence::OnGameItemFinish()
{
}
//完成通知
void NCHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//更新通知
void NCHomeScence::OnGameItemUpdateFinish()
{

}
//插入通知
void NCHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//更新通知
void NCHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//删除通知
void NCHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}
void NCHomeScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}