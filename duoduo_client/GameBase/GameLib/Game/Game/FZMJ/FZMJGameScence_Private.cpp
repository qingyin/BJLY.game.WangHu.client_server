#include "FZMJGameScence.h"
#include "Game/GameLib.h"
#include "FZMJPlayer.h"
#include "Game/Game/MissionWeiXin.h"

void FZMJGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("MJRule",(int)nRuleTag)).Value());
	}
}

void FZMJGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameHNMJ/xml/HNGamePrivateScence.xml",this);

	WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(FZMJGameScence::Button_WeiXinFriend));
	WidgetManager::addButtonCB("HNMJButton_PrivateEndWeiXinShare",this,button_selector(FZMJGameScence::Button_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(FZMJGameScence::ButtonPlayerHeadClick));
	WidgetManager::addButtonCB("Button_PrivateReturnHome",this,button_selector(FZMJGameScence::Button_ExitGameBase));
}
void FZMJGameScence::defaultPrivateState()
{
	WidgetFun::setVisible(this,"PrivateDismissPlane",false);
}
void FZMJGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	m_kRoomInfo = *pNetInfo;
	WidgetFun::setVisible(this,"PrivateInfo",true);
	WidgetFun::setVisible(this,"GoldRoomInfo",false);
	setPrivateInfo(pNetInfo);
	setPlayCount(pNetInfo->dwPlayCout);
	bool bShowDismiss = false;
	if (pNetInfo->dwCreateUserID == m_pLocal->GetUserID())
	{
		if (pNetInfo->bStartGame || pNetInfo->dwPlayCout == 0)
		{
			bShowDismiss = true;
		}
	}
	if (pNetInfo->bStartGame)
	{
		bShowDismiss = true;
	}
	WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",m_kPlayers.size() != GAME_PLAYER);
	WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);

	for (int i=0;i<MAX_PLAYER;i++)
	{
		if (m_pPlayer[i]->GetUserID() == pNetInfo->dwCreateUserID)
		{
			m_pPlayer[i]->showFangZhu(true);
		}
		else
		{
			m_pPlayer[i]->showFangZhu(false);
		}
	}

	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_WIN_LOSE1))
	{
		for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
		{
			FZMJPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",1000+(int)pNetInfo->kWinLoseScore[i]);
		}
	}
	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO))
	{
		for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
		{
			FZMJPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",(int)pNetInfo->kWinLoseScore[i]);
		}
	}
}
void FZMJGameScence::getGameRuleStr(std::vector<std::string>& kRuleList,dword	dwGameRuleIdex)
{
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_258,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_0));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZIMOHU,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_1));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_QIDUI,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_2));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_QIANGGANGHU,!HaveOptionRule(GAME_OPTION_RULE_HIDE_3));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO2,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_4));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO4,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_5));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO6,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_6));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_HONGZHONG,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_7));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_CS_ZHUANGXIANFEN,!HaveOptionRule(GAME_OPTION_RULE_HIDE_8));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO3,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_9));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO5,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_10));
}
void FZMJGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));
	int nJushu = pNetInfo->dwPlayCout;
	if (pNetInfo->bStartGame == 0 && nJushu<pNetInfo->dwPlayTotal)
	{
		nJushu++;
	}
	WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString(nJushu,"/",pNetInfo->dwPlayTotal));

	std::vector<std::string> kRuleList;
	if(!HaveOptionRule(GAME_OPTION_RULE_HIDE_GAMETYPE))
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("HNGameType",(int)pNetInfo->bGameTypeIdex)).Value());
	}
	getGameRuleStr(kRuleList,pNetInfo->bGameRuleIdex);
	for (int i=0;i<MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),false);
	}
	for (int i=0;i<(int)kRuleList.size() && i < MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),true);
		WidgetFun::setText(this,utility::toString("RoomGameRule",i),kRuleList[i]);
	}
}
void FZMJGameScence::setOutCardWaiteTime(int iOutCardWaiteTime)
{
	m_iOutCardWaiteTime = iOutCardWaiteTime;
}
void FZMJGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != MAX_PLAYER
		||pNetInfo->lPlayerAction.size() != MAX_PLAYER*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != MAX_PLAYER");
		return;
	}
	int nMaxPao = 0;
	int nMaxWin = 0;
	int nMaxPaoID = -1;
	int nMaxWinID = -1;
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		GamePlayer* pPlayer = m_pPlayer[i];
		if (!pPlayer)
		{
			continue;
		}
		int iChairID = pPlayer->GetChairID();
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",i)));
		WidgetFun::setText(pNode,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pNode,"ID",(int)pPlayer->GetUserID());
		WidgetFun::setText(pNode,"ActionTxt0",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+0]));
		WidgetFun::setText(pNode,"ActionTxt1",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+1]));
		WidgetFun::setText(pNode,"ActionTxt2",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+2]));
		WidgetFun::setText(pNode,"ActionTxt3",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+3]));
		WidgetFun::setText(pNode,"ActionTxt4",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+4]));
		WidgetFun::setText(pNode,"TotalScore",utility::toString((int)pNetInfo->lPlayerWinLose[iChairID]));
		WidgetFun::setVisible(pNode,"FangZhu",pPlayer->GetUserID() == m_kRoomInfo.dwCreateUserID);
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
		
		WidgetFun::setVisible(pNode,"DaYingJia",false);
		WidgetFun::setVisible(pNode,"ZuiJiaPaoShou",false);

		int nPaoNum = (int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+2];
		int nWinNum = (int)pNetInfo->lPlayerWinLose[iChairID];
		if (nPaoNum>nMaxPao)
		{
			nMaxPao = nPaoNum;
			nMaxPaoID = i;
		}
		if (nWinNum>nMaxWin)
		{
			nMaxWin = nWinNum;
			nMaxWinID = i;
		}
	}
	if (nMaxPao>0)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",nMaxPaoID)));
		WidgetFun::setVisible(pNode,"ZuiJiaPaoShou",true);
	}
	if (nMaxWin>0)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",nMaxWinID)));
		WidgetFun::setVisible(pNode,"DaYingJia",true);
	}

	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	if (pJieSuanNode->getActionByTag(1) == NULL 
		&& pJieSuanNode->isVisible() == false)
	{
		WidgetFun::setVisible(pRootNode,"PrivateEndPlane",true);
	}
	else
	{
		int iTime = utility::parseInt(WidgetFun::getWidgetUserInfo(pJieSuanNode,"Time"))+3;
		WidgetFun::getChildWidget(pRootNode,"PrivateEndPlane")->runAction(cocos2d::CCVisibleAction::create(iTime,true));
	}

	if(HaveOptionRule(GAME_OPTION_RULE_PRIVATAEND_RETURN_HOME))
	{
		WidgetFun::setVisible(pRootNode,"Button_PrivateReturnHome",true);
		WidgetFun::setVisible(pRootNode,"Button_PrivateAgagin",false);
	}
}
void FZMJGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(3.0f,false));
		return;
	}
	if (!pNode->isVisible())
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"DissPrivateStateWaiteTime");
		pNode->stopAllActions();
		pNode->runAction(cocos2d::MoveExTxtTime::create(pNetInfo->dwValue2));
	}
	pNode->setVisible(true);
	int kChairID[4] = {1,1,1,1};

	bool bShowSelfAction = true;
	int iIdex = 0;
	for (int i = 0;i<(int)pNetInfo->dwDissUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwDissChairID[i]);
		kChairID[pNetInfo->dwDissChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text0",kName);
		iIdex++;
	}
	for (int i = 0;i<(int)pNetInfo->dwNotAgreeUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwNotAgreeChairID[i]);
		kChairID[pNetInfo->dwNotAgreeChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text1",kName);
		iIdex++;
	}
	for (int i = 0;i<4;i++)
	{
		if (kChairID[i] == 0)
		{
			continue;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),"");
		WidgetFun::setText(pNode,utility::toString("DissPrivateState",iIdex),"");
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		iIdex++;
	}
	WidgetFun::setVisible(pNode,"PrivateDismissAction",bShowSelfAction);
}
void FZMJGameScence::Button_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	if(isHZ())
	{
		int iCout = 0;
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO2)))
		{
			iCout = 2;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO3)))
		{
			iCout = 3;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO4)))
		{
			iCout = 4;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO5)))
		{
			iCout = 5;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO6)))
		{
			iCout = 6;
		}
		std::string kQu = utility::getScriptString("HZWeiXinSharTitleQu");

		std::string kTitle = utility::getScriptReplaceValue("HZWeiXinSharTitle",
			WidgetFun::getText(this,"RoomID"),kQu);

		std::string kDes =  utility::getScriptReplaceValue("HZWeiXinSharDes",
			kQu,(int)m_kRoomInfo.dwPlayTotal,iCout);

		MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("HNWeiXinSharUrl"),
			kTitle,
			kDes,
			MissionWeiXin::SHARE_SESSION);
	}
	else
	{
		std::string kHNWeiXinSharDes,kGameTypeStr;

		kGameTypeStr = ScriptData<std::string>(utility::toString("HNGameType",(int)m_kRoomInfo.bGameTypeIdex)).Value();

		kHNWeiXinSharDes = utility::getScriptReplaceValue("HNWeiXinSharDes",
			kGameTypeStr,(int)m_kRoomInfo.dwRoomNum,(int)m_kRoomInfo.dwPlayTotal);
		
		std::string kRuleTxt;
		std::vector<std::string> kRuleList;
		getGameRuleStr(kRuleList,m_kRoomInfo.bGameRuleIdex);
		for (int i = 0;i<(int)kRuleList.size();i++)
		{
			kRuleTxt += kRuleList[i];
			kRuleTxt += ",";
		}
		utility::StringReplace(kHNWeiXinSharDes,"#",kRuleTxt);

		MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("HNWeiXinSharUrl"),
			utility::getScriptString("HNWeiXinSharTitle"),
			kHNWeiXinSharDes,
			MissionWeiXin::SHARE_SESSION);
	}
}
void FZMJGameScence::Button_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static FZMJGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
		}
	},"screenshot.png");

}
void FZMJGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	GamePlayer* pPlayer = getPlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),
		utility::paseInt(pPlayer->GetUserID(),6),
		pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}

void FZMJGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;
}
int FZMJGameScence::getPlayCount()
{
	return m_nPlayCount;
}
