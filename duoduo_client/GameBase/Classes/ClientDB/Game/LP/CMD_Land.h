#pragma once

#include "Game/Game/GameDefine.h"
//////////////////////////////////////////////////////////////////////////

#pragma pack(1)

//公共宏定义

#define KIND_ID						404									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("轮盘")						//游戏名字
//组件属性
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

#define	HISTORY_NUMBER				13									//历史记录个数
#define	MAX_BET_POS					256									//最多下注位置

//游戏状态
#define GAME_SCENE_BET					GAME_STATUS_PLAY+1							//下注状态
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+2							//游戏状态

#define LP_MAX_NUMBER         38
//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_GAME_END				101									//游戏结束
#define SUB_S_USER_BET				102									//用户下注
#define SUB_S_USER_CANCLE_BET		103									//用户取消下注

#define LP_BET_TIME        30  //下注时间
#define LP_END_TIME        30	 //结算时间

#define LP_USER_CANCEL_ONCE	0 
#define LP_USER_CANCEL_ALL		1
#define LP_USER_CANCEL_LAST	2


//下注状态
struct CMD_S_StatusBet
{
	BYTE							cbTimeLeave;	//剩余时间
};
//开奖状态
struct CMD_S_StatusPlay
{
	BYTE							cbTimeLeave;	//剩余时间
};

//游戏开始
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;	//剩余时间
};

//游戏结束
struct LPCMD_S_GameEnd
{
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							bWinNumber;						//胜利数字
	QYLONG							lWinScore[GAME_PLAYER];			//胜利分数
};
//用户下注
struct CMD_S_UserBet
{
	WORD			  wChairID;										//用户位置
	BYTE          cbBetArea;										//下注区域
	QYLONG          lBetScore;										//下注分数
	LONGLONG       lTaltalScore;									//剩余分数
	float         kPosX;											//筹码位置
	float         kPosY;											//筹码位置
};

struct CMD_S_UserCancleBet
{
	WORD			  wChairID;										//用户位置
	int			  nType;
	LONGLONG       lBetScore;
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_USER_BET					10									//用户叫分
#define SUB_C_USER_CANCLE_BET			11									//用户取消下注


//用户下注
struct CMD_C_UserBet
{
	BYTE          cbBetArea;										//下注区域
	QYLONG        lBetScore;										//下注分数
	float         kPosX;											//筹码位置
	float         kPosY;											//筹码位置
};

struct CMD_C_UserCancleBet
{
	int nType;
};
#pragma pack()