#include "LPGameScence.h"
#include "Game/GameLib.h"
#include "CMD_Land.h"

bool LPGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_BET:		//下注状态
		{
			OnBetScence(data,dataSize);
			return true;
		}
	case GAME_SCENE_PLAY:		//游戏状态
		{
			OnPlayScence(data,dataSize);
			return true;
		}
	}
	return true;
}
void LPGameScence::OnBetScence(void* data, int wDataSize)
{
	cocos2d::log("OnBetScence");
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_StatusBet));
	if (wDataSize!=sizeof(CMD_S_StatusBet)) return;

	CMD_S_StatusBet* pStateBet = (CMD_S_StatusBet*)data;

	cleanYaChips(m_pYaChipList);
	cleanYaChips(m_kBetChipMap);
	showClock(pStateBet->cbTimeLeave);
	m_nGameState = GAME_SCENE_BET;
	
}
void LPGameScence::OnPlayScence(void* data, int wDataSize)
{
	cocos2d::log("OnPlayScence");
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pStatePlay = (CMD_S_StatusPlay*)data;
	m_nGameState = GAME_SCENE_PLAY;
//	showClock(pStatePlay->cbTimeLeave);
}