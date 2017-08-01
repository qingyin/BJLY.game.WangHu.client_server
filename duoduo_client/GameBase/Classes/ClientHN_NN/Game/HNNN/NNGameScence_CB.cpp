#include "NNGameScence.h"
#include "Game/GameLib.h"

#include "CMD_Ox.h"
#include "NNGameLogic.h"
#include "NNPlayer.h"

bool NNGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_TK_FREE)
	{
		OnFreeScence(data,dataSize);
	}
	if (cbGameStatus == GS_TK_CALL)
	{
	}
	if (cbGameStatus == GS_TK_SCORE)
	{
	}
	if (cbGameStatus == GS_TK_PLAYING)
	{
	}
	return true;
}
void NNGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	CMD_S_StatusFree* pNetInfo = (CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() == US_FREE);
}
void NNGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;

	defaultState();


}
void NNGameScence::OnGFGameClose(int iExitCode)
{
	ExitGame();
}
void NNGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}