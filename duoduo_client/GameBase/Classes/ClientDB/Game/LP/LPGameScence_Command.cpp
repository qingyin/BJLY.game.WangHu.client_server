#include "LPGameScence.h"
#include "Game/GameLib.h"
#include "CMD_Land.h"

void LPGameScence::upSelfPlayerInfo()
{
	m_lTotalScore = getSelfGamePlayer()->GetUserScore();
	WidgetFun::setText(this,"LPGold_Num",m_lTotalScore);
}
void LPGameScence::Command_PlaceBet(int iArea,int iBetScore,float x,float y)
{
	CMD_C_UserBet kBetInfo;
	zeromemory(&kBetInfo,sizeof(kBetInfo));
	kBetInfo.cbBetArea = iArea;
	kBetInfo.lBetScore = iBetScore;
	kBetInfo.kPosX = x;
	kBetInfo.kPosY = y;

	UserBetInfo kUserBetInfo;
	kUserBetInfo.cbBetArea = iArea;
	kUserBetInfo.lBetScore = iBetScore;
	kUserBetInfo.kPosX = x;
	kUserBetInfo.kPosY = y;

	m_kLastBetListTemp.push_back(kUserBetInfo);

	WidgetFun::setButtonEnabled(this,"LPButton_LastBet",true);

	SendSocketData(SUB_C_USER_BET,&kBetInfo,sizeof(kBetInfo));
}
void LPGameScence::Command_CancleBet(int nType)
{
	CMD_C_UserCancleBet kInfo;
	kInfo.nType = nType;

	SendSocketData(SUB_C_USER_CANCLE_BET,&kInfo,sizeof(kInfo));
}