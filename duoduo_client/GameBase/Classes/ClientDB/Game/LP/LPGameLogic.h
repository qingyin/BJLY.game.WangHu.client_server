
#pragma once

#include "Game/GameLib.h"
#include "Game/Game/GameDefine.h"

struct LPBetType
{
	int nBetID;
	int nBetType;
	std::string _kCellName;
	std::vector<int> _kContentNums;
};
enum LP_BEISHU
{
	LP_BEISHU_1  =36,
	LP_BEISHU_2  =18,
	LP_BEISHU_3  =12,
	LP_BEISHU_4  =9,
	LP_BEISHU_5  =7,
	LP_BEISHU_6  =6,
	LP_BEISHU_12 =3,
	LP_BEISHU_18 =2,

};

enum LP_BET_TYPE
{
	LP_BET_TYPE_1  = 38,
	LP_BET_TYPE_2  = 60,
	LP_BET_TYPE_3  = 15,
	LP_BET_TYPE_4  = 22,
	LP_BET_TYPE_5  = 1,
	LP_BET_TYPE_6  = 11,
	LP_BET_TYPE_12 = 6,
	LP_BET_TYPE_18 = 6,

};

//游戏逻辑
class LPGameLogic:public FvSingleton<LPGameLogic>
{
public:	    
	//变量定义
	const static int  MAX_BET_ID   = 159;

	static const int	 m_sBetNums_1[LP_BET_TYPE_1][1];	
	static const int	 m_sBetNums_2[LP_BET_TYPE_2][2];	
	static const int	 m_sBetNums_3[LP_BET_TYPE_3][3];
	static const int	 m_sBetNums_4[LP_BET_TYPE_4][4];
	static const int	 m_sBetNums_5[LP_BET_TYPE_5][5];
	static const int	 m_sBetNums_6[LP_BET_TYPE_6][6];	
	static const int	 m_sBetNums_12[LP_BET_TYPE_12][12];		
	static const int	 m_sBetNums_18[LP_BET_TYPE_18][18];

	static const char* m_sBetTypeName_12[LP_BET_TYPE_12];
	static const char* m_sBetTypeName_18[LP_BET_TYPE_18];

	static const int   m_sWheelNumOrder[38];
	static const int   m_sBlackNum[18];

	static const float  m_sfRangle; //#define  rangle 9.47368421f
public:
	//构造函数
	LPGameLogic();
	//析构函数
	virtual ~LPGameLogic();
public:
	void initBetType();
	int  getBeiShu(int nBetIndex);
	std::vector<int> getNumListByName(const std::string& kName);
	std::vector<int> getNumListByBetID(int nBetID);
	int getBetID( std::vector<int> kNumList);
	float getRangle(int nEndNum);
	std::vector<std::string> getOtherCellName(int nEndNum); //根据数字 得到下注区域
	std::vector<std::string> getBetCellName(int nArea);//下注类型 得到下注区域
	bool bIsBlackNum(int nNum);
protected:
	std::map<int,LPBetType> m_kBetTypeList;
};

