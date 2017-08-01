#include "NNGameScence.h"
#include "Game/GameLib.h"

#include "CMD_Ox.h"
#include "NNPlayer.h"
#include "NNGameLogic.h"

void NNGameScence::initNet()
{
	addNetCBDefine(SUB_S_CALL_BANKER,this,NNGameScence::OnSubCallBanker);
	addNetCBDefine(SUB_S_GAME_START,this,NNGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_ADD_SCORE,this,NNGameScence::OnSubAddScore);
	addNetCBDefine(SUB_S_SEND_CARD,this,NNGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPEN_CARD,this,NNGameScence::OnSubOpenCard);
	addNetCBDefine(SUB_S_PLAYER_EXIT,this,NNGameScence::OnSubPlayerExit);
	addNetCBDefine(SUB_S_GAME_END,this,NNGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_AMDIN_COMMAND,this,NNGameScence::OnSubAdminControl);
}
//用户叫庄
void NNGameScence::OnSubCallBanker(void * pBuffer, int wDataSize)
{
}
//游戏开始
void NNGameScence::OnSubGameStart(void * pBuffer, int wDataSize)
{
}
//用户加注
void NNGameScence::OnSubAddScore(void * pBuffer, int wDataSize)
{
}
//发牌消息
void NNGameScence::OnSubSendCard(void * pBuffer, int wDataSize)
{
}
//用户摊牌
void NNGameScence::OnSubOpenCard(void * pBuffer, int wDataSize)
{
}
//用户强退
void NNGameScence::OnSubPlayerExit(void * pBuffer, int wDataSize)
{
}
//游戏结束
void NNGameScence::OnSubGameEnd(void * pBuffer, int wDataSize)
{
}
//控制
void NNGameScence::OnSubAdminControl(void * pBuffer, int wDataSize)
{
}