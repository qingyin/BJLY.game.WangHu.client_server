
#include "LPGameLogic.h"

//////////////////////////////////////////////////////////////////////////

FV_SINGLETON_STORAGE(LPGameLogic);

//00的id 37
const int LPGameLogic::m_sBetNums_1[LP_BET_TYPE_1][1]=
{
	//36倍 1压  38种
	{37},{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},{17},{18},{19},{20},
	{21},{22},{23},{24},{25},{26},{27},{28},{29},{30},{31},{32},{33},{34},{35},{36},
};
const int LPGameLogic::m_sBetNums_2[LP_BET_TYPE_2][2] =
{
	//18倍 2压  60种
	{37,1}, {0,37}, {0,3},  {1,2},  {1,4},  {2,3},  {2,5},  {3,6}, {4,5},  {4,7},  {5,6},  {5,8},  {6,9},  {7,8}, {7,10}, {8,9},
	{8,11}, {9,12}, {10,11},{10,13},{11,12},{11,14},{12,15},{13,14},{13,16},{14,15},{14,17},{15,18},{16,17},{16,19},{17,18},{17,20},
	{18,21},{19,20},{19,22},{20,21},{20,23},{21,24},{22,23},{22,25},{23,24},{23,26},{24,27},{25,26},{25,28},{26,27},{26,29},{27,30},
	{28,29},{28,31},{29,30},{29,32},{30,33},{31,32},{31,34},{32,33},{32,35},{33,36},{34,35},{35,36},
};
const int LPGameLogic::m_sBetNums_3[LP_BET_TYPE_3][3] =
{
	//12倍 3压  15种
	{37,1,2},  {0,2,37},  {0,2,3},  {1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15},{16,17,18},{19,20,21},{22,23,24},{25,26,27,},
	{28,29,30},{31,32,33},{34,35,36},
};

const int LPGameLogic::m_sBetNums_4[LP_BET_TYPE_4][4] =
{
	//9倍 4压  22种
	{1,2,4,5},    {2,3,5,6},   {4,5,7,8},    {5,6,8,9},   {7,8,10,11},  {8,9,11,12},  {10,11,13,14},{11,12,14,15},{13,14,16,17},
	{14,15,17,18},{16,17,19,20},{17,18,20,21},{19,20,22,23},{20,21,23,24},{22,23,25,26},{23,24,26,27},{25,26,28,29},{26,27,29,30},
	{28,29,31,32},{29,30,32,33},{31,32,34,35},{32,33,35,36},
};
const int LPGameLogic::m_sBetNums_5[LP_BET_TYPE_5][5] =
{
	//7倍 5压 1种
	{37,0,1,2,3}
};
const int LPGameLogic::m_sBetNums_6[LP_BET_TYPE_6][6] =
{
	//6倍 6压 11种
	{1,2,3,4,5,6},{4,5,6,7,8,9},{7,8,9,10,11,12},{10,11,12,13,14,15},{13,14,15,16,17,18},{16,17,18,19,20,21},{19,20,21,22,23,24},
	{22,23,24,25,26,27},{25,26,27,28,29,30},{28,29,30,31,32,33},{31,32,33,34,35,36},

};
const int LPGameLogic::m_sBetNums_12[LP_BET_TYPE_12][12] =
{
	//3倍 12压 6种
	{1,4,7,10,13,16,19,22,25,28,31,34},{2,5,8,11,14,17,20,23,26,29,32,35},{3,6,9,12,15,18,21,24,27,30,33,36}, //行123
	{1,2,3,4,5,6,7,8,9,10,11,12},{13,14,15,16,17,18,19,20,21,22,23,24},{25,26,27,28,29,30,31,32,33,34,35,36}, //区压 1-12 13-24 25-26

};
const int LPGameLogic::m_sBetNums_18[LP_BET_TYPE_18][18] =
{
	//2倍 18压 6种
	{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18},{19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36},//大小
	{1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35},{2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36},//单双
	{1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36},{2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35},//红黑
};

const char* LPGameLogic::m_sBetTypeName_12[LP_BET_TYPE_12] =
{
	//行123
	"LP_38",
	"LP_39",
	"LP_40",

	//区压 1-12 13-24 25-26
	"LP_41",
	"LP_42",
	"LP_43",
};
const char* LPGameLogic::m_sBetTypeName_18[LP_BET_TYPE_18] =
{
	"LP_44",//大小
	"LP_49",

	"LP_45",//单双
	"LP_48",

	"LP_46",//红黑
	"LP_47",
};

const int LPGameLogic::m_sWheelNumOrder[] =
{
	0,28,9,26,30,11,7,20,32,17,5,22,34,15,3,24,36,13,1,37,27,10,25,29,12,8,19,31,18,6,21,33,16,4,23,35,14,2
};
const int LPGameLogic::m_sBlackNum[] =
{
	2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35
};

const float LPGameLogic::m_sfRangle = 9.47368421f;

LPGameLogic::LPGameLogic()
{
	initBetType();
}

LPGameLogic::~LPGameLogic()
{

}

void LPGameLogic::initBetType()
{
	int nIndexID = 0;

#define SET_DATA_MAP(T) \
	for (int i=0;i<LP_BET_TYPE_##T;i++)\
	{\
		LPBetType kInfo;\
		kInfo.nBetID = nIndexID;\
		for (int n=0;n<T;n++)\
		{\
			kInfo._kContentNums.push_back(m_sBetNums_##T[i][n]);\
		}\
		std::sort(kInfo._kContentNums.begin(),kInfo._kContentNums.end());\
		m_kBetTypeList.insert(std::make_pair(nIndexID,kInfo));\
		nIndexID++;\
	}\

#define SET_DATA_MAP_NAME(T) \
	for (int i=0;i<LP_BET_TYPE_##T;i++)\
	{\
	LPBetType kInfo;\
	kInfo.nBetID = nIndexID;\
	for (int n=0;n<T;n++)\
	{\
		kInfo._kContentNums.push_back(m_sBetNums_##T[i][n]);\
	}\
	std::sort(kInfo._kContentNums.begin(),kInfo._kContentNums.end());\
	kInfo._kCellName = m_sBetTypeName_##T[i];\
	m_kBetTypeList.insert(std::make_pair(nIndexID,kInfo));\
	nIndexID++;\
	}\

	SET_DATA_MAP(1);
	SET_DATA_MAP(2);
	SET_DATA_MAP(3);
	SET_DATA_MAP(4);
	SET_DATA_MAP(5);
	SET_DATA_MAP(6);
	SET_DATA_MAP_NAME(12);
	SET_DATA_MAP_NAME(18);
}

int LPGameLogic::getBeiShu(int nBetIndex)
{
	std::map<int,LPBetType>::iterator iter = m_kBetTypeList.find(nBetIndex);
	if (iter!=m_kBetTypeList.end())
	{
		int len = iter->second._kContentNums.size();

		if (len ==1)
		{
			return LP_BEISHU_1;
		}
		else if (len == 2 )
		{
			return LP_BEISHU_2;
		}
		else if (len == 3 )
		{
			return LP_BEISHU_3;
		}
		else if (len == 4 )
		{
			return LP_BEISHU_4;
		}
		else if (len == 5 )
		{
			return LP_BEISHU_5;
		}
		else if (len == 6 )
		{
			return LP_BEISHU_6;
		}
		else if (len == 12 )
		{
			return LP_BEISHU_12;
		}else if (len == 18 )
		{
			return LP_BEISHU_18;
		}
	}
	return 0;
}

std::vector<int> LPGameLogic::getNumListByName(const std::string& kName)
{
	std::vector<int> numsList;
	std::map<int,LPBetType>::iterator iter = m_kBetTypeList.begin();
	for (;iter!=m_kBetTypeList.end();iter++)
	{
		if ( kName == iter->second._kCellName )
		{
			numsList = iter->second._kContentNums;
			break;
		}
	}
	return numsList;
}

std::vector<int> LPGameLogic::getNumListByBetID(int nBetID)
{
	std::vector<int> numsList;
	std::map<int,LPBetType>::iterator iter = m_kBetTypeList.find(nBetID);
	if(iter!=m_kBetTypeList.end())
	{
		numsList = iter->second._kContentNums;	
	}
	return numsList;
}

int LPGameLogic::getBetID(std::vector<int> kNumList)
{
	std::sort(kNumList.begin(),kNumList.end());
	std::map<int,LPBetType>::iterator iter = m_kBetTypeList.begin();

	for (;iter!=m_kBetTypeList.end();iter++)
	{
		if (kNumList == iter->second._kContentNums)
		{
			return iter->first;
		}
	}
	ASSERT(false);
	return -1;
}

float LPGameLogic::getRangle(int nEndNum)
{
	for (int i=0;i<38;i++)
	{
		if (m_sWheelNumOrder[i] == nEndNum )
		{
			return i*m_sfRangle;
		}
	}
	ASSERT(false);
	return -1;
}

std::vector<std::string> LPGameLogic::getOtherCellName(int nEndNum)
{
	std::vector<std::string> kCellNameList;
	std::map<int,LPBetType>::iterator iter = m_kBetTypeList.begin();
	for (;iter != m_kBetTypeList.end();iter++)
	{
		if (iter->second._kContentNums.size()<12 || iter->second._kCellName.empty())
		{
			continue;
		}
		std::vector<int>::iterator iterNum = std::find(iter->second._kContentNums.begin(),iter->second._kContentNums.end(),nEndNum);
		if (iterNum != iter->second._kContentNums.end())
		{
			kCellNameList.push_back(iter->second._kCellName);
		}
	}
	return kCellNameList;
}

std::vector<std::string> LPGameLogic::getBetCellName(int nArea)
{
	std::vector<std::string> kCellNameList;

	std::map<int,LPBetType>::iterator iter = m_kBetTypeList.find(nArea);
	if (iter != m_kBetTypeList.end())
	{
		if (iter->second._kCellName.empty())
		{
			for (int i =0 ;i<(int)iter->second._kContentNums.size();i++)
			{
				kCellNameList.push_back(utility::toString("LP_",iter->second._kContentNums[i]));
			}
		}
		else
		{
			kCellNameList.push_back(iter->second._kCellName);
		}
	}
	return kCellNameList;
}

bool LPGameLogic::bIsBlackNum(int nNum)
{
	bool bValue = false;
	for (int i=0;i<18;i++)
	{
		if (m_sBlackNum[i] == nNum )
		{
			bValue = true;
			break;
		}
	}
	return bValue;
}
