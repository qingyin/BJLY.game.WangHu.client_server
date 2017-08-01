#include "HNHomeScence.h"
#include "HNScenceManager.h"
#include "GameLib/Game/GameLib.h"


//完成通知
void HNHomeScence::OnGameItemFinish()
{
}
//完成通知
void HNHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//更新通知
void HNHomeScence::OnGameItemUpdateFinish()
{

}
//插入通知
void HNHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//更新通知
void HNHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//删除通知
void HNHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}
void HNHomeScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}
void HNHomeScence::Req_UserInfo(std::string kChannel)
{
	if (kChannel == "")
	{
		WidgetFun::setText(this,"HomeGold","0");
		return;
	}
	m_kActChannel = kChannel;
	static std::string s_kUrl = "http://120.76.118.20:8080/api/queryweixin?";
	static std::string s_kAppID = "701";
	static std::string s_kAppKey = "1669h01mes";
	std::string kUrl = s_kUrl;

	MCWebReq::pushValue(kUrl,"appid",s_kAppID);
	MCWebReq::pushValue(kUrl,"appkey",s_kAppKey);
	MCWebReq::pushValue(kUrl,"channel",kChannel);//"79969";
	MCWebReq::pushValue(kUrl,"u_weixin",(int)UserInfo::Instance().getUserID());

	MCWebReq::instance().sendRequestDocumentUrl(kUrl,nullptr,CC_CALLBACK_1(HNHomeScence::Rsp_UserInfo,this));
}
void HNHomeScence::Rsp_UserInfo(std::string kStr)
{
	if (kStr == "16")
	{
		NoticeMsg::Instance().ShowTopMsgByScript("ChannelError");
	}
	else
	{
		if (UserInfo::Instance().getUserChannel() == "")
		{
			m_kIndividualMission.modifyUserChannel(m_kActChannel);
		}
		Req_FangKaInfo(m_kActChannel);
	}
}
void HNHomeScence::Req_FangKaInfo(std::string kChannel)
{
	if (kChannel == "")
	{
		WidgetFun::setText(this,"HomeGold","0");
		return;
	}
	static std::string s_kUrl = "http://120.76.118.20:8080/api/pay/card?";
	static std::string s_kAppID = "701";
	static std::string s_kAppKey = "1669h01mes";
	static std::string s_kAppChannel = kChannel;//"79969";
	std::string kUrl = s_kUrl;

	MCWebReq::pushValue(kUrl,"appid",s_kAppID);
	MCWebReq::pushValue(kUrl,"appkey",s_kAppKey);
	MCWebReq::pushValue(kUrl,"wx_id",(int)UserInfo::Instance().getUserID());
	MCWebReq::pushValue(kUrl,"use_card","0");

	MCWebReq::instance().sendRequestDocumentUrl(kUrl,CC_CALLBACK_1(HNHomeScence::Rsp_FangKaInfo,this),nullptr);
}
void HNHomeScence::Rsp_FangKaInfo(rapidjson::Document* pDoc)
{
	std::string kStrUserCout = MCWebReq::getDataValueStr(pDoc,"card_num");

	WidgetFun::setText(this,"HomeGold",kStrUserCout);
	WidgetFun::setVisible(this,"ChannelPlane",false);
}
void HNHomeScence::onGPIndividualSuccess(int type, const char* szDescription)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}
void HNHomeScence::onGPIndividualFailure(int type, const char* szDescription)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}
void HNHomeScence::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	std::string kStr = utility::a_u8(szDescription);
	m_kMsgList = utility::split(kStr,"|");
	NextSpeaker();
}
void HNHomeScence::NextSpeaker()
{
	if (!m_kMsgList.size())
	{
		return;
	}
	std::string kStr = *m_kMsgList.begin();
	m_kMsgList.erase(m_kMsgList.begin());
	m_kMsgList.push_back(kStr);
	showSysSpeakerTxt(kStr,"255 255 255");
}
void HNHomeScence::showSysSpeakerTxt(std::string kTxt,std::string kColor)
{
	cocos2d::Node* pTxt = WidgetFun::getChildWidget(this,"SysSpeakerTxt");
	cocos2d::Node* pLayoutNode = WidgetFun::getChildWidget(this,"LayoutNode");
	WidgetFun::setText(pTxt,kTxt);
	WidgetFun::setTextColor(pTxt,kColor);
	float fMaxHeight = pLayoutNode->getContentSize().height;
	float fMaxWidth = pLayoutNode->getContentSize().width;
	float fTxtWidth = pTxt->getContentSize().width;
	pTxt->setPosition(0,-fMaxHeight);
	pTxt->stopAllActions();
	pTxt->setVisible(true);
	if (fMaxWidth > fTxtWidth)
	{
		cocos2d::CCAction * seq1 = cocos2d::CCSequence::create(
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,0)),
			cocos2d::CCDelayTime::create(6.0f), 
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,fMaxHeight)),
			cocos2d::CCVisibleAction::create(0,false),
			cocos2d::CCCallFunc::create(this,callfunc_selector(HNHomeScence::NextSpeaker)),
			NULL);
		pTxt->runAction(seq1);
	}
	else
	{
		float fPosX = fMaxWidth-fTxtWidth-10;
		cocos2d::CCAction * seq1 = cocos2d::CCSequence::create( 
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,0)),
			cocos2d::CCDelayTime::create(3.0f), 
			cocos2d::MoveTo::create(2.0f,cocos2d::Point(fPosX,0)),
			cocos2d::CCDelayTime::create(3.0f), 
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(fPosX,fMaxHeight)),
			cocos2d::CCVisibleAction::create(0,false),
			cocos2d::CCCallFunc::create(this,callfunc_selector(HNHomeScence::NextSpeaker)),
			NULL);
		pTxt->runAction(seq1);
	}
}