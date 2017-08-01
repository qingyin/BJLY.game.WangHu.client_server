#include "BJLGameScence.h"
#include "Game/GameLib.h"
#include "Game/Game/QYCardTurnManager.h"
#include "Game/Widget/QYCardTurn.h"
#include "Game/Script/WindowInfo.h"

#include "CMD_Baccarat.h"

void BJLGameScence::initButton()
{
	WidgetManager::addButtonCB("BJLButton_Zhu_0",this,button_selector(BJLGameScence::BJLButton_Zhu));
	WidgetManager::addButtonCB("BJLButton_Zhu_1",this,button_selector(BJLGameScence::BJLButton_Zhu));
	WidgetManager::addButtonCB("BJLButton_Zhu_2",this,button_selector(BJLGameScence::BJLButton_Zhu));
	WidgetManager::addButtonCB("BJLButton_Zhu_3",this,button_selector(BJLGameScence::BJLButton_Zhu));

	WidgetManager::addButtonCB("BJHButton_Bet_0",this,button_selector(BJLGameScence::BJHButton_Bet));
	WidgetManager::addButtonCB("BJHButton_Bet_1",this,button_selector(BJLGameScence::BJHButton_Bet));
	WidgetManager::addButtonCB("BJHButton_Bet_2",this,button_selector(BJLGameScence::BJHButton_Bet));
	WidgetManager::addButtonCB("BJHButton_Bet_6",this,button_selector(BJLGameScence::BJHButton_Bet));
	WidgetManager::addButtonCB("BJHButton_Bet_7",this,button_selector(BJLGameScence::BJHButton_Bet));

	WidgetManager::addButtonCB("BJL_OpenCard",this,button_selector(BJLGameScence::BJL_OpenCard));
	WidgetManager::addButtonCB("BJL_SubCard",this,button_selector(BJLGameScence::BJL_SubCard));
	WidgetManager::addButtonCB("BJL_TurnCard",this,button_selector(BJLGameScence::BJL_TurnCard));

	WidgetManager::addButtonCB("BJHButton_ContorXian",this,button_selector(BJLGameScence::BJHButton_Control));
	WidgetManager::addButtonCB("BJHButton_ContorHe",this,button_selector(BJLGameScence::BJHButton_Control));
	WidgetManager::addButtonCB("BJHButton_ContorZhuang",this,button_selector(BJLGameScence::BJHButton_Control));
	WidgetManager::addButtonCB("BJHButton_ContorXianDui",this,button_selector(BJLGameScence::BJHButton_Control));
	WidgetManager::addButtonCB("BJHButton_ContorZhuangDui",this,button_selector(BJLGameScence::BJHButton_Control));

	WidgetManager::addButtonCB("BJLButton_ZhuangList",this,button_selector(BJLGameScence::BJLButton_ZhuangList));
	WidgetManager::addButtonCB("BJLButton_Shangzhuang",this,button_selector(BJLGameScence::BJLButton_Shangzhuang));
	WidgetManager::addButtonCB("BJLButton_QuXiaoZhuang",this,button_selector(BJLGameScence::BJLButton_QuXiaoZhuang));

	WidgetManager::addButtonCB("BJLButton_CancleBet",this,button_selector(BJLGameScence::BJLButton_CancleBet));
	WidgetManager::addButtonCB("BJLButton_LastBet",this,button_selector(BJLGameScence::BJLButton_LastBet));

}
void BJLGameScence::BJLButton_Zhu(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	m_iActSelectGoldButton = iIdex;
	upButtonState();
}
void BJLGameScence::BJHButton_Bet(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iBetScore = getAreaScore(m_iActSelectGoldButton);
	int iArea = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Area"));
	Command_PlaceBet(iArea,iBetScore);
	if (iArea == AREA_XIAN)
	{
		WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_XIAN),true);
		WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_ZHUANG),false);
	}
	else if(iArea == AREA_ZHUANG)
	{
		WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_ZHUANG),true);
		WidgetFun::setButtonEnabled(this,utility::toString("BJHButton_Bet_",AREA_XIAN),false);
	}
}

void BJLGameScence::BJL_OpenCard(cocos2d::Ref*,WidgetUserInfo*)
{
	autoOpenCard(0);
}
void BJLGameScence::BJHButton_Control( cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Area"));
	Command_ControlArea(iIdex);
}
void BJLGameScence::BJLButton_ZhuangList(cocos2d::Ref*,WidgetUserInfo*)
{
	SendSocketData(SUB_C_BANKER_LIST,NULL,NULL);
}
void BJLGameScence::BJLButton_Shangzhuang(cocos2d::Ref*,WidgetUserInfo*)
{
	SendSocketData(SUB_C_APPLY_BANKER,NULL,NULL);
}
void BJLGameScence::BJLButton_QuXiaoZhuang(cocos2d::Ref*,WidgetUserInfo*)
{
	SendSocketData(SUB_C_CANCEL_BANKER,NULL,NULL);
}
void BJLGameScence::BJLButton_CancleBet(cocos2d::Ref*,WidgetUserInfo*)
{
	SendSocketData(SUB_C_CANCLE_BET,NULL,NULL);
}
void BJLGameScence::BJLButton_LastBet(cocos2d::Ref*,WidgetUserInfo*)
{
	SendSocketData(SUB_C_CANCLE_BET,NULL,NULL);
	for (int i=0;i<(int)m_kLastBetList.size();i++)
	{
		Command_PlaceBet(m_kLastBetList[i].nArea,m_kLastBetList[i].nScore);
	}
}