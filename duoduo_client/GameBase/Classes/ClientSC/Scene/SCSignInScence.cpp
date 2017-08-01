#include "SCSignInScence.h"
#include "SCScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(SCSignInScence);

SCSignInScence::SCSignInScence()
	:m_kSignInMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kSignInMission.setMissionSink(this);
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(SCSignInScence::LogonSucess));
}
SCSignInScence::~SCSignInScence()
{

}
bool SCSignInScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCSignInScence.xml",this);
	WidgetManager::addButtonCB("Button_SignInAction",this,button_selector(SCSignInScence::Button_SignInAction));
	WidgetManager::addButtonCB("Button_ItemClick",this,button_selector(SCSignInScence::Button_ItemClick));
	
	return true;
}
void SCSignInScence::HideAll()
{
	WidgetFun::setVisible(this,"SignInPlane",false);
}
void SCSignInScence::LogonSucess()
{
	m_kSignInMission.query();
}
void SCSignInScence::Button_SignInAction(cocos2d::Ref*,WidgetUserInfo*)
{
	m_kSignInMission.done();
}
void SCSignInScence::Button_ItemClick(cocos2d::Ref*,WidgetUserInfo*)
{
	m_kSignInMission.award();
}
void SCSignInScence::onSignInQueryInfoResult(CMD_GP_CheckInInfo* pNetInfo)
{
	int iActAwardDay = 100;
	if (pNetInfo->wAwardDate < LEN_SIGIN)
	{
		 iActAwardDay = pNetInfo->lRewardDay[pNetInfo->wAwardDate];
	}
	bool bShowSign = !pNetInfo->bTodayChecked;
	WidgetFun::setText(this,"SignInDayTxt",(int)pNetInfo->wSeriesDate);
	WidgetFun::setVisible(this,"Button_SignInAction",!pNetInfo->bTodayChecked);
	WidgetFun::setVisible(this,"TodaySignIn",pNetInfo->bTodayChecked);
	
	bool bHaveAward = false;
	for (int i = 0;i<LEN_SIGIN;i++)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("SignItem",i));
		WidgetFun::setText(pNode,"ItemName",utility::toString((int)pNetInfo->lRewardDay[i],
			WidgetFun::getWidgetUserInfo(pNode,"ItemName","Txt")));
		WidgetFun::setText(pNode,"ItemGold",utility::toString((int)pNetInfo->lRewardGold[i],
			WidgetFun::getWidgetUserInfo(pNode,"ItemGold","Txt1")));
		if (i < pNetInfo->wAwardDate)
		{
			WidgetFun::setVisible(pNode,"HaveSignIn",true);
			WidgetFun::setVisible(pNode,"NextCanSignIn",false);
			WidgetFun::setVisible(pNode,"NowCanSignIn",false);
			continue;
		}
		if (bHaveAward)
		{
			WidgetFun::setVisible(pNode,"NextCanSignIn",false);
			WidgetFun::setVisible(pNode,"HaveSignIn",false);
			WidgetFun::setVisible(pNode,"NowCanSignIn",false);
			continue;
		}
		if (pNetInfo->wSeriesDate >= pNetInfo->lRewardDay[i])
		{
			WidgetFun::setVisible(pNode,"HaveSignIn",false);
			WidgetFun::setVisible(pNode,"NextCanSignIn",false);
			WidgetFun::setVisible(pNode,"NowCanSignIn",true);
			bShowSign = true;
			bHaveAward = true;
			continue;
		}
		if ((pNetInfo->wSeriesDate+1) >= pNetInfo->lRewardDay[i])
		{
			WidgetFun::setVisible(pNode,"HaveSignIn",false);
			WidgetFun::setVisible(pNode,"NextCanSignIn",true);
			WidgetFun::setVisible(pNode,"NowCanSignIn",false);
			continue;
		}
		WidgetFun::setVisible(pNode,"NextCanSignIn",false);
		WidgetFun::setVisible(pNode,"HaveSignIn",false);
		WidgetFun::setVisible(pNode,"NowCanSignIn",false);
	}
	if (!WidgetFun::isWidgetVisble(this,"SignInPlane")&&bShowSign)
	{
		WidgetFun::ActionStart(this,"SignInPlane");
	}
}
void SCSignInScence::onSignInDoneResult(CMD_GP_CheckInResult* pNetInfo)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pNetInfo->szNotifyContent));
	if (pNetInfo->bSuccessed)
	{
		m_kSignInMission.query();
		EffectScence::Instance().ShowGoldEffect();
	}
}