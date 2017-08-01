#include "LPGameScence.h"
#include "Game/GameLib.h"
#include "CMD_Land.h"
#include "LPYaCell.h"
#include "GameLib/JniCross/JniFun.h"

void LPGameScence::initButton()
{
	WidgetManager::addButtonCB("LPButton_Zhu_0",this,button_selector(LPGameScence::BJLButton_Zhu));
	WidgetManager::addButtonCB("LPButton_Zhu_1",this,button_selector(LPGameScence::BJLButton_Zhu));
	WidgetManager::addButtonCB("LPButton_Zhu_2",this,button_selector(LPGameScence::BJLButton_Zhu));
	WidgetManager::addButtonCB("LPButton_Zhu_3",this,button_selector(LPGameScence::BJLButton_Zhu));
	WidgetManager::addButtonCB("LPButton_Zhu_4",this,button_selector(LPGameScence::BJLButton_Zhu));

	WidgetManager::addButtonCB("LPButton_Return",this,button_selector(GameBase::Button_ExitGameBase));

	WidgetManager::addButtonCB("LPButton_History",this,button_selector(LPGameScence::LPButton_History));
	WidgetManager::addButtonCB("LPButton_CloseHistory",this,button_selector(LPGameScence::LPButton_CloseHistory));
	WidgetManager::addButtonCB("LPButton_LastBet",this,button_selector(LPGameScence::LPButton_LastBet));

	WidgetManager::addButtonCB("LPButton_ResetAll",this,button_selector(LPGameScence::LPButton_ResetAll));
	WidgetManager::addButtonCB("LPButton_Reset",this,button_selector(LPGameScence::LPButton_Reset));
	WidgetManager::addButtonCB("LPButton_Rule",this,button_selector(LPGameScence::LPButton_Rule));
}
void LPGameScence::BJLButton_Zhu(cocos2d::Ref* pRef,WidgetUserInfo* pUserInfo)
{
	m_nChipsType = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	
	WidgetFun::setPos(this,"LPChipsBtn_Effect",((Node*)pRef)->getPosition());
}

void LPGameScence::LPButton_History(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"LPHistoryBg",true);
	updateHistotry();
}
void LPGameScence::LPButton_CloseHistory(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"LPHistoryBg",false);
}
void LPGameScence::LPButton_LastBet(cocos2d::Ref*,WidgetUserInfo*)
{
	if (m_kLastBetList.empty())
	{
		WidgetFun::runWidgetAction(this,"LPTip_NoBet","TipMove");
		return;
	}
	Command_CancleBet(LP_USER_CANCEL_LAST);
	
//	m_kLastBetList.clear();
//	WidgetFun::setButtonEnabled(this,"LPButton_LastBet",false);
}

void LPGameScence::LPButton_ResetAll(cocos2d::Ref*,WidgetUserInfo*)
{
	Command_CancleBet(LP_USER_CANCEL_ALL);
}
void LPGameScence::LPButton_Reset(cocos2d::Ref*,WidgetUserInfo*)
{
	Command_CancleBet(LP_USER_CANCEL_ONCE);
}
void LPGameScence::LPButton_Rule( cocos2d::Ref*,WidgetUserInfo* )
{
	JniFun::showWebView("http://static.111ydcasino.com/macau/web/rw-help.html");
}
