#include "XZDDGameScence.h"
#include "Game/GameLib.h"
#include "XZDDPlayer.h"
#include "Game/Game/MissionWeiXin.h"

void XZDDGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("MJRule",(int)nRuleTag)).Value());
	}
}
void XZDDGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("Script/HNGamePrivateScence.xml",this);

	WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(XZDDGameScence::XZDDButton_WeiXinFriend));
	WidgetManager::addButtonCB("Button_PrivateEndWeiXinShare",this,button_selector(XZDDGameScence::XZDDButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(XZDDGameScence::ButtonPlayerHeadClick));
}
void XZDDGameScence::defaultPrivateState()
{
	if (WidgetFun::getChildWidget(this,"PrivateDismissPlane"))
	{
		WidgetFun::setVisible(this,"PrivateDismissPlane",false);
		WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",false);
		WidgetFun::setVisible(this,"TalkImagic",false);
	}
}
void XZDDGameScence::showJieSuanInfo(CMD_S_GameEnd* pGameEnd)
{
	for (int i=0;i<MAX_PLAYER;i++)
	{

		int iChirID = m_pPlayer[i]->GetChairID();
		cocos2d::Node* pJieSuanInfo = WidgetFun::getChildWidget(this,utility::toString("JieSuanInfo",i));
		WidgetFun::setText(pJieSuanInfo,"Jiesuan_Nicke",m_pPlayer[i]->GetNickName());
		WidgetFun::setText(pJieSuanInfo,"Jiesuan_GoldNum",pGameEnd->lGameScore[iChirID]);

		WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Zhuang",false);
		WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Hu",false);
		if (m_pPlayer[i]->getChiHuCard() != 0)
		{
			WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Hu",true);
		}
		if (iChirID == m_iBankerUser)
		{
			WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Zhuang",true);
		}
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pJieSuanInfo,"JieSuanCardNode");
		m_pPlayer[i]->showJieSuanCard(pCardNode,m_pPlayer[i]->getWeaveItemArray(),m_pPlayer[i]->getWeaveCount(),
			pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
	}
	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	pJieSuanNode->runAction(cocos2d::CCVisibleAction::create(3.0f,true));
	int nChairID = m_pLocal->GetChairID();
	cocos2d::Node* pJieSuanTitle = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_Title");
	cocos2d::Node* pJieSuanNPC = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_NPC");
	if (pGameEnd->lGameScore[nChairID]>0)
	{
		std::string kTitleWin = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Win");
		std::string kNPCXiao = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Smile");

		WidgetFun::setImagic(pJieSuanTitle,kTitleWin,false);
		WidgetFun::setImagic(pJieSuanNPC,kNPCXiao,false);
	}
	else if (pGameEnd->lGameScore[nChairID]== 0)
	{
		std::string kTitlePing = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Ping");
		std::string kNPCXiao = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Smile");

		WidgetFun::setImagic(pJieSuanTitle,kTitlePing,false);
		WidgetFun::setImagic(pJieSuanNPC,kNPCXiao,false);
	}
	else if (pGameEnd->lGameScore[nChairID] < 0)
	{
		std::string kTitleLose = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Lose");
		std::string kNPCKu = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Cry");

		WidgetFun::setImagic(pJieSuanTitle,kTitleLose,false);
		WidgetFun::setImagic(pJieSuanNPC,kNPCKu,false);
	}
}
void XZDDGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
	WidgetFun::setVisible(this,"MenuNode",false);
	setPrivateInfo(pNetInfo);
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
	WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",!pNetInfo->bStartGame 
		&& pNetInfo->dwCreateUserID == m_pLocal->GetUserID());
	WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);

	for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		WidgetFun::setText(pPlayer->getPlayerNode(),
			"GoldTxt",1000+(int)pNetInfo->kWinLoseScore[i]);
	}
}

void XZDDGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));
	if (pNetInfo->bPlayCoutIdex == 0)
	{
		WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));
	}
	else
	{
		WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));
	}

	std::vector<std::string> kRuleList;
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_XZ,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_XL,true);

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

void XZDDGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != MAX_PLAYER
		||pNetInfo->lPlayerAction.size() != MAX_PLAYER*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != MAX_PLAYER");
		return;
	}
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
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
	}
	WidgetFun::setVisible(pRootNode,"PrivateEndPlane",true);
}
void XZDDGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	pNode->setVisible(true);
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(1.0f,false));
		return;
	}
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
void XZDDGameScence::XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("WeiXinSharUrl"),
		utility::getScriptString("WeiXinSharTitle"),
		utility::getScriptReplaceValue("PrivateTxt1",WidgetFun::getText(this,"RoomID")),
		MissionWeiXin::SHARE_SESSION);
}
void XZDDGameScence::XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static XZDDGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
			pTemp->Button_ExitGameBase(NULL,NULL);
			pTemp->ExitGame();
			WidgetFun::setVisible(pTemp,"PrivateEndPlane",false); 
		}
	},"screenshot.png");


}
void XZDDGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	GamePlayer* pPlayer = getBasePlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),
		utility::paseInt(pPlayer->GetUserID(),6),
		pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}