#include "NCRouletteScence.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"

FV_SINGLETON_STORAGE(NCRouletteScence);

class SignScenceAction : public cocos2d::ActionInterval
{
public:
	SignScenceAction()
	{
		m_iDestIdex = 0;
	}
	static SignScenceAction* create(float fTime,int iIdex)
	{
		SignScenceAction *pRet = new SignScenceAction();
		pRet->initWithDuration(fTime);
		pRet->initValue(iIdex);
		pRet->autorelease();
		return pRet;
	}
	void initValue(int iIdex)
	{
		m_iDestIdex = 360*4+iIdex*360/11.f+95.0f;
	}
public:
	virtual void update(float fTime)
	{
		float fValue = sin(fTime*3.14159f/2.0f)*m_iDestIdex;
		if (getTarget())
		{
			getTarget()->setRotation(fValue);
		}
	}
private:
	float m_iDestIdex;
};

NCRouletteScence::NCRouletteScence()
	:m_kRouletteMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_bHaveDone(true)
{
	init();
	m_kRouletteMission.setMissionSink(this);
}
NCRouletteScence::~NCRouletteScence()
{

}
bool NCRouletteScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNRouletteScence.xml",this);
	WidgetManager::addButtonCB("Button_ChouJiang",this,button_selector(NCRouletteScence::Button_ShowRoulette));
	WidgetManager::addButtonCB("Button_RouletteDone",this,button_selector(NCRouletteScence::Button_RouletteDone));
	
	return true;
}
void NCRouletteScence::EnterScence()
{
}
void NCRouletteScence::upDataAction(float fTime)
{
}
void NCRouletteScence::hideAll()
{
	WidgetFun::setVisible(this,"RoulettePlane",false);
}
void NCRouletteScence::StartRouletteIdex(int iIdex)
{
}
void NCRouletteScence::Button_RouletteDone(cocos2d::Ref*,WidgetUserInfo*)
{
	if (m_bHaveDone)
	{
		NoticeMsg::Instance().ShowTopMsgByScript("RouletteHaveDone");
		return;
	}
	m_bHaveDone = true;
	m_kRouletteMission.PerformRouletteDone(UserInfo::Instance().getUserID());
}
void NCRouletteScence::Button_ShowRoulette(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	m_bHaveDone = true;
	WidgetFun::setVisible(this,"RoulettePlane",true);
	EnterScence();
	m_kRouletteMission.PerformRouletteUeserInfo(UserInfo::Instance().getUserID());
}
void NCRouletteScence::onRouletteUeserInfo(CMD_GP_BackRouletteUeserInfo* pNetInfo)
{
	m_bHaveDone = pNetInfo->dwHaveDone;
}
void NCRouletteScence::EndAction()
{
	WidgetFun::setButtonEnabled(this,"Button_RouletteDone",true);
	if (m_kRouletteDoneNetInfo.cbRewardType == 1)//½ð±Ò
	{
		NoticeMsg::Instance().ShowTopMsgByScript("RouletteTxt0",utility::toString((int)m_kRouletteDoneNetInfo.lRewardGold));
	}
	if (m_kRouletteDoneNetInfo.cbRewardType == 2)//½ð±Ò
	{
		NoticeMsg::Instance().ShowTopMsgByScript("RouletteTxt1",utility::toString((int)m_kRouletteDoneNetInfo.lRewardGold));
	}
	UserInfo::Instance().reqAccountInfo();
}
void NCRouletteScence::onRouletteDone(CMD_GP_BackRouletteDone* pNetInfo)
{
	WidgetFun::setButtonEnabled(this,"Button_RouletteDone",false);
	m_kRouletteMission.PerformRouletteUeserInfo(UserInfo::Instance().getUserID());
	WidgetFun::getChildWidget(this,"RouletteImgaic")->runAction(SignScenceAction::create(4.0f,-(int)pNetInfo->cbRewardIdex));
	SoundFun::Instance().playEffect("zhuanpan.mp3");
	m_kRouletteDoneNetInfo = *pNetInfo;
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NCRouletteScence::EndAction,this),5.0f);
}