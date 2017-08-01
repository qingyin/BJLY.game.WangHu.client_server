#pragma once
#include "cocos2d.h"
USING_NS_CC;
// enum 
// {
// 	type_1rd,
// 	type_2rd,
// 	type_3rd,
// 
// 	type_1_12,
// 	type_13_24,
// 	type_25_36,
// 
// 	type_1_18,
// 	type_odd,
// 	type_red,
// 
// 	type_black,
// 	type_even,
// 	type_19_36,
// 
// 	type_max,
// };
// static const char* s_typeCell[] =
// {
// 	//"LP_37",    //00点
// 	"LP_38",     //type_1rd
// 	"LP_39",		//type_2rd
// 	"LP_40",		//type_3rd
// 
// 	"LP_41" ,	//type_1_12
// 	"LP_42",		//type_13_24
// 	"LP_43",		//type_25_36
// 
// 	"LP_44" ,	//type_1_18
// 	"LP_45" ,	//type_odd
// 	"LP_46",		//type_red
// 
// 	"LP_47" ,	//type_black
// 	"LP_48" ,	//type_even
// 	"LP_49",		//type_19_36
// };

enum 
{
	ya_unknow,
	ya_1 = 1,
	ya_2,
	ya_3,
	ya_4,
	ya_5,
	ya_6,
	ya_1rd,
	ya_2rd,
	ya_3rd,
	ya_1_12,
	ya_13_24,
	ya_25_36,
	ya_1_18,
	ya_19_36,
	ya_odd,
	ya_even,
	ya_red,
	ya_black,
};

class YaCell
{
public:
	static Vec2 cheakEndPos(std::vector<YaCell*>& kCellList,int nBetType);
	YaCell(const std::string& kName,Node* pShadow);
	~YaCell();
public:
	bool checkByRect(const Rect& kRect);
	bool checkByPos(const Vec2& kPos);
	void show(bool bShow);
	bool isShow();
	std::string& getName();
	const Vec2& getPosition();
	float getTop();
	int   getNumber();
private:
	std::string     m_kName;
	Sprite*       m_pCellShadow;

};



class YaChip
{
public:
	enum 
	{
		gameChips1 = 100,
		gameChips2 = 500,
		gameChips3 = 1000,
		gameChips4 = 5000,
		gameChips5 = 10000,
		chip_num_tag,
	};
public:
	YaChip();
	~YaChip();
	void defaultInit();
	void init(int nType,bool bTxt);
	static YaChip* create(int nType,bool bTxt = true);
public:
	Node* getNode();
	void setYaType(int nYaType);
	void setYaID(int nArea);

	int getYaID();
	int getYaNum();
	void setYaNumTemp(int nNum);
	void updateText();
private:
	int   m_nChipType;//筹码类型
	int   m_nNum;	  //押注数
	int   m_nNumTemp; //表像押注数
	Sprite*       m_pChip;
	Sprite*		  m_pTextBg;
	Label*        m_pText;
	int   m_nYaType;	//该筹码押注类型
	int   m_nYaID;      //押注类型id
public:
	void clearNumList();
	void setNumList(std::vector<int> kList);
	void insertNum(int nNum);
	bool isYaZhong(int nEndNum);
	bool isYaZhongByArea(int nEndNum);

	void moveToTable(const cocos2d::Vec2& kFrom,const cocos2d::Vec2& kTo,int nIndex);
	void remove();
	void recycle(const cocos2d::Vec2& kTo,int nIndex);
private:
	std::vector<int>  m_kNumList;
};






