#include "SCAddRankScence.h"
#include "SCScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(SCAddRankScence);

SCAddRankScence::SCAddRankScence()
	:m_kAddRankMission(ScriptData<std::string>("address").Value().c_str()
	,ScriptData<int>("Port").Value())
{
	init();
	m_kAddRankMission.setMissionSink(this);

	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(SCAddRankScence::upPlayerInfo));
}
SCAddRankScence::~SCAddRankScence()
{

}
bool SCAddRankScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/SCAddRankScence.xml",this);

	WidgetManager::addButtonCB("AddRankButton_Rank0",this,button_selector(SCAddRankScence::Button_GetRank));
	WidgetManager::addButtonCB("AddRankButton_Rank1",this,button_selector(SCAddRankScence::Button_GetRank));
	WidgetManager::addButtonCB("AddRankButton_Rank2",this,button_selector(SCAddRankScence::Button_GetRank));
	WidgetManager::addButtonCB("Button_AddRanScence",this,button_selector(SCAddRankScence::Button_AddRanScence));
	
	return true;
}
void SCAddRankScence::HideAll()
{
	WidgetFun::setVisible(this,"AddRankPlane",false);
}
void SCAddRankScence::upPlayerInfo()
{
	m_kAddRankMission.GetAddBank(0);
}
void SCAddRankScence::setRankItemInfo(cocos2d::Node* pNode,std::string kName,int iFaceID,int iUserPoint,int iRank)
{
	WidgetFun::setText(pNode,"RankName",utility::a_u8(kName));
	WidgetFun::setText(pNode,"RankTxt",iRank+1);
	WidgetFun::setText(pNode,"RankPoint",iUserPoint);
	if (iRank == -1)
	{
		WidgetFun::setVisible(pNode,"RankOut",true);
		WidgetFun::setVisible(pNode,"RankOther",false);
		WidgetFun::setVisible(pNode,"Rank123",false);
	}
	else if (iRank < 3)
	{
		WidgetFun::setVisible(pNode,"RankOut",false);
		WidgetFun::setVisible(pNode,"RankOther",false);
		WidgetFun::setVisible(pNode,"Rank123",true);
		WidgetFun::setPlaceImagicKey(pNode,"Rank123",utility::toString("Rank",iRank));
	}
	else
	{
		WidgetFun::setVisible(pNode,"RankOut",false);
		WidgetFun::setVisible(pNode,"RankOther",true);
		WidgetFun::setVisible(pNode,"Rank123",false);
	}
}

void SCAddRankScence::onGPAwardInfoResult(CMD_GP_BackAddBankAwardInfo* pNetInfo)
{

}
void SCAddRankScence::onGPBackAddBankResult(CMD_GP_BackAddBank* pNetInfo)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,utility::toString("Rank",pNetInfo->iRankIdex));
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(pRootNode,"Rank_List");
	pList->removeAllChildren();
	cocos2d::Node* pItemRootNode = NULL;
	int iSelfRank = -1;
	for (int i = 1;i<(int)pNetInfo->kNickName.size();i++)
	{
		int iLogicIdex = i - 1;
		int iIdex1 = iLogicIdex / 2;
		int iIdex2 = iLogicIdex % 2;
		if (iIdex2 == 0)
		{
			pItemRootNode = WidgetManager::Instance().createWidget("RankListItem",pList);
			WidgetFun::setVisible(pItemRootNode,"BackImagic1",false);
			WidgetFun::setVisible(pItemRootNode,"Item1",false);
		}
		else
		{
			WidgetFun::setVisible(pItemRootNode,"BackImagic1",true);
			WidgetFun::setVisible(pItemRootNode,"Item1",true);
		}
		cocos2d::Node* pItemNode = WidgetFun::getChildWidget(pItemRootNode,utility::toString("Item",iIdex2));
		pItemNode->setVisible(true);
		setRankItemInfo(pItemNode,pNetInfo->kNickName[iLogicIdex],pNetInfo->kFaceID[iLogicIdex],pNetInfo->kUserPoint[iLogicIdex],iLogicIdex);

		if (UserInfo::Instance().getUserID() == pNetInfo->kUserID[iLogicIdex])
		{
			iSelfRank = iLogicIdex;
		}
	}
	if (iSelfRank == -1)
	{
		setRankItemInfo(WidgetFun::getChildWidget(pRootNode,"SelfRank"),UserInfo::Instance().getUserNicName(),0,0,-1);
	}
	else
	{
		setRankItemInfo(WidgetFun::getChildWidget(pRootNode,"SelfRank"),pNetInfo->kNickName[0],pNetInfo->kFaceID[0],pNetInfo->kUserPoint[0],iSelfRank);
	}
}
void SCAddRankScence::Button_AddRanScence(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::ActionStart(this,"AddRankPlane");
}
void SCAddRankScence::Button_GetRank(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pInfo,"Idex"));
	m_kAddRankMission.GetAddBank(iIdex);
}