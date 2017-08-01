#include "CarGameScence.h"
#include "Game/GameLib.h"

void CarGameScence::initButton()
{
	WidgetManager::addButtonCB("CarBtn_Bet1",this,button_selector(CarGameScence::CarBtn_Bet1));
	WidgetManager::addButtonCB("CarBtn_Bet2",this,button_selector(CarGameScence::CarBtn_Bet2));
	WidgetManager::addButtonCB("CarBtn_Bet3",this,button_selector(CarGameScence::CarBtn_Bet3));
	WidgetManager::addButtonCB("CarBtn_Bet4",this,button_selector(CarGameScence::CarBtn_Bet4));
	WidgetManager::addButtonCB("CarBtn_Bet5",this,button_selector(CarGameScence::CarBtn_Bet5));
	WidgetManager::addButtonCB("CarBtn_Bet6",this,button_selector(CarGameScence::CarBtn_Bet6));
	WidgetManager::addButtonCB("CarBtn_Bet7",this,button_selector(CarGameScence::CarBtn_Bet7));
	WidgetManager::addButtonCB("CarBtn_Bet8",this,button_selector(CarGameScence::CarBtn_Bet8));

	WidgetManager::addButtonCB("CarBtn_Switch",this,button_selector(CarGameScence::CarBtn_Switch));
	WidgetManager::addButtonCB("CarBtn_ReBet",this,button_selector(CarGameScence::CarBtn_ReBet));
	WidgetManager::addButtonCB("CarBtn_AutoBet",this,button_selector(CarGameScence::CarBtn_AutoBet));

	WidgetManager::addButtonCB("CarBtn_Return",this,button_selector(CarGameScence::Button_ExitGameBase));
}

void CarGameScence::CarBtn_Bet1( cocos2d::Ref*,WidgetUserInfo* pUserInfo )
{
	Command_UserBet(1,m_nUnitBet);
}

void CarGameScence::CarBtn_Bet2(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	Command_UserBet(2,m_nUnitBet);
}
void CarGameScence::CarBtn_Bet3(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	Command_UserBet(3,m_nUnitBet);
}
void CarGameScence::CarBtn_Bet4(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	Command_UserBet(4,m_nUnitBet);
}
void CarGameScence::CarBtn_Bet5(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	Command_UserBet(5,m_nUnitBet);
}
void CarGameScence::CarBtn_Bet6(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	Command_UserBet(6,m_nUnitBet);
}
void CarGameScence::CarBtn_Bet7(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	Command_UserBet(7,m_nUnitBet);
}
void CarGameScence::CarBtn_Bet8(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	Command_UserBet(8,m_nUnitBet);
}

void CarGameScence::CarBtn_Switch( cocos2d::Ref*,WidgetUserInfo* pUserInfo )
{
	static int i=0;
	i++;
	m_nUnitBet = UNIT_BET[i%4];
	i = i%4;
	setUnitBet(m_nUnitBet);
}
void CarGameScence::CarBtn_ReBet(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	atuoBet();
}
void CarGameScence::CarBtn_AutoBet(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{

}