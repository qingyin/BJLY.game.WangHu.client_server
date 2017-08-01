#include "LPGameScence.h"
#include "Game/GameLib.h"
#include "CMD_Land.h"
#include "LPYaCell.h"
#include "Game/Script/SoundFun.h"

#define CheakNetInfo(InfoType,pNetInfo) \
	ASSERT(wDataSize == sizeof(InfoType));\
	if (wDataSize!=sizeof(InfoType)) return;\
	InfoType* pNetInfo = (InfoType*)pBuffer;\

void LPGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,LPGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_GAME_END,this,LPGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_USER_BET,this,LPGameScence::OnSubUserBet);
	addNetCBDefine(SUB_S_USER_CANCLE_BET,this,LPGameScence::OnSubUserCancleBet);
}
void LPGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	CheakNetInfo(CMD_S_GameStart,pGameStart);

	cleanYaChips(m_pYaChipList);
	cleanYaChips(m_kBetChipMap);

	showClock(pGameStart->cbTimeLeave);
	m_nGameState = GAME_SCENE_BET;
	m_lWinScore = 0;

	showCheck(false);
	WidgetFun::runWidgetAction(this,"LPTipNode_Start","Start");
	WidgetFun::setButtonEnabled(this,"LPButton_ResetAll",true);
	WidgetFun::setButtonEnabled(this,"LPButton_Reset",true);
	WidgetFun::setVisible(this,"LPWheelPointNode",false);
	WidgetFun::setVisible(this,"LPPointNode",false);

	SoundFun::Instance().playEffect("BJLSound/voice_kaishixiazhu.mp3");
}
void LPGameScence::OnSubGameEnd(const void * pBuffer, word wDataSize)
{
	//效验数据
	CheakNetInfo(LPCMD_S_GameEnd,pGameEnd);

	SoundFun::Instance().playEffect("BJLSound/voice_tingzhixiazhu.mp3");

	WidgetFun::setVisible(this,"LPTimeNode",false);
	showClockTimeCallBack("LPTime_Num",pGameEnd->cbTimeLeave-15,
		CC_CALLBACK_0(LPGameScence::playTimeOver1,this),0); 
	showClockTimeCallBack("LPGoldBg",pGameEnd->cbTimeLeave-8,
		CC_CALLBACK_0(LPGameScence::playTimeOver2,this),0); 

	wheel_run(pGameEnd->bWinNumber);
	m_nGameState = GAME_SCENE_PLAY;
	m_nEndNum = pGameEnd->bWinNumber;
	m_lWinScore = pGameEnd->lWinScore[getSelfChairID()];

	WidgetFun::runWidgetAction(this,"LPTipNode_Stop","Start");
	if (!m_kLastBetListTemp.empty())
	{
		m_kLastBetList = m_kLastBetListTemp;//保存本局的押注数据
		m_kLastBetListTemp.clear();
	}
}
void LPGameScence::OnSubUserBet(const void * pBuffer, word wDataSize)
{
	//效验数据
	CheakNetInfo(CMD_S_UserBet,pUserBet);
	if (pUserBet->wChairID == getSelfChairID())
	{
		setUserScore(-pUserBet->lBetScore);
	}
	else
	{
		Node* LPTableNode = WidgetFun::getChildWidget(this,"LPTableNode");
		Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","OtherPos"));

		YaChip* pChip = YaChip::create(pUserBet->lBetScore,false);
		pChip->getNode()->setPosition(pos);
		LPTableNode->addChild(pChip->getNode());
		pChip->getNode()->runAction(MoveTo::create(0.1f,Vec2(pUserBet->kPosX,pUserBet->kPosY)));
		pChip->setYaID(pUserBet->cbBetArea);

		SoundFun::Instance().playEffect("BJLSound/effect_rw_chips.mp3");
		BetChipMap::iterator iter = m_kBetChipMap.find(pUserBet->wChairID);
		if (iter == m_kBetChipMap.end())
		{
			ChipList kChipList;
			kChipList.push_back(pChip);
			m_kBetChipMap.insert(std::make_pair(pUserBet->wChairID,kChipList));
		}
		else
		{
			iter->second.push_back(pChip);
		}
	}
}

void LPGameScence::OnSubUserCancleBet(const void* pBuffer,word wDataSize)
{
	//效验数据
	CheakNetInfo(CMD_S_UserCancleBet,pCancelBet);
	if (pCancelBet->wChairID == getSelfChairID())
	{
		if (pCancelBet->nType == LP_USER_CANCEL_ALL||pCancelBet->nType == LP_USER_CANCEL_LAST)
		{
			recycleYaChips(m_pYaChipList,true);
			m_kLastBetListTemp.clear();
			upSelfPlayerInfo();
		}
		else if(pCancelBet->nType == LP_USER_CANCEL_ONCE && !m_pYaChipList.empty())
		{
			ChipList::reverse_iterator iter_re = m_pYaChipList.rbegin();
			recycleYaChip(*iter_re,true);

			m_pYaChipList.pop_back();
			m_kLastBetListTemp.pop_back();

			setUserScore(pCancelBet->lBetScore);
		}

		if (pCancelBet->nType == LP_USER_CANCEL_LAST)
		{
			TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(LPGameScence::betLastOnce,this),0.1f);
		}
	}
	else
	{
		userCancleBet(pCancelBet->wChairID,pCancelBet->nType);
	}
}

void LPGameScence::userCancleBet(word wChairID,int nCancleType)
{
	if (wChairID== getSelfChairID()) return;

	BetChipMap::iterator iter = m_kBetChipMap.find(wChairID);
	if (iter != m_kBetChipMap.end())
	{
		if (nCancleType == LP_USER_CANCEL_ALL)
		{
			recycleYaChips(iter->second,false);
			m_kBetChipMap.erase(iter);
		}
		else if (nCancleType == LP_USER_CANCEL_ONCE)
		{
			ChipList::reverse_iterator iter_re = iter->second.rbegin();
			recycleYaChip(*iter_re,false);
			iter->second.pop_back();
		}
	}
}void LPGameScence::betLastOnce()
{
	for (int i=0;i<(int)m_kLastBetList.size();i++)
	{
		Command_PlaceBet(m_kLastBetList[i].cbBetArea,m_kLastBetList[i].lBetScore,m_kLastBetList[i].kPosX,m_kLastBetList[i].kPosY);

		Node* LPTableNode = WidgetFun::getChildWidget(this,"LPTableNode");
		Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this,"LPTableNode","WinPos"));

		YaChip* pChip = YaChip::create(m_kLastBetList[i].lBetScore);
		LPTableNode->addChild(pChip->getNode());
		pChip->moveToTable(pos,Vec2(m_kLastBetList[i].kPosX,m_kLastBetList[i].kPosY),i);
		pChip->setYaID(m_kLastBetList[i].cbBetArea);

		addChipInList(pChip);
	}
}
