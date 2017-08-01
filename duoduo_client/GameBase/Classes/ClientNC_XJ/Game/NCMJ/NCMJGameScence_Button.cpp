#include "NCMJGameScence.h"
#include "Game/GameLib.h"

#include "CMD_NCMJ.h"
#include "NCMJPlayer.h"
#include "NCMJGameLogic.h"
#include "GameLib/JniCross/JniFun.h"

void NCMJGameScence::initButton()
{
	WidgetManager::addButtonCB("HNMJButton_Ready",this,button_selector(NCMJGameScence::NCMJButton_Ready));
	WidgetManager::addButtonCB("HNMJButton_GuoAction",this,button_selector(NCMJGameScence::NCMJButton_GuoAction));
	WidgetManager::addButtonCB("HNMJButton_GangAction",this,button_selector(NCMJGameScence::NCMJButton_GangAction));
	WidgetManager::addButtonCB("HNMJButton_PengAction",this,button_selector(NCMJGameScence::NCMJButton_PengAction));
	WidgetManager::addButtonCB("HNMJButton_ChiAction",this,button_selector(NCMJGameScence::NCMJButton_ChiAction));
	WidgetManager::addButtonCB("HNMJButton_HuAction",this,button_selector(NCMJGameScence::NCMJButton_HuAction));
	WidgetManager::addButtonCB("HNMJButton_XiaoHuAction",this,button_selector(NCMJGameScence::NCMJButton_XiaoHuAction));

	WidgetManager::addButtonCB("HNMJButtonAction_ShowCard",this,button_selector(NCMJGameScence::NCMJButtonAction_ShowCard));
	WidgetManager::addButtonCB("HNMJButtonAction_ShowNext",this,button_selector(NCMJGameScence::NCMJButtonAction_ShowNext));

	WidgetManager::addButtonCB("HNMJButton_JieSuanShare",this,button_selector(NCMJGameScence::NCMJButton_JieSuanShare));
	WidgetManager::addButtonCB("HNMJButton_JieSuanStart",this,button_selector(NCMJGameScence::NCMJButton_JieSuanStart));

	WidgetManager::addButtonCBBegin("HNMJButton_Talk",this,button_selector(NCMJGameScence::NCMJButton_TalkBegin));
	WidgetManager::addButtonCB("HNMJButton_Talk",this,button_selector(NCMJGameScence::NCMJButton_TalkEnd));
}
void NCMJGameScence::NCMJButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}
void NCMJGameScence::NCMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_NULL;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void NCMJGameScence::NCMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_GANG;
	OperateCard.cbOperateCard = m_pLocal->getGangCard(utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard")));
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void NCMJGameScence::NCMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_PENG;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
void NCMJGameScence::NCMJButton_ChiAction(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	BYTE nChiAction  = (BYTE)utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Chi_Type"));
	CMD_C_OperateCard OperateCard;
	if (nChiAction & WIK_LEFT)
	{
		OperateCard.cbOperateCode=WIK_LEFT;
	}
	if (nChiAction & WIK_CENTER)
	{
		OperateCard.cbOperateCode=WIK_CENTER;
	}
	if (nChiAction & WIK_RIGHT)
	{
		OperateCard.cbOperateCode=WIK_RIGHT;
	}
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}

void NCMJGameScence::NCMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_CHI_HU;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}

void NCMJGameScence::NCMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_C_XiaoHu kXiaoHu;
	kXiaoHu.cbOperateCode=WIK_XIAO_HU;
	kXiaoHu.cbOperateCard=0;
	SendSocketData(SUB_C_XIAOHU,&kXiaoHu,sizeof(kXiaoHu));

	WidgetFun::setVisible(this,"SelfActionNode",false);
}

void NCMJGameScence::NCMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*)
{
	showSaiZi(utility::parseUInt(WidgetFun::getWidgetUserInfo(this,"SaiZiNode","NextValue")));
}
void NCMJGameScence::NCMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
	}
	setCurrentPlayer(m_iCurrentUser,WIK_NULL);
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}

void NCMJGameScence::NCMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*)
{

}
void NCMJGameScence::NCMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*)
{
	NCMJButton_Ready(NULL,NULL);
}

void NCMJGameScence::NCMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
{
	if (WidgetFun::isWidgetVisble(this,"TalkImagic"))
	{
		JniFun::stopSoundRecord();
		WidgetFun::setVisible(this,"TalkImagic",false);
		return;
	}
	SoundFun::Instance().PaseBackMusic();
	WidgetFun::setVisible(this,"TalkImagic",true);
	JniFun::startSoundRecord();
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NCMJGameScence::endButtonTalk,this),5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}
void NCMJGameScence::endButtonTalk()
{
	NCMJButton_TalkEnd(NULL,NULL);
}
void NCMJGameScence::NCMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
{
	SoundFun::Instance().ResumeBackMusic();
	if (!WidgetFun::isWidgetVisble(this,"TalkImagic"))
	{
		return;
	}
	int iTimeID = utility::parseInt(WidgetFun::getWidgetUserInfo(this,"TalkImagic","TimeID"));
	TimeManager::Instance().removeByID(iTimeID);
	WidgetFun::setVisible(this,"TalkImagic",false);
	std::string kFileName = JniFun::stopSoundRecord();
	sendTalkFile(m_pLocal->GetChairID(),kFileName);
}