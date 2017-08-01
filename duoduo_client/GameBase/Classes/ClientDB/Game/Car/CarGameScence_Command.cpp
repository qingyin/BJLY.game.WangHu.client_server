#include "CarGameScence.h"
#include "Game/GameLib.h"
#include "CMD_RedNine100.h"

void CarGameScence::Command_UserBet( int nIndex,SCORE nScore )
{
	if (!(nIndex>=1 && nIndex<=S_AREA_COUNT)) return;
	if (nScore<=0)return;

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	zeromemory(&PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbJettonArea = nIndex;
	PlaceJetton.lJettonScore = nScore;

	if (m_nKeYongScore < nScore )
	{
		nScore = m_nKeYongScore;
	}
	m_nKeYongScore -= nScore;
	setUserKeYong(m_nKeYongScore);

	setSelfBet(nIndex,nScore);
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
}
