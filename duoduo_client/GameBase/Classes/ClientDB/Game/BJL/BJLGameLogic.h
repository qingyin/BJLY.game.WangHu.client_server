
#pragma once

#include "Game/GameLib.h"
#include "Game/Game/GameDefine.h"

//游戏逻辑
class BJLGameLogic:public FvSingleton<BJLGameLogic>
{
public:

	//数值掩码
	const static int	LOGIC_MASK_COLOR			=0xF0;							//花色掩码
	const static int	LOGIC_MASK_VALUE			=0x0F;							//数值掩码
	const static int	LOGIC_GAME_PLAYER				=100;							
	const static int	LOGIC_AREA_MAX					=8;								
	const static int	LOGIC_AREA_XIAN				=0;								
	const static int	LOGIC_AREA_ZHUANG				=2;					    
	//变量定义
private:
	static const BYTE				m_cbCardListData[52*8];				//扑克定义

	//函数定义
public:
	//构造函数
	BJLGameLogic();
	//析构函数
	virtual ~BJLGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return (cbCardData&LOGIC_MASK_COLOR)/LOGIC_MASK_VALUE; }

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardPip(BYTE cbCardData);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	BYTE GetAreaBetMax(int nArea,LONGLONG AllPlayBet[LOGIC_GAME_PLAYER][LOGIC_AREA_MAX],SCORE& nMaxBetNum);


};

