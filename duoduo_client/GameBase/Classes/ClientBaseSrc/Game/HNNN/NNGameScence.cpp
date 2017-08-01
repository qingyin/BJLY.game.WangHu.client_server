#include "NNGameScence.h"
#include "NNPlayer.h"
#include "Game/GameLib.h"
#include "CMD_Ox.h"
#include "NNPlayer.h"
#include "NNGameLogic.h"
#include "NNSoundFun.h"

FV_SINGLETON_STORAGE(NNGameScence);

NNGameScence::NNGameScence()
	:m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
{

	init();
}
NNGameScence::~NNGameScence()
{

}
bool NNGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNMJ/GameScence.xml",this);

	initPrivate();

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new NNPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("NCMJButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	return true;
}
void NNGameScence::EnterScence()
{
	WidgetFun::setVisible(this,"GameHNNNPlane",true);
	if (WidgetFun::getChildWidget(this,"PrivateEndPlane"))
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	defaultState();
	NNSoundFun::playBackMusic(NNSoundFun::EX_BACKGRUAND);
}
bool NNGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameHNMJPlane");
}
void NNGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameHNNNPlane",false);
	defaultPrivateState();
}
void NNGameScence::defaultState()
{
	m_pTouchCardNode = NULL;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
	}

	defaultPrivateState();
}