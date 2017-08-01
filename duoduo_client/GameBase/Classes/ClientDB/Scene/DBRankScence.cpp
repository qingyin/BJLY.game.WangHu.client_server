#include "DBRankScence.h"
#include "DBScenceManager.h"
#include "Game/GameLib.h"
#include "DBHomeScence.h"

FV_SINGLETON_STORAGE(DBRankScence);

DBRankScence::DBRankScence()
	:m_kAddRankMission(ScriptData<std::string>("address").Value().c_str()
	,ScriptData<int>("Port").Value())
{
	init();
	m_kAddRankMission.setMissionSink(this);

	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(DBRankScence::getRank));
}
DBRankScence::~DBRankScence()
{

}
bool DBRankScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/DBAddRankScence.xml",this);

	WidgetManager::addButtonCB("Button_GoldRankScence",this,button_selector(DBRankScence::Button_GoldRankScence));
	
	return true;
}
void DBRankScence::HideAll()
{
	WidgetFun::setVisible(this,"AddRankPlane",false);
}
void DBRankScence::getRank()
{
	m_kAddRankMission.GetAddBank(3);
}
void DBRankScence::setRankItemInfo(cocos2d::Node* pNode,std::string kName,int iFaceID,int iUserPoint,int iRank)
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

void DBRankScence::onGPAwardInfoResult(CMD_GP_BackAddBankAwardInfo* pNetInfo)
{

}
void DBRankScence::onGPBackAddBankResult(CMD_GP_BackAddBank* pNetInfo)
{

	if (pNetInfo->kNickName.size() <= 0)
	{
		CCAssert(false,"");
		return;
	}
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,utility::toString("Rank",pNetInfo->iRankIdex));
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(pRootNode,"Rank_List");
	pList->removeAllChildren();
	cocos2d::Node* pItemRootNode = NULL;
	int iSelfRank = -1;
	cocos2d::Node* pHomeNode = WidgetFun::getChildWidget(DBHomeScence::pInstance(),"Button_GoldRankScence");
	for (int i = 0;i<(int)pNetInfo->kNickName.size();i++)
	{
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("RankListItem",pList);
		setRankItemInfo(pItemNode,pNetInfo->kNickName[i],pNetInfo->kFaceID[i],pNetInfo->kUserPoint[i],i);

		if (UserInfo::Instance().getUserID() == pNetInfo->kUserID[i])
		{
			iSelfRank = i;
		}
		if (i < 3)
		{
			cocos2d::Node* pHomeChildNode = WidgetFun::getChildWidget(pHomeNode,utility::toString("HomeRank",i));
			WidgetFun::setText(pHomeChildNode,"HomeRankName",utility::a_u8(pNetInfo->kNickName[i]));
		}
	}
}
void DBRankScence::Button_GoldRankScence(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::ActionStart(this,"AddRankPlane");
	getRank();
}