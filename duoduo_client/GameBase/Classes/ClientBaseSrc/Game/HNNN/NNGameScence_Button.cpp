#include "NNGameScence.h"
#include "Game/GameLib.h"

#include "CMD_Ox.h"
#include "NNPlayer.h"
#include "NNGameLogic.h"
#include "GameLib/JniCross/JniFun.h"

void NNGameScence::initButton()
{
	WidgetManager::addButtonCB("HNMJButton_Ready",this,button_selector(NNGameScence::NCMJButton_Ready));
}
void NNGameScence::NCMJButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}