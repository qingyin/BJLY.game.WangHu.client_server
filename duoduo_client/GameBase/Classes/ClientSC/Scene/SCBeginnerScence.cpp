#include "SCBeginnerScence.h"
#include "SCScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(SCBeginnerScence);

SCBeginnerScence::SCBeginnerScence()
	:m_kBeginnerMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kBeginnerMission.setMissionSink(this);
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(SCBeginnerScence::LogonSucess));
}
SCBeginnerScence::~SCBeginnerScence()
{

}
bool SCBeginnerScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/SCBeginnerScence.xml",this);
	WidgetManager::addButtonCB("Button_BeginnerScence",this,button_selector(SCBeginnerScence::Button_BeginnerScence));
	WidgetManager::addButtonCB("BegiinerButton_Award",this,button_selector(SCBeginnerScence::BegiinerButton_Award));
	
	return true;
}
void SCBeginnerScence::Button_BeginnerScence(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::ActionStart(this,"BeginnerPlane");
	m_kBeginnerMission.query();
}
void SCBeginnerScence::HideAll()
{
	WidgetFun::setVisible(this,"BeginnerPlane",false);
}
void SCBeginnerScence::LogonSucess()
{
	m_kBeginnerMission.query();
}
void SCBeginnerScence::BegiinerButton_Award(cocos2d::Ref*,WidgetUserInfo*)
{
	m_kBeginnerMission.done();
}
void SCBeginnerScence::onBeginnerQueryInfoResult(CMD_GP_BeginnerInfo* pNetInfo)
{
	for (int i = 0;i<LEN_BEGINNER;i++)
	{
		int iIdex1= i/7;
		int iIdex2= i%7;
		cocos2d::Node* pItemNode = WidgetFun::getChildWidget(this,
			utility::toString("BeginnerItemList",iIdex1,":","BeginnerItem",iIdex2));
		WidgetFun::setVisible(pItemNode,false);
		if (pNetInfo->lRewardGold[i] == 0)
		{
			continue;
		}
		WidgetFun::setVisible(pItemNode,true);
		if (pNetInfo->lRewardType[i] == CMD_GP_BeginnerInfo::Type_Gold)
		{
			WidgetFun::setPlaceTextKey(pItemNode,"TxtAward","Txt1",utility::toString(pNetInfo->lRewardGold[i]));
			WidgetFun::setPlaceImagicKey(pItemNode,"ItemImagic","ImagicGold");
		}
		if (pNetInfo->lRewardType[i] == CMD_GP_BeginnerInfo::Type_Phone)
		{
			WidgetFun::setPlaceTextKey(pItemNode,"TxtAward","Txt2",utility::toString(pNetInfo->lRewardGold[i]));
			WidgetFun::setPlaceImagicKey(pItemNode,"ItemImagic","ImagicPhone");
		}
		if (pNetInfo->wSeriesDate > i)
		{
			WidgetFun::setVisible(pItemNode,"ItemAward",true);
		}
		else
		{
			WidgetFun::setVisible(pItemNode,"ItemAward",false);
		}
		WidgetFun::setPlaceTextKey(pItemNode,"DayAward","Txt",utility::toString(i+1));
	}
	for (int i = 0;i<LEN_BEGINNER;i++)
	{
		if (pNetInfo->lRewardGold[i] == 0)
		{
			continue;
		}
		if (pNetInfo->lRewardType[i] != CMD_GP_BeginnerInfo::Type_Phone)
		{
			continue;
		}
		if (pNetInfo->wSeriesDate >= (i+1))
		{
			continue;
		}

		cocos2d::Node* pItemNode = WidgetFun::getChildWidget(this,"NextBigAwardItem");
		pItemNode->setVisible(true);
		WidgetFun::setPlaceTextKey(pItemNode,"TxtAward","Txt2",utility::toString(pNetInfo->lRewardGold[i]));
		WidgetFun::setPlaceTextKey(pItemNode,"DayAward","Txt",utility::toString(i+1));
		WidgetFun::setPlaceImagicKey(pItemNode,"ItemImagic","ImagicPhone");
		break;
	}
	WidgetFun::setVisible(this,"BegiinerButton_Award",!pNetInfo->bTodayChecked);

	if (!pNetInfo->bLastCheckIned)
	{
		m_kBeginnerMission.done();
	}
}
void SCBeginnerScence::onBeginnerDoneResult(CMD_GP_BeginnerResult* pNetInfo)
{
	if (pNetInfo->bSuccessed)
	{
		m_kBeginnerMission.query();
	}
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pNetInfo->szNotifyContent));
}