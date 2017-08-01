#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

#define IDI_TIMER_XIAO_HU			1 //小胡
#define IDI_TIMER_PLAYER_ACTION		2 //出牌
#define IDI_TIMER_HAIDI_ACTION		3 //海底
#define IDI_TIMER_CS_GANG_ACTION	4 //长沙杠

#define TIME_XIAO_HU				3
#define TIME_PLAYER_ACTION			30
#define TIME_PLAYER_ACTION_TRUSTEE	2
#define TIME_HAIDI_ACTION	3
//构造函数
CTableFrameSink::CTableFrameSink()
{
	m_pITableFrame = NULL;
	m_dwGameRuleIdex = 0;
	m_cbGameTypeIdex = 0;
	isDuoPao = false;
	isGameOver = false;
	m_bIsNullWin = false;
	m_iGameCount = 0;
	m_wBankerUser=INVALID_CHAIR;

	isQiPaiPlayer0 = false;
	isQiPaiPlayer1 = false;
	isQiPaiPlayer2 = false;
	isQiPaiPlayer3 = false;

	m_Player = 0;
	t_Player = 0;
	l_Player = 0;
	r_Player = 0;
	//游戏变量
	RepositionSink();
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	m_cbGameTypeIdex = GAME_TYPE_ZZ;
	if (pCustomConfig->cbGameType1)
	{
		m_cbGameTypeIdex = GAME_TYPE_ZZ;
	}
	if (pCustomConfig->cbGameType2)
	{
		m_cbGameTypeIdex = GAME_TYPE_CS;
	}
	for (int i = 0;i<GAME_TYPE_ZZ_HONGZHONG_GZ;i++)
	{
		if (pCustomConfig->cbGameRule[i])
		{
			FvMask::Add(m_dwGameRuleIdex,_MASK_(i+1));
		}
	}

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_GangScore,sizeof(m_GangScore) );
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_wLostFanShu,sizeof(m_wLostFanShu) );
	ZeroMemory( m_cbChiHuCardData,sizeof(m_cbChiHuCardData) );
	ZeroMemory( m_cbGangInfo,sizeof(m_cbGangInfo) );
	
	//出牌信息
	m_kUserActionStates1.m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
	
	//发牌信息
	m_cbHaiDiSendCardData = 0;
	m_cbNiaoCount = 0;
	m_cbNiaoPick = INVALID_BYTE;
	m_kUserActionStates1.m_cbSendCardData=0;
	m_kUserActionStates2.m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	clearUserActionStates();

	m_wHaiDiCout = 0;
	m_wHaiDiUser=INVALID_CHAIR;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;
	m_wNiaoDuoPaoStartChairID=0;
	
	//状态变量
	m_bGangStatus = STATE_NULL;
	m_bGangOutStatus = false;
	m_bGangStatus_CS = false;
	m_wCSGangUser = 0;

	/************************************************************************/
	/* 冲锋鸡and责任鸡                                                      */
	/************************************************************************/
	m_dFirstChickID = -1;
	m_dErrandChickID = -1;
	m_cbChichCount = -1;
	m_cbChickID[0] = -1;
	m_cbChickID[1] = -1;
	m_cbChickID[2] = -1;
	m_cbChickID[3] = -1;

	//用户状态
	clearUserActionStates();
	
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		m_kUserActionStates1.m_ChiHuRight[i].SetEmpty();
		m_kUserActionStates2.m_ChiHuRight[i].SetEmpty();
		m_StartHuRight[i].SetEmpty();
		ZeroMemory(m_WeaveItemArray[i],sizeof(m_WeaveItemArray[i]));
	}
	//组合扑克
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	m_lNiaoScore = 0;
	ZeroMemory( m_lStartHuScore,sizeof(m_lStartHuScore) );
	ZeroMemory( m_cbCardDataNiao,sizeof(m_cbCardDataNiao) );
	ZeroMemory( m_cbChairIDNiaoCout,sizeof(m_cbChairIDNiaoCout) );
	ZeroMemory( m_cbVaildChairIDNiaoCout,sizeof(m_cbVaildChairIDNiaoCout) );
	ZeroMemory( m_cbMasterCheckCard,sizeof(m_cbMasterCheckCard) );
	ZeroMemory( m_cbCanHu,sizeof(m_cbCanHu) );
	ZeroMemory( m_bHasCSGang,sizeof(m_bHasCSGang) );
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_cbMasterZhaNiao[i] = INVALID_BYTE;
	}

	if (m_pITableFrame)
	{
		m_pITableFrame->KillGameTimer(IDI_TIMER_PLAYER_ACTION);
		m_pITableFrame->KillGameTimer(IDI_TIMER_XIAO_HU);
	}
	return;
}


//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	//qiHuPlayerVector.clear();
	RepositionSink();
	if (m_wBankerUser == INVALID_CHAIR)
	{
		//随机庄
		m_wBankerUser = rand()%4;
	}

	/************************************************************************/
	/* 2017年3月17日16:03:27    修改游戏开始时建房者做庄                    */
	/************************************************************************/
	if (m_iGameCount > 0)
	{
		//荒庄情况
		if (m_bIsNullWin)
		{
		}
		else if (!isDuoPao && !isGameOver)
		{
			m_wBankerUser = rand()%4;
		}
	}
	++m_iGameCount;

	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_cbMasterZhaNiao[i] = INVALID_BYTE;
	}

	if (hasRule(GAME_TYPE_ZZ_HONGZHONG))
	{
		Shuffle(m_cbRepertoryCard,MAX_REPERTORY_HZ);
	}
	else
	{
		Shuffle(m_cbRepertoryCard,MAX_REPERTORY);
	}
	if (m_cbGameTypeIdex == GAME_TYPE_ZZ)
	{
		GameStart_ZZ();
	}
	else if (m_cbGameTypeIdex == GAME_TYPE_CS)
	{
		GameStart_CS();
	}
	return true;
}

void CTableFrameSink::Shuffle(BYTE* pRepertoryCard,int nCardCount)
{
	m_cbLeftCardCount = nCardCount;
	m_GameLogic.RandCardData(pRepertoryCard,nCardCount);

	//test
	//BYTE byTest[] = {
	//	0x11,0x11,0x12,
	//	0x12,0x13,0x13,
	//	0x16,0x19,0x19,
	//	0x19,0x16,0x19,				
	//};
	//int iCout = CountArray(byTest);
	//for (int i = 0;i<iCout;i++)
	//{
	//	pRepertoryCard[m_cbLeftCardCount-1-i] = byTest[i];
	//}
	//m_wBankerUser = 2;
	//end test

	//分发扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if( m_pITableFrame->GetTableUserItem(i) != NULL )
		{
			m_cbLeftCardCount-=(MAX_COUNT-1);
			m_GameLogic.SwitchToCardIndex(&pRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
		}
	}
}

void CTableFrameSink::GameStart_ZZ()
{
	//混乱扑克
	LONG lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),MAKEWORD(rand()%6+1,rand()%6+1));

	//设置变量
	m_wProvideUser=INVALID_CHAIR;

	
	m_wCurrentUser=m_wBankerUser;

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.lSiceCount=lSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.cbLeftCardCount = m_cbLeftCardCount;

	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);
	GameStart.cbXiaoHuTag = 0;

	bool bHasHu = false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//胡牌判断
		CChiHuRight chr;
		m_kUserActionStates1.m_cbUserAction[i] |= AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],m_cbWeaveItemCount[i],0,chr);
		if (m_kUserActionStates1.m_cbUserAction[i]&WIK_CHI_HU && !m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
		{
			bHasHu = true;
			m_wCurrentUser=i;
		}
		else
		{
			m_kUserActionStates1.m_cbUserAction[i] = WIK_NULL;
		}
	}


	//发送数据
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置变量
		GameStart.cbUserAction = WIK_NULL;//m_cbUserAction[i];

		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		if( m_pITableFrame->GetTableUserItem(i)->IsAndroidUser() )
		{
			BYTE bIndex = 1;
			for( WORD j=0; j<GAME_PLAYER; j++ )
			{
				if( j == i ) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j],&GameStart.cbCardData[MAX_COUNT*bIndex++]);
			}

			CMD_S_AndroidRule kAndroidRule;
			kAndroidRule.cbGameTypeIdex = m_cbGameTypeIdex;
			kAndroidRule.dwGameRuleIdex = m_dwGameRuleIdex;

			m_pITableFrame->SendTableData(i,SUB_S_ANDROID_RULE,&kAndroidRule,sizeof(kAndroidRule));
			m_pITableFrame->SendLookonData(i,SUB_S_ANDROID_RULE,&kAndroidRule,sizeof(kAndroidRule));

		}

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	if (hasRule(GAME_TYPE_ZZ_HONGZHONG)&& isHZGuiZe() && bHasHu)
	{
		SendOperateNotify();
	}
	else
	{
		starGameRecord();
		sendMasterHandCard();
		DispatchCardData(m_wCurrentUser);
	}

}
void CTableFrameSink::GameStart_CS()
{
	//混乱扑克
	LONG lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),MAKEWORD(rand()%6+1,rand()%6+1));

	//设置变量
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.lSiceCount=lSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.cbLeftCardCount = m_cbLeftCardCount;

	bool bInXaoHu = false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//胡牌判断
		CChiHuRight chr;
		m_kUserActionStates1.m_cbUserAction[i] |= AnalyseChiHuCardCS_XIAOHU(m_cbCardIndex[i],chr);
		if (m_kUserActionStates1.m_cbUserAction[i] != WIK_NULL && !m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
		{
			bInXaoHu = true;
		}
		else
		{
			m_kUserActionStates1.m_cbUserAction[i] = WIK_NULL;
		}
	}

	if (bInXaoHu)
	{
		m_pITableFrame->SetGameStatus(GS_MJ_XIAOHU);
		GameStart.cbXiaoHuTag =1;
	}
	else
	{
		m_pITableFrame->SetGameStatus(GS_MJ_PLAY);
		GameStart.cbXiaoHuTag = 0;
	}

	//发送数据
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置变量
		GameStart.cbUserAction = WIK_NULL;//m_cbUserAction[i];

		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		if( m_pITableFrame->GetTableUserItem(i)->IsAndroidUser() )
		{
			BYTE bIndex = 1;
			for( WORD j=0; j<GAME_PLAYER; j++ )
			{
				if( j == i ) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j],&GameStart.cbCardData[MAX_COUNT*bIndex++]);
			}
		}

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	sendMasterHandCard();
	if (GameStart.cbXiaoHuTag == 1)
	{
		SendOperateNotify();
	}
	else
	{
		starGameRecord();
		DispatchCardData(m_wCurrentUser);
	}
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//Test
			//if (hasRule(GAME_TYPE_CS_HUPAIZHUANG))
			//{
			//	m_wBankerUser = wChairID;
			//}
			//qiHuPlayerVector.clear();

			isQiPaiPlayer0 = false;
			isQiPaiPlayer1 = false;
			isQiPaiPlayer2 = false;
			isQiPaiPlayer3 = false;

			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = INVALID_CHAIR;

			//设置中鸟数据
			ZeroMemory(GameEnd.cbCardDataNiao,sizeof(GameEnd.cbCardDataNiao));
			GameEnd.cbNiaoPick = m_cbNiaoPick;
			GameEnd.cbNiaoCount = m_cbNiaoCount;
			GameEnd.cbFanShu = m_FanShu;

			// 日志输出 ！！！！！！！！！！！！！！！！！！！！！！！
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("end => m_FanShu %d"), m_FanShu);
			CTraceService::TraceString(szDescribe,TraceLevel_Normal);


			ASSERT(m_cbNiaoCount < MAX_COUNT);
			for (int i = 0;i<MAX_NIAO_CARD && i< m_cbNiaoCount&& m_cbNiaoCount < MAX_COUNT;i++)
			{
				GameEnd.cbCardDataNiao[i] = m_cbCardDataNiao[i];
			}
			
			//结束信息
			WORD wWinner = INVALID_CHAIR;
			BYTE cbLeftUserCount = 0;			//判断是否流局
			bool bUserStatus[GAME_PLAYER];		//
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbChiHuCardData[i][0] = m_cbChiHuCardData[i][0];
				GameEnd.cbChiHuCardData[i][1] = m_cbChiHuCardData[i][1];
				GameEnd.cbChairIDNiaoCout[i] = m_cbVaildChairIDNiaoCout[i];
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
				m_kUserActionStates1.m_ChiHuRight[i].GetRightData( &GameEnd.dwChiHuRight1[i],MAX_RIGHT_COUNT );
				m_kUserActionStates2.m_ChiHuRight[i].GetRightData( &GameEnd.dwChiHuRight2[i],MAX_RIGHT_COUNT );
				m_StartHuRight[i].GetRightData( &GameEnd.dwStartHuRight[i],MAX_RIGHT_COUNT );
				
				//流局玩家数
				if( m_kUserActionStates1.m_ChiHuRight[i].IsEmpty() &&  m_kUserActionStates2.m_ChiHuRight[i].IsEmpty() ) 
				{
					cbLeftUserCount++;
				}
				//当前玩家状态
				if( NULL != m_pITableFrame->GetTableUserItem(i) ) bUserStatus[i] = true;
				else bUserStatus[i] = false;
			}

			//游戏结束
			isGameOver = true;

			
			LONGLONG lGangScore[GAME_PLAYER];
			ZeroMemory(&lGangScore,sizeof(lGangScore));
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				for( BYTE j = 0; j < m_GangScore[i].cbGangCount; j++ )
				{
					for( WORD k = 0; k < GAME_PLAYER; k++ )
						lGangScore[k] += m_GangScore[i].lScore[j][k];
				}
			}

			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				m_lGameScore[i] += lGangScore[i];
				m_lGameScore[i] += m_lStartHuScore[i];

			}

			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			int	lGameTaxs[GAME_PLAYER];				//
			ZeroMemory(&lGameTaxs,sizeof(lGameTaxs));
			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if( NULL == m_pITableFrame->GetTableUserItem(i) ) continue;

				//设置积分
				if (m_lGameScore[i]>0L)
				{
					lGameTaxs[i] = m_pITableFrame->CalculateRevenue(i,m_lGameScore[i]);
					m_lGameScore[i] -= lGameTaxs[i];
				}
				
				BYTE ScoreKind;
				if( m_lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( m_lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;

				ScoreInfoArray[i].lScore   = m_lGameScore[i];
				ScoreInfoArray[i].lRevenue = lGameTaxs[i];
				ScoreInfoArray[i].cbType   = ScoreKind;

			}

			//写入积分
			datastream kDataStream;
			m_kGameRecord.StreamValue(kDataStream,true);
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray),kDataStream);

			CopyMemory( GameEnd.lGameScore,m_lGameScore,sizeof(GameEnd.lGameScore) );
			CopyMemory( GameEnd.lGangScore,lGangScore,sizeof(GameEnd.lGangScore) );
			CopyMemory( GameEnd.wProvideUser,m_wProvider,sizeof(GameEnd.wProvideUser) );
			CopyMemory( GameEnd.lGameTax,lGameTaxs,sizeof(GameEnd.lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				CopyMemory( GameEnd.wLostFanShu[i],m_wLostFanShu[i],sizeof(m_wLostFanShu[i]) );
				CopyMemory(GameEnd.WeaveItemArray[i],m_WeaveItemArray[i],sizeof(m_WeaveItemArray[i]));
			}
			CopyMemory( GameEnd.lStartHuScore,m_lStartHuScore,sizeof(GameEnd.lStartHuScore) );

			

			//组合扑克
			CopyMemory(GameEnd.cbWeaveCount,m_cbWeaveItemCount,sizeof(GameEnd.cbWeaveCount));

			//发送结束信息
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd) );

			CMD_S_GameEnd_LastCard kNetLastCard;
			kNetLastCard.cbCardCout = m_cbLeftCardCount;
			CopyMemory(kNetLastCard.cbCardData,m_cbRepertoryCard,m_cbLeftCardCount);
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GAME_END_LAST_CARD,&kNetLastCard,sizeof(kNetLastCard));
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GAME_END_LAST_CARD,&kNetLastCard,sizeof(kNetLastCard));

			//结束游戏
			m_pITableFrame->ConcludeGame(GS_MJ_FREE);

			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//结束游戏
			m_pITableFrame->ConcludeGame(GS_MJ_FREE);

			return true;
		}
	case GER_NETWORK_ERROR:		//网络错误
	case GER_USER_LEAVE:		//用户强退
		{
			m_pITableFrame->ConcludeGame(GS_MJ_FREE);
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);
	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_XIAOHU:
	case GS_MJ_PLAY:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;

			//状态变量
			StatusPlay.cbActionCard=m_kUserActionStates1.m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_kUserActionStates1.m_bResponse[wChiarID]==false)?m_kUserActionStates1.m_cbUserAction[wChiarID]:WIK_NULL;

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_kUserActionStates1.m_cbOutCardData;
			for (int i = 0;i<GAME_PLAYER;i++)
			{
				CopyMemory(StatusPlay.cbDiscardCard[i],m_cbDiscardCard[i],sizeof(BYTE)*60);
			}
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
			CopyMemory(StatusPlay.bHasCSGang,m_bHasCSGang,sizeof(StatusPlay.bHasCSGang));
			
			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			StatusPlay.cbSendCardData=((m_kUserActionStates1.m_cbSendCardData!=0)&&(m_wProvideUser==wChiarID))?m_kUserActionStates1.m_cbSendCardData:0x00;

			//发送场景
			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
			SendOperateNotify(wChiarID);
			sendMasterHandCard();
			if (m_wHaiDiUser != INVALID_CHAIR)
			{
				SendUseHaiDi(m_wHaiDiUser);
			}
			if (m_bGangStatus_CS && !m_bGangOutStatus && m_wCSGangUser == m_wCurrentUser)
			{
				CMD_S_SendCard_CSGang kCSNetInfo;
				kCSNetInfo.wCurrentUser=m_wCSGangUser;
				kCSNetInfo.cbActionMask |= m_kUserActionStates1.m_cbUserAction[m_wCurrentUser];
				kCSNetInfo.cbActionMask |= m_kUserActionStates2.m_cbUserAction[m_wCurrentUser];
				kCSNetInfo.cbCardData1 = m_kUserActionStates1.m_cbSendCardData;
				kCSNetInfo.cbCardData2 = m_kUserActionStates2.m_cbSendCardData;
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD_CSGANG,&kCSNetInfo,sizeof(kCSNetInfo));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD_CSGANG,&kCSNetInfo,sizeof(kCSNetInfo));
			}
			return true;
		}
	}

	return false;
}
bool CTableFrameSink::isUseTuoGuan()
{
	if (m_pITableFrame->GetGameServiceOption()->wServerType == GAME_GENRE_EDUCATE)
	{
		return false;
	}
	return true;
}
void CTableFrameSink::setActionTimeStart(bool bStart,bool Trustee)
{
	if (!isUseTuoGuan())
	{
		return;
	}
	int iTime = TIME_PLAYER_ACTION;
	if (Trustee)
	{
		iTime = TIME_PLAYER_ACTION_TRUSTEE;
	}
	if (bStart)
	{
		m_pITableFrame->SetGameTimer(IDI_TIMER_PLAYER_ACTION,iTime*1000,1,0);
	}
	else
	{
		m_pITableFrame->KillGameTimer(IDI_TIMER_PLAYER_ACTION);
	}
}
//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_TIMER_XIAO_HU:  //小胡结束
		{
			m_pITableFrame->KillGameTimer(IDI_TIMER_XIAO_HU);
			clearUserActionStates();
			m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

			starGameRecord();
			DispatchCardData(m_wCurrentUser);
			return true;
		}
	case IDI_TIMER_HAIDI_ACTION:  //海底捞
		{
			m_pITableFrame->KillGameTimer(IDI_TIMER_HAIDI_ACTION);
			OnUserOutCard(m_wCurrentUser,m_cbHaiDiSendCardData);
			m_cbHaiDiSendCardData = 0;
			return true;
		}
	case IDI_TIMER_CS_GANG_ACTION:  //长沙杠
		{
			m_pITableFrame->KillGameTimer(IDI_TIMER_XIAO_HU);
			WORD wChairID = m_wCurrentUser;
			m_wCurrentUser=(m_wCurrentUser+GAME_PLAYER-1)%GAME_PLAYER;
			bool bAroseAction = EstimateUserRespondCSGang(wChairID,m_kUserActionStates1.m_cbSendCardData,m_kUserActionStates2.m_cbSendCardData);
			if (!bAroseAction)
			{
				OnUserOutCardCSGang(m_wCSGangUser,m_kUserActionStates1.m_cbSendCardData,m_kUserActionStates2.m_cbSendCardData);
			}
			return true;
		}
	case IDI_TIMER_PLAYER_ACTION:  //玩家一轮
		{
			m_pITableFrame->KillGameTimer(IDI_TIMER_XIAO_HU);
			if (m_wHaiDiUser != INVALID_CHAIR)
			{
				OnUseHaiDiCard(m_wHaiDiUser,0);
				return true;
			}
			if (m_wCurrentUser == INVALID_CHAIR)
			{
				for (WORD i = 0;i<GAME_PLAYER;i++)
				{
					if (m_kUserActionStates1.m_cbUserAction[i] == WIK_NULL)
					{
						continue;
					}
					setChairTrustee(i,true);
					OnUserOperateCard(i,WIK_NULL,0,0);
				}
				for (WORD i = 0;i<GAME_PLAYER;i++)
				{
					if (m_kUserActionStates2.m_cbUserAction[i] == WIK_NULL)
					{
						continue;
					}
					setChairTrustee(i,true);
					OnUserOperateCard(i,WIK_NULL,0,1);
				}
			}
			else
			{
				setChairTrustee(m_wCurrentUser,true);
				std::vector<BYTE> kCard;
				BYTE cbActSendCard = INVALID_BYTE;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[m_wCurrentUser],kCard);
				if (kCard.size() == 0)
				{
					OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
					return true;
				}
				for (int i = 0;i<(int)kCard.size();i++)
				{
					if (kCard[i] == m_kUserActionStates1.m_cbSendCardData)
					{
						cbActSendCard = m_kUserActionStates1.m_cbSendCardData;
					}
				}
				if (cbActSendCard == INVALID_BYTE)
				{
					cbActSendCard = kCard[0];
				}
				OnUserOutCard(m_wCurrentUser,cbActSendCard);
			}

			return true;
		}
	}
	return false;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID* pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;
			m_FanShu = 0;
			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			if (m_cbHaiDiSendCardData != 0)
			{
				return true;
			}
			if (m_bHasCSGang[pIServerUserItem->GetChairID()])
			{
				return true;
			}
			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
		}
	case SUB_C_OUT_CARD_CSGANG:
		{
			//效验消息
			return true;
		}
	case SUB_C_USE_HAIDI:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_Use_HaiDi));
			if (wDataSize!=sizeof(CMD_C_Use_HaiDi)) return false;
			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			if (m_wHaiDiUser != pIServerUserItem->GetChairID())
			{
				return true;
			}
			//消息处理
			CMD_C_Use_HaiDi * pUseHaiDi=(CMD_C_Use_HaiDi *)pDataBuffer;
			OnUseHaiDiCard(m_wHaiDiUser,pUseHaiDi->cbUseHaiDi);
			return true;
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard,pOperateCard->cbCSGangIdex);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			setChairTrustee(pIServerUserItem->GetChairID(),pTrustee->bTrustee);

			return true;
		}
	case SUB_C_XIAOHU:
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_XiaoHu));
			if (wDataSize!=sizeof(CMD_C_XiaoHu)) return false;
			//消息处理
			CMD_C_XiaoHu * pXiaoHu=(CMD_C_XiaoHu *)pDataBuffer;

			if (m_pITableFrame->GetGameStatus() != GS_MJ_XIAOHU)
			{
				return true;
			}
			WORD wChairID = pIServerUserItem->GetChairID();
			OnUserXiaoHu(wChairID,pXiaoHu->cbOperateCode);
			return true;
		}
	case SUB_C_MASTER_LEFTCARD:
		{
			WORD wChairID = pIServerUserItem->GetChairID();
			sendMasterLeftCard(wChairID);
			return true;
		}
	case SUB_C_MASTER_CHEAKCARD:
		{

			MaterCheckCard *pMaterCheckCard =(MaterCheckCard *)pDataBuffer;
			if(wDataSize != sizeof(MaterCheckCard)) return false;

			WORD wChairID = pIServerUserItem->GetChairID();
			m_cbMasterCheckCard[wChairID] = pMaterCheckCard->cbCheakCard;
			return true;
		}
	case SUB_C_MASTER_ZHANIAO:
		{

			MaterNiaoCout *pMaterCheckCard =(MaterNiaoCout *)pDataBuffer;
			if(wDataSize != sizeof(MaterNiaoCout)) return false;

			BYTE cbNiaoCout = pMaterCheckCard->cbNiaoCout;
			if (cbNiaoCout > GetNiaoCardNum())
			{
				cbNiaoCout = GetNiaoCardNum();
			}
			WORD wChairID = pIServerUserItem->GetChairID();
			m_cbMasterZhaNiao[wChairID] = cbNiaoCout;


			// 日志输出 ！！！！！！！！！！！！！！！！！！！！！！！
			//TCHAR szDescribe[128]=TEXT("");
			//_sntprintf(szDescribe,CountArray(szDescribe),TEXT("OnGameMessage %d %d %d %d"),m_cbMasterZhaNiao[0],m_cbMasterZhaNiao[1],m_cbMasterZhaNiao[2],m_cbMasterZhaNiao[3]);
			//CTraceService::TraceString(szDescribe,TraceLevel_Normal);
			return true;
		}
	}

	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,true);
	}
	return true;
}
//用户重入
bool CTableFrameSink::OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
	{
		setChairTrustee(wChairID,false);
	}
	return true;
}
//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if (bLookonUser==false)
	{
	}

	return true;
}
void CTableFrameSink::SetPrivateInfo(BYTE bGameTypeIdex,DWORD	bGameRuleIdex)
{
	m_cbGameTypeIdex = bGameTypeIdex;
	m_dwGameRuleIdex = bGameRuleIdex;
}

void CTableFrameSink::SetCreateUserID(DWORD dwUserID)
{
	IServerUserItem* pUserItem = m_pITableFrame->SearchUserItem(dwUserID);

	//if (pUserItem)
	//{
	//	m_wBankerUser = pUserItem->GetChairID();
	//}

	m_iGameCount = 0;
	if (m_wBankerUser == INVALID_CHAIR)
	{
		m_wBankerUser = pUserItem->GetChairID();
	}
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//错误断言
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wCurrentUser) return true;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return true;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return true;
	}

	/************************************************************************/
	/* 添加冲锋鸡记录                  2017年3月17日20:52:31                */
	/************************************************************************/
	if (0x11 == cbCardData)
	{
		++m_cbChichCount;
		if (-1 != m_dFirstChickID || 0 == m_cbChichCount)
		{
			m_dFirstChickID = wChairID;
		}
		m_cbChickID[m_cbChichCount] = wChairID;
	}

	//设置变量
	if( m_bGangStatus != STATE_NULL)
	{
		if (m_bGangStatus == STATE_GANG)
		{
			m_bGangOutStatus = true;
		}
	}
	m_bGangStatus = STATE_NULL;
	m_bGangStatus_CS = false;
	clearUserActionStates(wChairID);

	//出牌记录
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_kUserActionStates1.m_cbOutCardData=cbCardData;
	m_kUserActionStates2.m_cbOutCardData=0;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;
	addGameRecordAction(OutCard);

	/************************************************************************/
	/*   注释掉红中玩法                  2017年3月17日21:02:53              */
	/************************************************************************/
	/***********************************************************************
	//出红中视为弃胡
	if (OutCard.cbOutCardData == 0x35)
	{
		for (int index = 0; index < 4; index++)
		{
			if (index == m_wCurrentUser)
			{
				if (index == 0)
				{
					isQiPaiPlayer0 = true;
				}

				if (index == 1)
				{
					isQiPaiPlayer1 = true;
				}

				if (index == 2)
				{
					isQiPaiPlayer2 = true;
				}

				if (index == 3)
				{
					isQiPaiPlayer3 = true;
				}
			}
		}
		 //qiHuPlayerVector.push_back(m_wCurrentUser);
	}
	***************************************************************************/

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_kUserActionStates1.m_cbProvideCard=cbCardData;

	//------------------------
	//听牌判断
	bool m_bool = m_GameLogic.IsTingCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
		m_cbWeaveItemCount[m_wCurrentUser]);
	CMD_S_TingCard TingCard;
	TingCard.wTingCardUser = m_wCurrentUser;
	TingCard.isTingCard = m_bool;
	if (m_GameLogic.getTingCardVector().size() != 0)
	{
		//TingCard.tingCardVector = m_GameLogic.getTingCardVector();
	}
	

	//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LISTEN_CARD,&TingCard,sizeof(TingCard));
	//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN_CARD,&TingCard,sizeof(TingCard));

	//-------------------------

	//用户切换
	m_wCurrentUser=(wChairID+GAME_PLAYER-1)%GAME_PLAYER;

	//响应判断
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//派发扑克
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}
bool CTableFrameSink::OnUserOutCardCSGang(WORD wChairID, BYTE cbCardData1,BYTE cbCardData2,bool bNext)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	m_bGangStatus_CS = false;
	m_bGangOutStatus = false;

	//构造数据
	CMD_S_OutCard_CSGang OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData1=cbCardData1;
	OutCard.cbOutCardData2=cbCardData2;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD_CSGANG,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD_CSGANG,&OutCard,sizeof(OutCard));

	//用户切换
	if (bNext)
	{
		m_wCurrentUser=(wChairID+GAME_PLAYER-1)%GAME_PLAYER;
		DispatchCardData(m_wCurrentUser);
	}
	return true;
}
bool CTableFrameSink::OnUseHaiDiCard(WORD wChairID, BYTE cbUseHaiDi)
{
	if (cbUseHaiDi)
	{
		if (DispatchCardData(wChairID,true))
		{
			return true;
		}
	}
	m_wHaiDiCout++;
	if (m_wHaiDiCout >= 4)
	{
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
	}
	else
	{
		m_wHaiDiUser=(m_wHaiDiUser+GAME_PLAYER-1)%GAME_PLAYER;
		SendUseHaiDi(m_wHaiDiUser);
	}
	return true;
}
bool CTableFrameSink::CanQiangGangHu()
{
	if (m_cbGameTypeIdex == GAME_TYPE_ZZ)
	{
		return hasRule(GAME_TYPE_ZZ_QIANGGANGHU);
	}
	return true;
}
bool CTableFrameSink::OnUserOperateCardBeiDong(WORD wChairID,DWORD cbOperateCode,UserActionStates* pUserActionStates)
{
	//效验状态
	if (pUserActionStates->m_bResponse[wChairID]==true) 
		return true;
	if ((cbOperateCode!=WIK_NULL)&&((pUserActionStates->m_cbUserAction[wChairID]&cbOperateCode)==0))
		return true;

	//变量定义
	WORD wTargetUser=wChairID;
	DWORD cbTargetAction=cbOperateCode;
	UserActionStates* pTargetUserActionState = NULL;

	//设置变量
	pUserActionStates->m_bResponse[wChairID]=true;
	pUserActionStates->m_cbPerformAction[wChairID]=cbOperateCode;
	pUserActionStates->m_cbOperateCard[wChairID]=pUserActionStates->m_cbProvideCard;

	if (cbTargetAction==WIK_NULL && FvMask::HasAny(pUserActionStates->m_cbUserAction[wChairID],WIK_CHI_HU))
	{
		m_cbCanHu[wChairID] = 1;
	}
	//执行判断
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取动作
		DWORD cbUserAction=(m_kUserActionStates1.m_bResponse[i]==false)?
			m_kUserActionStates1.m_cbUserAction[i]:m_kUserActionStates1.m_cbPerformAction[i];

		//优先级别
		BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
		BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

		//动作判断
		if (cbUserActionRank>cbTargetActionRank)
		{
			wTargetUser=i;
			cbTargetAction=cbUserAction;
			pTargetUserActionState = &m_kUserActionStates1;
		}
	}
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取动作
		DWORD cbUserAction=(m_kUserActionStates2.m_bResponse[i]==false)?
			m_kUserActionStates2.m_cbUserAction[i]:m_kUserActionStates2.m_cbPerformAction[i];

		//优先级别
		BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
		BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

		//动作判断
		if (cbUserActionRank>cbTargetActionRank)
		{
			wTargetUser=i;
			cbTargetAction=cbUserAction;
			pTargetUserActionState = &m_kUserActionStates2;
		}
	}
	if (pTargetUserActionState && pTargetUserActionState->m_bResponse[wTargetUser]==false) 
		return true;

	//吃胡等待
	if (cbTargetAction==WIK_CHI_HU)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if ((m_kUserActionStates1.m_bResponse[i]==false)&&(m_kUserActionStates1.m_cbUserAction[i]&WIK_CHI_HU))
				return true;
		}
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if ((m_kUserActionStates2.m_bResponse[i]==false)&&(m_kUserActionStates2.m_cbUserAction[i]&WIK_CHI_HU))
				return true;
		}
	}
	//放弃操作
	if (cbTargetAction==WIK_NULL)
	{
		//用户状态
		clearUserActionStates();

		DispatchCardData(m_wResumeUser);
		return true;
	}


	//变量定义
	BYTE cbTargetCard=pUserActionStates->m_cbOperateCard[wTargetUser];

	//出牌变量
	m_kUserActionStates1.m_cbOutCardData=0;
	m_kUserActionStates2.m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;

	DWORD dwChiHuKind1[GAME_PLAYER];
	DWORD dwChiHuKind2[GAME_PLAYER];
	ZeroMemory(dwChiHuKind1,sizeof(dwChiHuKind1));
	ZeroMemory(dwChiHuKind2,sizeof(dwChiHuKind2));

	/************************************************************************/
	/* 添加  责任鸡                                                         */
	/************************************************************************/
	if (0x11 == cbTargetCard)
	{
	}
	if (m_cbChichCount == 0)
	{
		if (m_dFirstChickID != -1)
		{
			//责任鸡
			//...
		}
	}
	//胡牌操作
	if (cbTargetAction==WIK_CHI_HU)
	{
		int nChiHuCount = 0;
		for (WORD m = 0;m < 4 ;m++)
		{
			WORD iChairID = (m_wProvideUser+GAME_PLAYER-m-1)%GAME_PLAYER;
			//过虑判断
			bool bHu = false;
			BYTE cbWeaveItemCount=m_cbWeaveItemCount[iChairID];
			tagWeaveItem * pWeaveItem=m_WeaveItemArray[iChairID];
			if ((m_kUserActionStates1.m_cbPerformAction[iChairID]&WIK_CHI_HU))
			{
				dwChiHuKind1[iChairID] = AnalyseChiHuCard(m_cbCardIndex[iChairID],pWeaveItem,cbWeaveItemCount,
					m_kUserActionStates1.m_cbOperateCard[iChairID],m_kUserActionStates1.m_ChiHuRight[iChairID]);
			}
			if ((m_kUserActionStates2.m_cbPerformAction[iChairID]&WIK_CHI_HU))
			{
				dwChiHuKind2[iChairID] = AnalyseChiHuCard(m_cbCardIndex[iChairID],pWeaveItem,cbWeaveItemCount,
					m_kUserActionStates2.m_cbOperateCard[iChairID],m_kUserActionStates2.m_ChiHuRight[iChairID]);
			}
			//插入扑克
			if (dwChiHuKind1[iChairID]!=WIK_NULL || dwChiHuKind2[iChairID]!=WIK_NULL) 
			{
				nChiHuCount ++;
				m_wNiaoDuoPaoStartChairID = iChairID;
			}
		}
		if (nChiHuCount >1)//一炮多响 放炮的为庄家
		{
			//m_wBankerUser = m_wNiaoDuoPaoStartChairID;
			m_wNiaoDuoPaoStartChairID = m_wProvideUser;
			m_wBankerUser = m_wNiaoDuoPaoStartChairID;
			isDuoPao = true;
		}

		for (WORD m = 0;m < 4 ;m++)
		{
			WORD iChairID = (m_wProvideUser+GAME_PLAYER-m-1)%GAME_PLAYER;
			//过虑判断
			//插入扑克
			int iChiHuCout = 0;
			if (dwChiHuKind1[iChairID]!=WIK_NULL && (m_kUserActionStates1.m_cbPerformAction[iChairID]&WIK_CHI_HU)) 
			{
				ProcessChiHuUser( iChairID,m_kUserActionStates1.m_ChiHuRight[iChairID],m_kUserActionStates1.m_cbProvideCard);
				m_cbChiHuCardData[iChairID][iChiHuCout] = m_kUserActionStates1.m_cbProvideCard;
				iChiHuCout++;

				if (nChiHuCount == 1)
				{
					m_wBankerUser = iChairID;
				}
			}
			if (dwChiHuKind2[iChairID]!=WIK_NULL && (m_kUserActionStates2.m_cbPerformAction[iChairID]&WIK_CHI_HU)) 
			{
				ProcessChiHuUser( iChairID,m_kUserActionStates2.m_ChiHuRight[iChairID],m_kUserActionStates2.m_cbProvideCard);
				m_cbChiHuCardData[iChairID][iChiHuCout] = m_kUserActionStates2.m_cbProvideCard;
				iChiHuCout++;

				if (nChiHuCount == 1)
				{
					m_wBankerUser = iChairID;
				}
			}

		}
		//如果是抢杠胡
		if(m_wCurrentUser == INVALID_CHAIR && m_bGangStatus && m_GangScore[m_wProvideUser].cbGangCount > 0)
		{
			m_GangScore[m_wProvideUser].cbGangCount--;
			int iGangCout = m_GangScore[m_wProvideUser].cbGangCount;
			for (int i = 0;i<GAME_PLAYER;i++)
			{
				m_GangScore[m_wProvideUser].lScore[iGangCout][i] = 0;
			}

			if (hasRule(GAME_TYPE_ZZ_HONGZHONG))
			{
				m_cbNiaoCount = 0;
			}
			
		}
		//if (nChiHuCount >1)//一炮多响 放炮的为庄家
		//{
		//	m_wBankerUser = m_wProvideUser;
		//	//m_wBankerUser = m_wNiaoDuoPaoStartChairID;
		//}

		OnEventGameConclude( INVALID_CHAIR,NULL,GER_NORMAL );

		return true;
	}

	if (m_bGangStatus_CS)
	{
		OnUserOutCardCSGang(m_wCSGangUser,m_kUserActionStates1.m_cbSendCardData,m_kUserActionStates2.m_cbSendCardData,false);
	}

	//用户状态
	clearUserActionStates();

	//组合扑克
	ASSERT(m_cbWeaveItemCount[wTargetUser]<4);
	WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
	m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
	m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
	m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
	m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

	//删除扑克
	switch (cbTargetAction)
	{
	case WIK_LEFT:		//上牌操作
		{
			//删除扑克
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_LEFT,cbTargetCard,cbRemoveCard);
			VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

			break;
		}
	case WIK_RIGHT:		//上牌操作
		{
			//删除扑克
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_RIGHT,cbTargetCard,cbRemoveCard);
			VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

			break;
		}
	case WIK_CENTER:	//上牌操作
		{
			//删除扑克
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_CENTER,cbTargetCard,cbRemoveCard);
			VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

			break;
		}
	case WIK_PENG:		//碰牌操作
		{
			//删除扑克
			BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );

			break;
		}
	case WIK_CS_GANG:
	case WIK_BU_ZHANG:
	case WIK_GANG:		//杠牌操作
		{
			//删除扑克,被动动作只存在放杠
			BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) );

			break;
		}
	default:
		ASSERT( FALSE );
		return false;
	}

	//构造结果
	CMD_S_OperateResult OperateResult;
	OperateResult.wOperateUser=wTargetUser;
	OperateResult.cbOperateCard=cbTargetCard;
	OperateResult.cbOperateCode=cbTargetAction;
	OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
	addGameRecordAction(OperateResult);
	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

	//设置用户
	m_wCurrentUser=wTargetUser;

	//杠牌处理
	if (cbTargetAction==WIK_GANG)
	{
		ASSERT(m_cbGameTypeIdex != GAME_TYPE_CS);

		SCORE  nActionScore = m_pGameServiceOption->lCellScore*GANG_FEN;
		//发送信息
		CMD_S_GangScore gs;
		ZeroMemory( &gs,sizeof(gs) );
		gs.wChairId = wTargetUser;
		gs.cbXiaYu = FALSE;
		gs.lGangScore[wTargetUser] = nActionScore;
		gs.lGangScore[m_wProvideUser] = -nActionScore;
		m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
		m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );

		//杠得分
		BYTE cbIndex = m_GangScore[wTargetUser].cbGangCount++;
		m_GangScore[wTargetUser].lScore[cbIndex][wTargetUser] = nActionScore;
		m_GangScore[wTargetUser].lScore[cbIndex][m_wProvideUser] = -nActionScore;

		m_bGangStatus = STATE_GANG;
		DispatchCardData(wTargetUser);
	}
	else if (cbTargetAction == WIK_CS_GANG)
	{
		ASSERT(m_cbGameTypeIdex == GAME_TYPE_CS);

		m_bHasCSGang[wTargetUser] = true;
		m_bGangStatus = STATE_GANG;
		DispatchCardData_CSGang(wTargetUser);
	}
	else if (cbTargetAction == WIK_BU_ZHANG)
	{
		ASSERT(m_cbGameTypeIdex == GAME_TYPE_CS);

		m_bGangStatus = STATE_BU;
		DispatchCardData(wTargetUser);
	}
	else
	{
		if (hasRule(GAME_TYPE_ZZ_258))
		NoticeGangState(m_wCurrentUser,true);
	}
	return true;
}
//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, DWORD cbOperateCode, BYTE cbOperateCard, BYTE cbCSGangIdex)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()!=GS_MJ_FREE);
	if (m_pITableFrame->GetGameStatus()==GS_MJ_FREE)
		return true;

	//效验用户	注意：机器人有可能发生此断言
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return true;
	UserActionStates* pUserActionState = NULL;
	if (cbCSGangIdex == 0)
	{
		pUserActionState = &m_kUserActionStates1;
	}
	else
	{
		pUserActionState = &m_kUserActionStates2;
	}
	//被动动作
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		OnUserOperateCardBeiDong(wChairID,cbOperateCode,pUserActionState);

		if (cbOperateCode == WIK_NULL && m_kUserActionStates2.m_cbUserAction[wChairID] != WIK_NULL)
		{
			OnUserOperateCardBeiDong(wChairID,cbOperateCode,&m_kUserActionStates2);
		}
		return true;
	}

	//主动动作
	if (m_wCurrentUser==wChairID)
	{
		if (m_bGangStatus_CS && cbOperateCode==WIK_NULL)
		{
			bool bAroseAction = EstimateUserRespondCSGang(wChairID,m_kUserActionStates1.m_cbSendCardData,m_kUserActionStates2.m_cbSendCardData);
			if (!bAroseAction)
			{
				OnUserOutCardCSGang(m_wCSGangUser,m_kUserActionStates1.m_cbSendCardData,m_kUserActionStates2.m_cbSendCardData);
			}
			return true;
		}
		if (cbOperateCode==WIK_NULL && m_bHasCSGang[wChairID] && m_cbGameTypeIdex == GAME_TYPE_CS)
		{
			int iCardCout = m_GameLogic.GetCardCount(m_cbCardIndex[wChairID]);
			if (iCardCout%3 ==2)
			{
				OnUserOutCard(wChairID,pUserActionState->m_cbSendCardData);
			}
			return true;
		}
		//效验操作
		if ((cbOperateCode==WIK_NULL)||
			(
				(m_kUserActionStates1.m_cbUserAction[wChairID]&cbOperateCode)==0 
				&& ((m_kUserActionStates2.m_cbUserAction[wChairID]&cbOperateCode)==0))
			)
		{
			return true;
		}

		//扑克效验
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode==WIK_GANG)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) 
		{
			return true;
		}

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_GANG:			//杠牌操作
			{
				clearUserActionStates();
				ASSERT(m_cbGameTypeIdex != GAME_TYPE_CS);
				bool bPublic = false;
				if (!checkSelfGang(wChairID,cbOperateCard,bPublic))
				{
					return false;
				}
				m_bGangStatus = STATE_GANG;

				//杠牌得分
				LONGLONG lScore = bPublic?m_pGameServiceOption->lCellScore*ZIMO_GANG_FEN :m_pGameServiceOption->lCellScore * ANGANG_FEN;
				BYTE cbGangIndex = m_GangScore[wChairID].cbGangCount++;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if(  i == wChairID ) continue;

					m_GangScore[wChairID].lScore[cbGangIndex][i] = -lScore;
					m_GangScore[wChairID].lScore[cbGangIndex][wChairID] += lScore;
				}

				//发送信息
				CMD_S_GangScore gs;
				gs.wChairId = wChairID;
				ZeroMemory( &gs,sizeof(gs) );
				gs.cbXiaYu = bPublic?FALSE:TRUE;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( i == wChairID ) continue;

					gs.lGangScore[i] = -lScore;
					gs.lGangScore[wChairID] += lScore;
				}
				m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
				m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GANG_SCORE,&gs,sizeof(gs) );
				
				//构造结果
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;
				addGameRecordAction(OperateResult);

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//效验动作
				bool bAroseAction=false;
				if (bPublic==true && CanQiangGangHu())
				{
					bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);
				}

				//发送扑克
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID);
				}
				return true;
			}
		case WIK_CHI_HU:		//吃胡操作
			{
				//吃牌权位
				if (m_cbOutCardCount==0)
				{
					m_wProvideUser = m_wCurrentUser;
					m_kUserActionStates1.m_cbProvideCard = m_kUserActionStates1.m_cbSendCardData;
					m_kUserActionStates2.m_cbProvideCard = m_kUserActionStates2.m_cbSendCardData;
				}

				//普通胡牌
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				//设置闲来鸟开始位置
				m_wNiaoDuoPaoStartChairID = wChairID;

				int iChiHuCout = 0;
				if (m_bGangStatus_CS)
				{
					if (m_kUserActionStates1.m_cbUserAction[wChairID] == WIK_CHI_HU)
					{
						m_kUserActionStates1.m_cbUserAction[wChairID] = AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,
							cbWeaveItemCount,m_kUserActionStates1.m_cbProvideCard,m_kUserActionStates1.m_ChiHuRight[wChairID]);
						ProcessChiHuUser( wChairID,m_kUserActionStates1.m_ChiHuRight[wChairID],m_kUserActionStates1.m_cbProvideCard);

						m_cbChiHuCardData[wChairID][iChiHuCout] = m_kUserActionStates1.m_cbProvideCard;
						iChiHuCout++;
					}
					if (m_kUserActionStates2.m_cbUserAction[wChairID] == WIK_CHI_HU)
					{
						m_kUserActionStates2.m_cbUserAction[wChairID] = AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,
							cbWeaveItemCount,m_kUserActionStates2.m_cbProvideCard,m_kUserActionStates2.m_ChiHuRight[wChairID]);
						ProcessChiHuUser( wChairID,m_kUserActionStates2.m_ChiHuRight[wChairID],m_kUserActionStates2.m_cbProvideCard);

						m_cbChiHuCardData[wChairID][iChiHuCout] = m_kUserActionStates2.m_cbProvideCard;
						iChiHuCout++;
					}
				}
				else
				{
					if (m_kUserActionStates1.m_cbProvideCard !=0)
					{
						m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_kUserActionStates1.m_cbProvideCard,1);
					}

					AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,
						cbWeaveItemCount,m_kUserActionStates1.m_cbProvideCard,m_kUserActionStates1.m_ChiHuRight[wChairID]);

					if (m_kUserActionStates1.m_cbProvideCard !=0)
					{
						m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_kUserActionStates1.m_cbProvideCard)]++;
					}
					ProcessChiHuUser( wChairID,m_kUserActionStates1.m_ChiHuRight[wChairID],m_kUserActionStates1.m_cbProvideCard);
					m_cbChiHuCardData[wChairID][iChiHuCout] = m_kUserActionStates1.m_cbProvideCard;
					iChiHuCout++;
				}


				m_wBankerUser = wChairID;

				
				OnEventGameConclude( INVALID_CHAIR,NULL,GER_NORMAL );

				return true;
			}
		case WIK_CS_GANG:
			{
				


				clearUserActionStates();
				ASSERT(m_cbGameTypeIdex == GAME_TYPE_CS);
				bool bPublic = false;
				if (!checkSelfGang(wChairID,cbOperateCard,bPublic))
				{
					return false;
				}
				//构造结果
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=WIK_CS_GANG;
				OperateResult.cbOperateCard=cbOperateCard;
				addGameRecordAction(OperateResult);

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				m_bHasCSGang[wChairID] = true;
				m_bGangStatus = STATE_GANG;
				//效验动作
				bool bAroseAction=false;
				if (bPublic==true && CanQiangGangHu())
				{
					bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);
				}
				if(bAroseAction==false)
				{
					DispatchCardData_CSGang(wChairID);
				}
				return true;
			}
		
		case WIK_BU_ZHANG:
			{
				clearUserActionStates();
				ASSERT(m_cbGameTypeIdex == GAME_TYPE_CS);

				bool bPublic = false;
				if (!checkSelfGang(wChairID,cbOperateCard,bPublic))
				{
					return false;
				}
				//构造结果
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=WIK_BU_ZHANG;
				OperateResult.cbOperateCard=cbOperateCard;
				addGameRecordAction(OperateResult);

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				m_bGangStatus = STATE_BU;
				//效验动作
				bool bAroseAction=false;
				if (bPublic==true && CanQiangGangHu())
				{
					bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);
				}

				//发送扑克
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID);
				}
				return true;
			}
		}
		clearUserActionStates();
		NoticeGangState(m_wCurrentUser,true);
		return true;
	}

	return true;
}

bool CTableFrameSink::checkSelfGang(WORD wChairID,BYTE cbOperateCard,bool& bPublic)
{
	//变量定义
	BYTE cbWeaveIndex=0xFF;
	BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

	//杠牌处理
	if (m_cbCardIndex[wChairID][cbCardIndex]==1)
	{
		//寻找组合
		for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
		{
			DWORD cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
			DWORD cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				bPublic=true;
				cbWeaveIndex=i;
				break;
			}
		}
		//效验动作
		ASSERT(cbWeaveIndex!=0xFF);
		if (cbWeaveIndex==0xFF) return false;

		//组合扑克
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind = WIK_GANG;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
	}
	else
	{
		//扑克效验
		ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
		if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
			return false;

		//设置变量
		bPublic=false;
		cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
		m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind= WIK_GANG;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
	}
	//删除扑克
	m_cbCardIndex[wChairID][cbCardIndex]=0;
	return true;
}

//发送操作
bool CTableFrameSink::SendOperateNotify(WORD wChairID)
{
	bool bTrustee = true;
	bool bTimeAction = false;
	//发送提示
	for (WORD i=0;i<GAME_PLAYER;i++)
	{

		if (wChairID != INVALID_CHAIR && wChairID != i)
		{
			continue;
		}
		if (m_kUserActionStates1.m_cbUserAction[i]==WIK_NULL 
			&& m_kUserActionStates2.m_cbUserAction[i]==WIK_NULL )
		{
			continue;
		}
		//构造数据
		CMD_S_OperateNotify OperateNotify;
		OperateNotify.bGangStatus_CS = m_bGangStatus_CS;
		OperateNotify.wProvideUser=m_wProvideUser;
		OperateNotify.cbActionCard1=m_kUserActionStates1.m_cbProvideCard;
		OperateNotify.cbActionMask1=m_kUserActionStates1.m_cbUserAction[i];
		OperateNotify.cbActionCard2=m_kUserActionStates2.m_cbProvideCard;
		OperateNotify.cbActionMask2=m_kUserActionStates2.m_cbUserAction[i];
		OperateNotify.cbGangCardCount=m_cbGangInfo[i].cbCardCount;
		CopyMemory( OperateNotify.cbGangCardData,m_cbGangInfo[i].cbCardData,sizeof(m_cbGangInfo[i].cbCardData) );

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		bTimeAction = true;
		if (m_bTrustee[i] == false)
		{
			bTrustee = false;
		}
	}
	if (bTimeAction)
	{
		setActionTimeStart(true,bTrustee);
	}
	return true;
}
void CTableFrameSink::NoticeGangState(WORD iChairID,bool bSendNotice)
{
	//杠牌判断
	if (getLeftCardNum()>0)
	{
		ZeroMemory(&m_cbGangInfo[iChairID],sizeof(m_cbGangInfo[iChairID]));
		m_cbGangInfo[iChairID].cbCardData[0] = m_kUserActionStates1.m_cbSendCardData;
		DWORD  cbTemp =m_GameLogic.AnalyseGangCard(m_cbCardIndex[iChairID],m_WeaveItemArray[iChairID],m_cbWeaveItemCount[iChairID],
			m_cbGangInfo[iChairID],m_cbGameTypeIdex);

		if (cbTemp == WIK_GANG && m_cbGameTypeIdex == GAME_TYPE_ZZ)
		{
			m_kUserActionStates1.m_cbUserAction[iChairID] |= WIK_GANG;
		}
		else if(cbTemp == WIK_GANG && m_cbGameTypeIdex == GAME_TYPE_CS)
		{
			m_kUserActionStates1.m_cbUserAction[iChairID] |= WIK_BU_ZHANG;

			if (getLeftCardNum() >=2 && !m_pITableFrame->GetTableUserItem(iChairID)->IsAndroidUser())
			{
				m_kUserActionStates1.m_cbUserAction[iChairID] |= WIK_CS_GANG;
			}
		}
	}
	if (bSendNotice && m_kUserActionStates1.m_cbUserAction[iChairID] != WIK_NULL)
	{
		SendOperateNotify(iChairID);
	}
}
//派发扑克
void CTableFrameSink::SendUseHaiDi(WORD wCurrentUser)
{
	CMD_S_UseHaiDi kNetInfo;
	kNetInfo.wCurrentUser = wCurrentUser;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USE_HAIDI_CARD,&kNetInfo,sizeof(kNetInfo));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USE_HAIDI_CARD,&kNetInfo,sizeof(kNetInfo));

	setActionTimeStart(true,m_bTrustee[wCurrentUser]);
}
//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bHaiDi)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_kUserActionStates1.m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_kUserActionStates1.m_cbOutCardData;
	}
	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_kUserActionStates2.m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_kUserActionStates2.m_cbOutCardData;
	}
	m_wOutCardUser=INVALID_CHAIR;
	clearUserActionStates();

	//荒庄结束
	if (getLeftCardNum()==0)
	{
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
		/************************************************************************/
		/*					荒庄    2017年3月20日10:19:49                       */
		/************************************************************************/
		m_bIsNullWin = true;
		return true;
	}else
	{
		m_bIsNullWin = false;
	}

	if (!bHaiDi && getLeftCardNum()==1 && m_cbGameTypeIdex == GAME_TYPE_CS)
	{
		m_wHaiDiUser = wCurrentUser;
		m_wCurrentUser = INVALID_CHAIR;
		SendUseHaiDi(m_wHaiDiUser);
		m_wBankerUser = m_wHaiDiUser;
		return true;
	}
	//设置变量
	m_kUserActionStates1.m_cbOutCardData=0;
	m_kUserActionStates2.m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;

	//杠后炮
	m_bGangOutStatus = false;

	//发送扑克
	m_cbSendCardCount++;
	m_kUserActionStates1.m_cbSendCardData = getSendCardData(m_wCurrentUser);
	m_kUserActionStates2.m_cbSendCardData = 0;

	//胡牌判断
	CChiHuRight chr;
	m_wProvideUser = wCurrentUser;
	//int qiHuPlayerSize = qiHuPlayerVector.size();

	m_kUserActionStates1.m_cbUserAction[wCurrentUser]|= AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
		m_cbWeaveItemCount[wCurrentUser],m_kUserActionStates1.m_cbSendCardData,chr);

	//弃胡玩家判断
	if (isQiPaiPlayer0)
	{
		m_kUserActionStates1.m_cbUserAction[0] = WIK_NULL;
	}

	if (isQiPaiPlayer1)
	{
		m_kUserActionStates1.m_cbUserAction[1] = WIK_NULL;
	}

	if (isQiPaiPlayer2)
	{
		m_kUserActionStates1.m_cbUserAction[2] = WIK_NULL;
	}

	if (isQiPaiPlayer3)
	{
		m_kUserActionStates1.m_cbUserAction[3] = WIK_NULL;
	}

	//加牌
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_kUserActionStates1.m_cbSendCardData)]++;

	//设置变量
	m_kUserActionStates1.m_cbProvideCard = m_kUserActionStates1.m_cbSendCardData;


	m_cbCanHu[wCurrentUser] = 0;
	//构造数据
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.bGangStatus_CS = false;
	SendCard.cbActionMask=m_kUserActionStates1.m_cbUserAction[wCurrentUser];
	SendCard.cbCardData= m_kUserActionStates1.m_cbSendCardData;
	addGameRecordAction(SendCard);

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//一直杠牌分析
	if (!(m_bHasCSGang[wCurrentUser] && m_cbGameTypeIdex == GAME_TYPE_CS))
	{
		NoticeGangState(wCurrentUser,true);
	}

	setActionTimeStart(true,m_bTrustee[wCurrentUser]);
	if (m_bHasCSGang[wCurrentUser] && m_cbGameTypeIdex == GAME_TYPE_CS && m_kUserActionStates1.m_cbUserAction[wCurrentUser] != WIK_CHI_HU)
	{
		OnUserOutCard(wCurrentUser,m_kUserActionStates1.m_cbSendCardData);
	}
	else if(m_kUserActionStates1.m_cbUserAction[wCurrentUser] == WIK_NULL && getLeftCardNum() == 0 && m_cbGameTypeIdex == GAME_TYPE_CS)
	{
		m_cbHaiDiSendCardData = m_kUserActionStates1.m_cbSendCardData;
		m_pITableFrame->SetGameTimer(IDI_TIMER_HAIDI_ACTION,TIME_HAIDI_ACTION*1000,1,0);
	}
	return true;
}
bool CTableFrameSink::DispatchCardData_CSGang( WORD wCurrentUser )
{
	ASSERT(m_cbGameTypeIdex == GAME_TYPE_CS);
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	ASSERT(getLeftCardNum()>=2);
	if (wCurrentUser==INVALID_CHAIR || getLeftCardNum()<2 || m_cbGameTypeIdex != GAME_TYPE_CS)
		return false;

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_kUserActionStates1.m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_kUserActionStates1.m_cbOutCardData;
	}
	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_kUserActionStates2.m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_kUserActionStates2.m_cbOutCardData;
	}
	//设置变量
	m_kUserActionStates1.m_cbOutCardData=0;
	m_kUserActionStates2.m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wCSGangUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;
	m_bGangStatus_CS = true;

	//杠后炮
	if( m_bGangOutStatus )
	{
		m_bGangOutStatus = false;
	}


	//发送扑克
	m_cbSendCardCount +=2;
	m_kUserActionStates1.m_cbSendCardData = getSendCardData(m_wCurrentUser);
	m_kUserActionStates2.m_cbSendCardData = getSendCardData(m_wCurrentUser);
	//胡牌判断
	CChiHuRight chr1,chr2;
	m_wProvideUser = wCurrentUser;

	DWORD cbAction1;
	DWORD cbAction2;
	cbAction1 = AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
		m_cbWeaveItemCount[wCurrentUser],m_kUserActionStates1.m_cbSendCardData,chr1);
	cbAction2 = AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
		m_cbWeaveItemCount[wCurrentUser],m_kUserActionStates2.m_cbSendCardData,chr2);



	////弃胡玩家判断
	//if (qiHuPlayerVector.size() != 0)
	//{

	//	for (int index = 0; index < qiHuPlayerVector.size(); index++)
	//	{
	//		if (m_wProvideUser == qiHuPlayerVector.at(index))
	//		{
	//			cbAction1 = WIK_NULL;
	//			cbAction2 = WIK_NULL;
	//		}else
	//		{
	//			cbAction1 = AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
	//				m_cbWeaveItemCount[wCurrentUser],m_kUserActionStates1.m_cbSendCardData,chr1);
	//			cbAction2 = AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
	//				m_cbWeaveItemCount[wCurrentUser],m_kUserActionStates2.m_cbSendCardData,chr2);
	//		}
	//		
	//		
	//	}
	//}else{
	//	cbAction1 = AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
	//		m_cbWeaveItemCount[wCurrentUser],m_kUserActionStates1.m_cbSendCardData,chr1);
	//	cbAction2 = AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],
	//		m_cbWeaveItemCount[wCurrentUser],m_kUserActionStates2.m_cbSendCardData,chr2);
	//}

	CMD_S_SendCard_CSGang kNetSendCard;
	ZeroMemory(&kNetSendCard,sizeof(kNetSendCard));

	if (cbAction1 != WIK_NULL)
	{
		m_kUserActionStates1.m_cbUserAction[wCurrentUser]|=cbAction1;
		m_kUserActionStates1.m_cbProvideCard = m_kUserActionStates1.m_cbSendCardData;
		kNetSendCard.cbActionMask=cbAction1;
	}
	if (cbAction2 != WIK_NULL)
	{
		m_kUserActionStates2.m_cbUserAction[wCurrentUser]|=cbAction2;
		m_kUserActionStates2.m_cbProvideCard = m_kUserActionStates2.m_cbSendCardData;
		kNetSendCard.cbActionMask=cbAction2;
	}
	if (kNetSendCard.cbActionMask == 0)
	{
		m_pITableFrame->SetGameTimer(IDI_TIMER_CS_GANG_ACTION,4*1000,1,0);
	}
	//设置变量

	m_cbCanHu[wCurrentUser] = 0;
	//构造数据
	kNetSendCard.wCurrentUser=wCurrentUser;
	kNetSendCard.cbCardData1=m_kUserActionStates1.m_cbSendCardData;
	kNetSendCard.cbCardData2=m_kUserActionStates2.m_cbSendCardData;
	//	addGameRecordAction(SendCard);
	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD_CSGANG,&kNetSendCard,sizeof(kNetSendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD_CSGANG,&kNetSendCard,sizeof(kNetSendCard));

	return true;
}
void CTableFrameSink::setChairTrustee(WORD iChairID,bool bTrustee)
{
	if (!isUseTuoGuan())
	{
		return;
	}
	if (iChairID >= GAME_PLAYER)
	{
		return;
	}
	m_bTrustee[iChairID]=bTrustee;

	CMD_S_Trustee Trustee;
	Trustee.bTrustee = bTrustee;
	Trustee.wChairID = iChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
}

void CTableFrameSink::EstimateUserRespond_Other(UserActionStates& kUserStates,WORD wUserChairID, BYTE cbCenterCard)
{
	////碰牌判断
	kUserStates.m_cbUserAction[wUserChairID]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[wUserChairID],cbCenterCard);

	//杠牌判断
	if (getLeftCardNum()>0) 
	{
		DWORD cbTemp = m_GameLogic.EstimateGangCard(m_cbCardIndex[wUserChairID],cbCenterCard);
		if (cbTemp == WIK_GANG && m_cbGameTypeIdex == GAME_TYPE_ZZ)
		{
			kUserStates.m_cbUserAction[wUserChairID] |= WIK_GANG;
			m_cbGangInfo[wUserChairID].cbCardData[0] = cbCenterCard;
			m_cbGangInfo[wUserChairID].cbCardCount = 1;
		}
		else if(cbTemp == WIK_GANG && m_cbGameTypeIdex == GAME_TYPE_CS)
		{
			kUserStates.m_cbUserAction[wUserChairID] |= WIK_BU_ZHANG;
			m_cbGangInfo[wUserChairID].cbCardData[0] = cbCenterCard;
			m_cbGangInfo[wUserChairID].cbCardCount = 1;

			if (getLeftCardNum() >=2 &&!m_pITableFrame->GetTableUserItem(wUserChairID)->IsAndroidUser())
			{
				kUserStates.m_cbUserAction[wUserChairID] |= WIK_CS_GANG;
			}
		}

		////设不停牌不显示杠
		//if (!m_GameLogic.AnalyseTingCard(m_cbCardIndex[wUserChairID],m_WeaveItemArray[wUserChairID],m_cbWeaveItemCount[wUserChairID]))
		//{
		//	cbTemp = WIK_NULL;
		//}

	}
}
//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction=false;

	//用户状态
	clearUserActionStates();

	bool bHaiDiLao = false;
	if (m_cbGameTypeIdex == GAME_TYPE_CS && getLeftCardNum() == 0)
	{
		bHaiDiLao = true;
	}
	//动作判断
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//用户过滤
		if (wCenterUser==i) continue;

		//出牌类型
		if (EstimatKind==EstimatKind_OutCard && !m_bHasCSGang[i] && !bHaiDiLao)
		{
			EstimateUserRespond_Other(m_kUserActionStates1, i, cbCenterCard);
		}

		if (m_cbCanHu[i] != 1)
		{
			//吃胡判断
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_kUserActionStates1.m_cbUserAction[i]|= AnalyseChiHuCard(m_cbCardIndex[i], m_WeaveItemArray[i], cbWeaveCount, cbCenterCard, chr);
		}

		//结果判断
		if (m_kUserActionStates1.m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}

	//长沙麻将吃操作
	if (m_cbGameTypeIdex == GAME_TYPE_CS && !m_bHasCSGang[m_wCurrentUser] && !bHaiDiLao && EstimatKind == EstimatKind_OutCard)
	{
		m_kUserActionStates1.m_cbUserAction[m_wCurrentUser] |= m_GameLogic.EstimateEatCard(m_cbCardIndex[m_wCurrentUser],cbCenterCard);

		//结果判断
		if (m_kUserActionStates1.m_cbUserAction[m_wCurrentUser]!=WIK_NULL) 
			bAroseAction=true;
	}
	//结果处理
	if (bAroseAction==true) 
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_kUserActionStates1.m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//发送提示
		SendOperateNotify();
		return true;
	}

	return false;
}
bool CTableFrameSink::EstimateUserRespondCSGang( WORD wCenterUser, BYTE cbCenterCard1,BYTE cbCenterCard2)
{

	//设置变量
	if( m_bGangStatus != STATE_NULL)
	{
		if (m_bGangStatus == STATE_GANG)
		{
			m_bGangOutStatus = true;
		}
		m_bGangStatus = STATE_NULL;

	}
	//出牌记录
	m_cbOutCardCount+=2;;
	m_wOutCardUser=wCenterUser;
	m_kUserActionStates1.m_cbOutCardData=cbCenterCard1;
	m_kUserActionStates2.m_cbOutCardData=cbCenterCard2;
	m_wProvideUser=wCenterUser;
	m_kUserActionStates1.m_cbProvideCard=cbCenterCard1;
	m_kUserActionStates2.m_cbProvideCard=cbCenterCard2;
	//变量定义
	bool bAroseAction=false;

	bool bHaiDiLao = false;
	if (m_cbGameTypeIdex == GAME_TYPE_CS && getLeftCardNum() == 0)
	{
		bHaiDiLao = true;
	}
	//动作判断
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//用户过滤
		if (wCenterUser==i) continue;

		//出牌类型
		if ( !m_bHasCSGang[i] && !bHaiDiLao)
		{
			EstimateUserRespond_Other(m_kUserActionStates1,i,cbCenterCard1);
			EstimateUserRespond_Other(m_kUserActionStates2,i,cbCenterCard2);
		}

		if (m_cbCanHu[i] != 1)
		{
			//吃胡判断
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_kUserActionStates1.m_cbUserAction[i]|= AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard1,chr);
			m_kUserActionStates2.m_cbUserAction[i]|= AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard2,chr);
		}

		//结果判断
		if (m_kUserActionStates1.m_cbUserAction[i]!=WIK_NULL || m_kUserActionStates2.m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}

	//长沙麻将吃操作
	if (m_cbGameTypeIdex == GAME_TYPE_CS && !m_bHasCSGang[m_wCurrentUser] && !bHaiDiLao)
	{
		m_kUserActionStates1.m_cbUserAction[m_wCurrentUser] |= m_GameLogic.EstimateEatCard(m_cbCardIndex[m_wCurrentUser],cbCenterCard1);
		m_kUserActionStates2.m_cbUserAction[m_wCurrentUser] |= m_GameLogic.EstimateEatCard(m_cbCardIndex[m_wCurrentUser],cbCenterCard2);

		//结果判断
		if (m_kUserActionStates1.m_cbUserAction[m_wCurrentUser]!=WIK_NULL || m_kUserActionStates2.m_cbUserAction[m_wCurrentUser]!=WIK_NULL) 
			bAroseAction=true;
	}

	//结果处理
	if (bAroseAction==true) 
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//发送提示
		SendOperateNotify();
		return true;
	}
	return false;
}

//清除用户操作状态
void CTableFrameSink::clearUserActionStates(WORD cbChairID)
{
	if (cbChairID == INVALID_CHAIR)
	{
		m_kUserActionStates1.m_cbProvideCard = 0;
		ZeroMemory(m_kUserActionStates1.m_bResponse,sizeof(m_kUserActionStates1.m_bResponse));
		ZeroMemory(m_kUserActionStates1.m_cbUserAction,sizeof(m_kUserActionStates1.m_cbUserAction));
		ZeroMemory(m_kUserActionStates1.m_cbPerformAction,sizeof(m_kUserActionStates1.m_cbPerformAction));

		m_kUserActionStates2.m_cbProvideCard = 0;
		ZeroMemory(m_kUserActionStates2.m_bResponse,sizeof(m_kUserActionStates2.m_bResponse));
		ZeroMemory(m_kUserActionStates2.m_cbUserAction,sizeof(m_kUserActionStates2.m_cbUserAction));
		ZeroMemory(m_kUserActionStates2.m_cbPerformAction,sizeof(m_kUserActionStates2.m_cbPerformAction));
	}
	else
	{
		if (cbChairID >= GAME_PLAYER)
		{
			return;
		}
		m_kUserActionStates1.m_bResponse[cbChairID] = false;
		m_kUserActionStates1.m_cbUserAction[cbChairID] = 0;
		m_kUserActionStates1.m_cbPerformAction[cbChairID] = 0;

		m_kUserActionStates2.m_bResponse[cbChairID] = false;
		m_kUserActionStates2.m_cbUserAction[cbChairID] = 0;
		m_kUserActionStates2.m_cbPerformAction[cbChairID] = 0;
	}
}

//
void CTableFrameSink::ProcessChiHuUser( WORD wChairId,CChiHuRight &chr,BYTE	cbProvideCard)
{


	FiltrateRight( wChairId,chr );
	WORD wFanShu = 0;
	int nFanNum = 0;
	if ( m_cbGameTypeIdex == GAME_TYPE_ZZ)
	{
		wFanShu = m_GameLogic.GetChiHuActionRank_ZZ(chr);
		if (!(chr&CHR_QIANG_GANG_HU).IsEmpty())
		{
			wFanShu += 5;
		}
		
		nFanNum = 1;
	}
	if ( m_cbGameTypeIdex == GAME_TYPE_CS)
	{
		wFanShu = m_GameLogic.GetChiHuActionRank_CS(chr);
		//传奇胡牌为庄
		if (hasRule(GAME_TYPE_CS_HUPAIZHUANG))
		{
			m_wBankerUser = wChairId;
		}
		//wFanShu += 1;
		//nFanNum = 1;
		//if (wFanShu>6)
		//{
		//	nFanNum = wFanShu/6;
		//}
	}
	
	//胡牌分数
	LONGLONG lChiHuScore = wFanShu;//*m_pGameServiceOption->lCellScore;
	m_FanShu = wFanShu;
	if( m_wProvideUser != wChairId )
	{
		m_wLostFanShu[m_wProvideUser][wChairId] = (WORD)lChiHuScore;
	}
	else
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( i == wChairId ) continue;

			m_wLostFanShu[i][wChairId] = (WORD)lChiHuScore;
		}
	}

	if (m_cbNiaoPick == INVALID_BYTE)
	{
		m_lNiaoScore = checkNiaoScore(chr,wChairId,m_wProvideUser,lChiHuScore);//结束后设置鸟牌
	}

	//m_wProvideUser胡牌人 lChiHuScore胡牌分
	if( m_wProvideUser == wChairId )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( i == wChairId ) continue;
			//胡牌分
			m_lGameScore[i] -= lChiHuScore;
			m_lGameScore[wChairId] += lChiHuScore;


			if (hasRule(GAME_TYPE_CS_ZHUANGXIANFEN))
			{
				//庄闲分
				if (wChairId== m_wBankerUser || i== m_wBankerUser)
				{
					if (!(chr&CHR_SHUANG_HAOHUA_QI_XIAO_DUI).IsEmpty())
					{
						m_lGameScore[i] -= 3;
						m_lGameScore[wChairId] += 3;
					}else if (!(chr&CHR_HAOHUA_QI_XIAO_DUI).IsEmpty())
					{
						m_lGameScore[i] -= 2;
						m_lGameScore[wChairId] += 2;
					}
					else
					{
						m_lGameScore[i] -= 1;
						m_lGameScore[wChairId] += 1;
					}
				}
				else
				{
					//闲家
					if (!(chr&CHR_XIAO_DA_SI_XI).IsEmpty())
					{
						m_lGameScore[i] -= 1;
						m_lGameScore[wChairId] -= 1;
					}
					if (!(chr&CHR_XIAO_BAN_BAN_HU).IsEmpty())
					{
						m_lGameScore[i] -= 1;
						m_lGameScore[wChairId] -= 1;
					}
					if (!(chr&CHR_XIAO_LIU_LIU_SHUN).IsEmpty())
					{
						m_lGameScore[i] -= 1;
						m_lGameScore[wChairId] -= 1;
					}
					if (!(chr&CHR_XIAO_QUE_YI_SE).IsEmpty())
					{
						m_lGameScore[i] -= 1;
						m_lGameScore[wChairId] -= 1;
					}
				}

			}

			//if (hasRule(GAME_TYPE_ZZ_ZHANIAO_DIFF))
			//{
			//	m_lGameScore[i] -= m_cbChairIDNiaoCout[wChairId]*m_lNiaoScore;
			//	m_lGameScore[wChairId] += m_cbChairIDNiaoCout[wChairId]*m_lNiaoScore;

			//	m_lGameScore[i] -= m_cbChairIDNiaoCout[i]*m_lNiaoScore;
			//	m_lGameScore[wChairId] += m_cbChairIDNiaoCout[i]*m_lNiaoScore;

			//	m_cbVaildChairIDNiaoCout[i] = m_cbChairIDNiaoCout[i];
			//	m_cbVaildChairIDNiaoCout[wChairId] = m_cbChairIDNiaoCout[wChairId];
			//}
			//else
			//{
			//	m_lGameScore[i] -= m_lNiaoScore*m_cbNiaoPick;
			//	m_lGameScore[wChairId] += m_lNiaoScore*m_cbNiaoPick;
			//}

			//一码全中
			if (hasRule(GAME_TYPE_ZZ_ZHANIAO1))
			{
				m_lNiaoScore = checkNiaoScore(chr,wChairId,m_wProvideUser,lChiHuScore);//结束后设置鸟牌

				m_lGameScore[i] -= m_cbChairIDNiaoCout[wChairId]*m_lNiaoScore;
				m_lGameScore[wChairId] += m_cbChairIDNiaoCout[wChairId]*m_lNiaoScore;

				m_lGameScore[i] -= m_cbChairIDNiaoCout[i]*m_lNiaoScore;
				m_lGameScore[wChairId] += m_cbChairIDNiaoCout[i]*m_lNiaoScore;

				m_cbVaildChairIDNiaoCout[i] = m_cbChairIDNiaoCout[i];
				m_cbVaildChairIDNiaoCout[wChairId] = m_cbChairIDNiaoCout[wChairId];
			}

			//传奇鸟+1
			if (hasRule(GAME_TYPE_CS_ZHANIAO_1FEN))
			{
				switch ((m_wBankerUser - i + 4) % 4)
				{
				case 0:
					{
						m_lGameScore[i] -= m_Player * m_lNiaoScore;
						m_lGameScore[wChairId] += m_Player * m_lNiaoScore;
						break;
					}

				case 1:
					{
						m_lGameScore[i] -= r_Player * m_lNiaoScore;
						m_lGameScore[wChairId] += r_Player * m_lNiaoScore;
						break;
					}
				case 2:
					{

						m_lGameScore[i] -= t_Player * m_lNiaoScore;	
						m_lGameScore[wChairId] += t_Player * m_lNiaoScore;
						break;
					}
				case 3:
					{
						m_lGameScore[i] -= l_Player * m_lNiaoScore;
						m_lGameScore[wChairId] += l_Player * m_lNiaoScore;
						break;
					}
				default:
					break;
				}
			}

			//传奇鸟+鸟
			if (hasRule(GAME_TYPE_CS_NIAOJIANIAO))
			{
				m_lNiaoScore = lChiHuScore;
				if (i == m_wBankerUser)
				{
					if (!(chr&CHR_SHUANG_HAOHUA_QI_XIAO_DUI).IsEmpty())
					{
						m_lNiaoScore += 3;
					}
					else if (!(chr&CHR_HAOHUA_QI_XIAO_DUI).IsEmpty())
					{
						m_lNiaoScore += 2;
					}
					else
					{
						m_lNiaoScore++;
					}
				}
				switch ((m_wBankerUser - i + 4) % 4)
				{
				case 0:
					{
						m_lGameScore[i] -= m_Player * m_lNiaoScore;
						m_lGameScore[wChairId] += m_Player * m_lNiaoScore;
						break;
					}

				case 1:
					{
						m_lGameScore[i] -= r_Player * m_lNiaoScore;
						m_lGameScore[wChairId] += r_Player * m_lNiaoScore;
						break;
					}
				case 2:
					{
						m_lGameScore[i] -= t_Player * m_lNiaoScore;	
						m_lGameScore[wChairId] += t_Player * m_lNiaoScore;
						break;
					}
				case 3:
					{
						m_lGameScore[i] -= l_Player * m_lNiaoScore;
						m_lGameScore[wChairId] += l_Player * m_lNiaoScore;
						break;
					}
				default:
					break;
				}
				m_lNiaoScore--;
			}
			
		}
	}
	//点炮
	else
	{
		if (hasRule(GAME_TYPE_ZZ_HONGZHONG)&& isHZGuiZe()&&!(chr&CHR_QIANG_GANG_HU).IsEmpty())
		{
			lChiHuScore *=3;
		}

		if (hasRule(GAME_TYPE_CS_ZHUANGXIANFEN))
		{
			//庄闲分
			if (wChairId== m_wBankerUser || m_wProvideUser== m_wBankerUser)
			{
				if (!(chr&CHR_SHUANG_HAOHUA_QI_XIAO_DUI).IsEmpty())
				{
					m_lGameScore[m_wProvideUser] -= 3;
					m_lGameScore[wChairId] += 3;
				}else if (!(chr&CHR_HAOHUA_QI_XIAO_DUI).IsEmpty())
				{
					m_lGameScore[m_wProvideUser] -= 2;
					m_lGameScore[wChairId] += 2;
				}
				else
				{
					m_lGameScore[m_wProvideUser] -= 1;
					m_lGameScore[wChairId] += 1;
				}
			}
		}
		m_lGameScore[m_wProvideUser] -= lChiHuScore;
		m_lGameScore[wChairId] += lChiHuScore;

		if (hasRule(GAME_TYPE_CS_ZHANIAO_1FEN))
		{
			m_lGameScore[m_wProvideUser] -= m_lNiaoScore * m_cbNiaoPick;
			m_lGameScore[wChairId] += m_lNiaoScore * m_cbNiaoPick;
		}

		//传奇鸟+鸟
		if (hasRule(GAME_TYPE_CS_NIAOJIANIAO))
		{
			m_lNiaoScore = lChiHuScore;
			if (wChairId == m_wBankerUser || m_wProvideUser == m_wBankerUser)
			{
				if (!(chr&CHR_SHUANG_HAOHUA_QI_XIAO_DUI).IsEmpty())
				{
					m_lNiaoScore += 3;
				}
				else if (!(chr&CHR_HAOHUA_QI_XIAO_DUI).IsEmpty())
				{
					m_lNiaoScore += 2;
				}
				else
				{
					m_lNiaoScore++;
				}
			}
			m_lGameScore[m_wProvideUser] -= m_cbNiaoPick * m_lNiaoScore;
			m_lGameScore[wChairId] += m_lNiaoScore * m_cbNiaoPick;
		}
	}



	// 日志输出 ！！！！！！！！！！！！！！！！！！！！！！！
	//TCHAR szDescribe[128]=TEXT("");
	//_sntprintf(szDescribe,CountArray(szDescribe),TEXT("1 => m_lGameScore %d %d %d %d"),m_lGameScore[0],m_lGameScore[1],m_lGameScore[2],m_lGameScore[3]);
	//CTraceService::TraceString(szDescribe,TraceLevel_Normal);

	////TCHAR szDescribe[128]=TEXT("");
	//_sntprintf(szDescribe,CountArray(szDescribe),TEXT("2 => m_cbNiaoPick %d  wChairId %d"),m_cbNiaoPick, wChairId);
	//CTraceService::TraceString(szDescribe,TraceLevel_Normal);


	// 2017 03 08 zq
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//DWORD temv = m_lGameScore[wChairId];
		if (m_cbNiaoPick > 0)
		{
			if (hasRule(GAME_TYPE_ZZ_ZHANIAO2) || hasRule(GAME_TYPE_ZZ_ZHANIAO4) || hasRule(GAME_TYPE_ZZ_ZHANIAO6))
			{
				// 抓2鸟 抓4鸟 抓6鸟
				//if (m_cbNiaoCount)
				if( i == wChairId ) continue;
				m_lGameScore[i] -= wFanShu * m_cbNiaoPick;
				m_lGameScore[wChairId] += wFanShu * m_cbNiaoPick;
			}
		}
	}

	//_sntprintf(szDescribe,CountArray(szDescribe),TEXT("3 => m_lGameScore %d %d %d %d"),m_lGameScore[0],m_lGameScore[1],m_lGameScore[2],m_lGameScore[3]);
	//CTraceService::TraceString(szDescribe,TraceLevel_Normal);

	//设置变量
	m_wProvider[wChairId] = m_wProvideUser;

	//发送消息
	CMD_S_ChiHu ChiHu;
	ChiHu.wChiHuUser = wChairId;
	ChiHu.wProviderUser = m_wProvideUser;
	ChiHu.lGameScore = m_lGameScore[wChairId];
	ChiHu.cbCardCount = m_GameLogic.GetCardCount(m_cbCardIndex[wChairId]);
	ChiHu.cbChiHuCard = cbProvideCard;
	addGameRecordAction(ChiHu);

	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&ChiHu,sizeof(ChiHu) );
	m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&ChiHu,sizeof(ChiHu) );

	return;
}

BYTE CTableFrameSink::GetNiaoCardNum()
{
	BYTE nNum = 0;
	if(hasRule(GAME_TYPE_ZZ_ZHANIAO1))
	{
		nNum = 1;
	}
	if(hasRule(GAME_TYPE_ZZ_ZHANIAO2))
	{
		nNum = 2;
	}
	if(hasRule(GAME_TYPE_ZZ_ZHANIAO3))
	{
		nNum = 3;
	}
	if(hasRule(GAME_TYPE_ZZ_ZHANIAO4))
	{
		nNum = 4;
	}
	if(hasRule(GAME_TYPE_ZZ_ZHANIAO5))
	{
		nNum = 5;
	}
	if(hasRule(GAME_TYPE_ZZ_ZHANIAO6))
	{
		nNum = 6;
	}
	return nNum;
}

//返回鸟分
int CTableFrameSink::checkNiaoScore(CChiHuRight chr,BYTE wChairID,BYTE wProvideUser,LONGLONG lChiHuScore)
{
	m_cbNiaoPick = 0;
	ZeroMemory(m_cbCardDataNiao,sizeof(m_cbCardDataNiao));
	m_cbNiaoCount = GetNiaoCardNum();

	int nNiaoFen = m_pGameServiceOption->lCellScore;
	//传奇没有红中多抓一个鸟
	if((chr&CHR_HONGZHONG_WU).IsEmpty())
	{
		BYTE nHongZhongNum =  m_GameLogic.getMagicCardCount(m_cbCardIndex[wChairID]);
		if (nHongZhongNum == 0 && hasRule(GAME_TYPE_ZZ_HONGZHONG))
		{
			//没红中多加一个鸟
			m_cbNiaoCount++;
		}
	}
	if (m_cbNiaoCount == 0)
	{
		return 0;
	}
	BYTE cbMasterZhaNiao = m_cbMasterZhaNiao[wChairID];
	
	// 日志输出
	TCHAR szDescribe[128]=TEXT("");
	_sntprintf(szDescribe,CountArray(szDescribe),TEXT("checkNiaoScore %d %d %d %d"),m_cbMasterZhaNiao[0],m_cbMasterZhaNiao[1],m_cbMasterZhaNiao[2],m_cbMasterZhaNiao[3]);
	CTraceService::TraceString(szDescribe,TraceLevel_Normal);

	if (cbMasterZhaNiao != INVALID_BYTE)
	{
		if (cbMasterZhaNiao > m_cbNiaoCount)
		{
			cbMasterZhaNiao = m_cbNiaoCount;
		}
		BYTE cbTempVaild = 0;
		BYTE cbTempNiaoCardCout = 0;
		for (int i = 0;i<m_cbLeftCardCount && cbTempNiaoCardCout < m_cbNiaoCount;i++)
		{
			BYTE cbCard = 0;
			cbCard = m_cbRepertoryCard[i];
			BYTE nValue = cbCard&MASK_VALUE;
			if (nValue == 1 || nValue == 5 || nValue == 9 || m_GameLogic.IsMagicCard(cbCard))
			{
				if (cbTempVaild < cbMasterZhaNiao)
				{
					m_cbCardDataNiao[cbTempNiaoCardCout] = cbCard;
					cbTempNiaoCardCout++;
					cbTempVaild++;
				}
			}
		}
		for (int i = 0;i<m_cbLeftCardCount && cbTempNiaoCardCout < m_cbNiaoCount;i++)
		{
			BYTE cbCard = m_cbRepertoryCard[i];
			BYTE nValue = cbCard&MASK_VALUE;
			if (nValue == 1 || nValue == 5 || nValue == 9 || m_GameLogic.IsMagicCard(cbCard))
			{
			}
			else
			{
				m_cbCardDataNiao[cbTempNiaoCardCout] = cbCard;
				cbTempNiaoCardCout++;
			}
		}
		while (cbTempNiaoCardCout < m_cbNiaoCount)
		{
			m_cbCardDataNiao[cbTempNiaoCardCout] = m_cbRepertoryCard[0];
			cbTempNiaoCardCout++;
		}
	}
	else
	{
		BYTE cbTempNiaoCardCout = 0;
		for (BYTE i=0;i<m_cbNiaoCount;i++)
		{
			int iIdex = m_cbLeftCardCount - i - 1;
			if (iIdex < 0)
			{
				iIdex = 0;
			}
			//剩余的牌小于鸟牌数
			if (m_cbLeftCardCount < m_cbNiaoCount)
			{
				//鸟随机给索引
				m_cbCardDataNiao[cbTempNiaoCardCout] = m_cbRepertoryCard[rand()%112];
			}
			else
			{
				m_cbCardDataNiao[cbTempNiaoCardCout] = m_cbRepertoryCard[iIdex];
			}
			cbTempNiaoCardCout++;
		}
	}

	if (hasRule(GAME_TYPE_ZZ_ZHANIAO_DOUBLE))
	{
		//扎鸟翻倍
		for (int i=0;i<m_cbNiaoCount;i++)
		{
			ASSERT(m_GameLogic.IsValidCard(m_cbCardDataNiao[i]));

			BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;
			if (nValue == 1 || nValue == 5 || nValue == 9)
			{
				m_cbNiaoPick++;
			}
		}
		nNiaoFen = (int)lChiHuScore;
	}
	else if (hasRule(GAME_TYPE_ZZ_ZHANIAO_DIFF))
	{
		//扎鸟分开算
		m_cbNiaoPick = m_cbNiaoCount;
		for (int i=0;i<m_cbNiaoCount;i++)
		{
			ASSERT(m_GameLogic.IsValidCard(m_cbCardDataNiao[i]));

			BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;
			BYTE wDestChairID = (nValue-1+4)%4;
			wDestChairID = (m_wNiaoDuoPaoStartChairID-wDestChairID+GAME_PLAYER)%GAME_PLAYER;
			m_cbChairIDNiaoCout[wDestChairID]++;
		}
	}
	
	else if (hasRule(GAME_TYPE_ZZ_ZHANIAO_2FEN))
	{
		//扎鸟+2分
		for (int i=0;i<m_cbNiaoCount;i++)
		{
			ASSERT(m_GameLogic.IsValidCard(m_cbCardDataNiao[i]));

			BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;
			if (nValue == 1 || nValue == 5 || nValue == 9)
			{
				m_cbNiaoPick++;
			}
		}
		nNiaoFen = nNiaoFen*2;
	}
	//传奇麻将抓鸟+1
	else if (hasRule(GAME_TYPE_CS_ZHANIAO_1FEN))
	{
		wProvideUser;//放炮
		wChairID;//胡牌
		m_wBankerUser;//庄家

		m_Player = 0;
		r_Player = 0;
		t_Player = 0;
		l_Player = 0;
		
		for (int i=0;i<m_cbNiaoCount;i++)
		{
			ASSERT(m_GameLogic.IsValidCard(m_cbCardDataNiao[i]));
			BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;


			//放炮中鸟数统计
			if(nValue == 1||nValue == 5||nValue == 9)
			{
				m_Player++;
			}
			else if (nValue == 2||nValue == 6)
			{
				r_Player++;
			}
			else if(nValue == 3||nValue == 7)
			{
				t_Player++;
			}
			else if(nValue == 4||nValue == 8)
			{
				l_Player++;
			}
		}

		if (wProvideUser == wChairID)
		{
			//自摸
			switch (m_wBankerUser)
			{
			case 0:
				{
					m_cbNiaoPick = m_Player * 3 + r_Player + t_Player + l_Player;
					r_Player += m_Player;
					t_Player += m_Player;
					l_Player += m_Player;
					break;
				}

			case 1:
				{
					m_cbNiaoPick = m_Player + r_Player + t_Player + l_Player * 3;
					r_Player += l_Player;
					t_Player += l_Player;
					m_Player += l_Player;
					break;
				}

			case 2:
				{
					m_cbNiaoPick = m_Player + r_Player + t_Player * 3 + l_Player;
					r_Player += t_Player;
					m_Player += t_Player;
					l_Player += t_Player;
					break;
				}

			case 3:
				{
					m_cbNiaoPick = m_Player + r_Player * 3 + t_Player + l_Player;
					t_Player += r_Player;
					m_Player += r_Player;
					l_Player += r_Player;
					break;
				}

			default:
				break;
			}

		}
		else
		{
			for (int i=0;i<m_cbNiaoCount;i++)
			{
				ASSERT(m_GameLogic.IsValidCard(m_cbCardDataNiao[i]));
				BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;
				//胡牌中鸟数统计
				if((nValue==1||nValue==5||nValue==9)&& (wChairID==m_wBankerUser ||
					wProvideUser==m_wBankerUser))
				{
					m_cbNiaoPick++;
				}
				else if ((nValue==2||nValue==6)&&(wChairID==(m_wBankerUser+3)%4 ||wProvideUser==(m_wBankerUser+3)%4))
				{
					m_cbNiaoPick++;
				}
				else if((nValue==3||nValue==7)&&(wChairID==(m_wBankerUser+2)%4 ||wProvideUser==(m_wBankerUser+2)%4))
				{
					m_cbNiaoPick++;
				}else if((nValue==4||nValue==8)&&(wChairID==(m_wBankerUser+1)%4 ||wProvideUser==(m_wBankerUser+1)%4))
				{
					m_cbNiaoPick++;
				}
			}
		}

		nNiaoFen = 1;
	}
	//传奇麻将鸟加鸟
	else if (hasRule(GAME_TYPE_CS_NIAOJIANIAO))
	{
		m_Player = 0;
		r_Player = 0;
		t_Player = 0;
		l_Player = 0;

		for (int i=0;i<m_cbNiaoCount;i++)
		{
			BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;
			//中鸟数统计
			if(nValue == 1||nValue == 5||nValue == 9)
			{
				m_Player++;
			}
			else if (nValue == 2||nValue == 6)
			{
				r_Player++;
			}
			else if(nValue == 3||nValue == 7)
			{
				t_Player++;
			}
			else if(nValue == 4||nValue == 8)
			{
				l_Player++;
			}
		}

		if (wProvideUser == wChairID)
		{
			//自摸
			switch (m_wBankerUser)
			{
			case 0:
				{
					m_cbNiaoPick = m_Player * 3 + r_Player + t_Player + l_Player;
					r_Player += m_Player;
					t_Player += m_Player;
					l_Player += m_Player;
					break;
				}

			case 1:
				{
					m_cbNiaoPick = m_Player + r_Player + t_Player + l_Player * 3;
					r_Player += l_Player;
					t_Player += l_Player;
					m_Player += l_Player;
					break;
				}

			case 2:
				{
					m_cbNiaoPick = m_Player + r_Player + t_Player * 3 + l_Player;
					r_Player += t_Player;
					m_Player += t_Player;
					l_Player += t_Player;
					break;
				}

			case 3:
				{
					m_cbNiaoPick = m_Player + r_Player * 3 + t_Player + l_Player;
					t_Player += r_Player;
					m_Player += r_Player;
					l_Player += r_Player;
					break;
				}

			default:
				break;
			}
		}
		else
		{
			for (int i=0;i<m_cbNiaoCount;i++)
			{
				ASSERT(m_GameLogic.IsValidCard(m_cbCardDataNiao[i]));
				BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;
				//胡牌中鸟数统计
				if((nValue==1||nValue==5||nValue==9)&& (wChairID==m_wBankerUser ||
					wProvideUser==m_wBankerUser))
				{
					m_cbNiaoPick++;
				}
				else if ((nValue==2||nValue==6)&&(wChairID==(m_wBankerUser+3)%4 ||wProvideUser==(m_wBankerUser+3)%4))
				{
					m_cbNiaoPick++;
				}
				else if((nValue==3||nValue==7)&&(wChairID==(m_wBankerUser+2)%4 ||wProvideUser==(m_wBankerUser+2)%4))
				{
					m_cbNiaoPick++;
				}else if((nValue==4||nValue==8)&&(wChairID==(m_wBankerUser+1)%4 ||wProvideUser==(m_wBankerUser+1)%4))
				{
					m_cbNiaoPick++;
				}
			}
		}
	}
	//----------------------------------------------------------
	else
	{
		if (hasRule(GAME_TYPE_ZZ_HONGZHONG)&& isHZGuiZe())
		{
			nNiaoFen = nNiaoFen*2;
		}
		for (int i=0;i<m_cbNiaoCount;i++)
		{
			ASSERT(m_GameLogic.IsValidCard(m_cbCardDataNiao[i]));

			BYTE nValue = m_cbCardDataNiao[i]&MASK_VALUE;
			if (nValue == 1 || nValue == 5 || nValue == 9)
			{
				m_cbNiaoPick++;
			}
		}
		if (hasRule(GAME_TYPE_ZZ_HONGZHONG)&& isHZGuiZe()&&!(chr&CHR_QIANG_GANG_HU).IsEmpty())
		{
			nNiaoFen*=3;
		}
	}

	return nNiaoFen;
}



void CTableFrameSink::OnUserXiaoHu(WORD wChairID,DWORD dwCode)
{
	DWORD dwChiHuKind = AnalyseChiHuCardCS_XIAOHU(m_cbCardIndex[wChairID],m_StartHuRight[wChairID]);

	if (dwChiHuKind == dwCode  && dwCode == WIK_XIAO_HU)
	{
		LONGLONG lStartHuScore = 0;
		int wFanShu = m_GameLogic.GetChiHuActionRank_CS(m_StartHuRight[wChairID]);
		lStartHuScore  = wFanShu*m_pGameServiceOption->lCellScore;
		m_lStartHuScore[wChairID] += lStartHuScore*3;

		for (int i=0;i<GAME_PLAYER;i++)
		{
			if (i == wChairID)continue;
			m_lStartHuScore[i] -= lStartHuScore;
		}
	}
	if (dwCode !=WIK_NULL)
	{
		CMD_S_XiaoHu kXiaoHu;
		ZeroMemory(kXiaoHu.cbCardData,sizeof(kXiaoHu.cbCardData));
		getXiaoHuCard(wChairID,kXiaoHu);
		m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_XIAO_HU,&kXiaoHu,sizeof(kXiaoHu) );
		m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_XIAO_HU,&kXiaoHu,sizeof(kXiaoHu) );
	}

	m_kUserActionStates1.m_cbUserAction[wChairID] = WIK_NULL;

	bool bEnd = true;
	for (int i = 0;i < GAME_PLAYER;i++)
	{
		if(m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
		{
			continue;
		}
		if (m_kUserActionStates1.m_cbUserAction[i] != WIK_NULL)
		{
			bEnd = false;
		}
	}
	if (bEnd)
	{
		m_pITableFrame->SetGameTimer(IDI_TIMER_XIAO_HU,(TIME_XIAO_HU)*1000,1,0);
	}
}

void CTableFrameSink::getXiaoHuCard( WORD wChairID,CMD_S_XiaoHu& kXiaoHu)
{
	m_StartHuRight[wChairID].GetRightData( &kXiaoHu.dwXiaoCode,MAX_RIGHT_COUNT );
	
	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,m_cbCardIndex[wChairID],sizeof(cbCardIndexTemp));
	BYTE cbVaildIndexTemp[MAX_INDEX];
	ZeroMemory(cbVaildIndexTemp,sizeof(cbVaildIndexTemp));

	int cbLiuLiuShun = 0;
	bool bDaSiXi = false;
	kXiaoHu.cbCardCount = 0;
	kXiaoHu.wXiaoHuUser = wChairID;
	//计算单牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndexTemp[i];

		if (cbCardCount ==0)
		{
			continue;
		}

		if( cbCardCount == 4 && kXiaoHu.dwXiaoCode&CHR_XIAO_DA_SI_XI && !bDaSiXi)
		{
			cbVaildIndexTemp[i] = 4;
			bDaSiXi = true;
		}

		if(cbCardCount == 3 && kXiaoHu.dwXiaoCode&CHR_XIAO_LIU_LIU_SHUN && cbLiuLiuShun <2)
		{
			if (cbVaildIndexTemp[i] == 0)
			{
				cbVaildIndexTemp[i] = 3;
			}
			cbLiuLiuShun ++;
		}
	}

	if (kXiaoHu.dwXiaoCode&CHR_XIAO_QUE_YI_SE || kXiaoHu.dwXiaoCode&CHR_XIAO_BAN_BAN_HU)
	{
		kXiaoHu.cbCardCount = m_GameLogic.SwitchToCardData(cbCardIndexTemp,kXiaoHu.cbCardData);
		ASSERT(kXiaoHu.cbCardCount == MAX_COUNT -1);
	}
	else
	{
		kXiaoHu.cbCardCount = m_GameLogic.SwitchToCardData(cbVaildIndexTemp,kXiaoHu.cbCardData);
	}
}

int CTableFrameSink::getLeftCardNum()
{
	return m_cbLeftCardCount;
}

//////////////////////////////////////////////////////////////////////////
