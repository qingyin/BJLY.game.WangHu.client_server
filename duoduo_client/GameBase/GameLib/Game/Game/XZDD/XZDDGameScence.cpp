#include "XZDDGameScence.h"
#include "XZDDPlayer.h"
#include "Game/GameLib.h"
#include "CMD_XZDD.h"
#include "XZDDGameLogic.h"
#include "XZDDSoundFun.h"

FV_SINGLETON_STORAGE(XZDDGameScence);

XZDDGameScence::XZDDGameScence()
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_bIsHuanPai(false)
	,m_nGameState(XZDD_STATE_NULL)
	,m_nGameType(GAME_TYPE_CHENGDU)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,GameBase(0,0)
{

	init();
}
XZDDGameScence::~XZDDGameScence()
{

}
bool XZDDGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence("GameXZDD/xml/XZDDGameScence.xml",this);

	initPrivate();
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new XZDDPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("XZDDButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	defaultState();
	return true;
}
void XZDDGameScence::EnterScence()
{
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"PrivateEndPlane",false);
	WidgetFun::setVisible(this,"GameXZDDPlane",true);
	defaultState();
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->PlayerLeave();
	}

	XZDDSoundFun::playBackMusic("csmj.mp3"); 
}
bool XZDDGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameXZDDPlane");
}
void XZDDGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameXZDDPlane",false);
	defaultPrivateState();
	defaultRecordState();
}

void XZDDGameScence::ExitGame()
{
	XZDDSoundFun::stopBackMusic();
	GameBase::ExitGame();
}

void XZDDGameScence::defaultState()
{
	m_pTouchCardNode = NULL;
	m_bIsHuanPai = false;
	m_nGameState = XZDD_STATE_NULL;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
		m_pPlayer[i]->EndGame();
	}

	WidgetFun::setVisible(this,"TimeNode",false);
	WidgetFun::setVisible(this,"LastCardNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",false);
	WidgetFun::setVisible(this,"SaiZiNode",false);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"GameResoult1",false);
	WidgetFun::setVisible(this,"GameResoult2",false);
	WidgetFun::setVisible(this,"GameRoomUserInfo",false);
	WidgetFun::setVisible(this,"ExchangeCardNode",false);

	defaultPrivateState();
	defaultRecordState();
}
std::string XZDDGameScence::getStringHuRight(dword kValue,int nGenCount)
{
	dword dwRight[] = {
		CHR_SHU_FAN,CHR_TIAN_HU,CHR_DI_HU,
		CHR_DA_DUI_ZI,CHR_QING_YI_SE,CHR_QI_XIAO_DUI,CHR_DAI_YAO,CHR_JIANG_DUI,CHR_QING_DUI,
		CHR_LONG_QI_DUI,CHR_QING_QI_DUI,CHR_QING_YAO_JIU,CHR_QING_LONG_QI_DUI,
		CHR_GANG_SHANG_PAO,CHR_GANG_KAI
	};
	std::string pszRight[] = {
		ScriptData<std::string>("HuRight0").Value(),
		ScriptData<std::string>("HuRight1").Value(),
		ScriptData<std::string>("HuRight2").Value(),
		ScriptData<std::string>("HuRight3").Value(),
		ScriptData<std::string>("HuRight4").Value(),
		ScriptData<std::string>("HuRight5").Value(),
		ScriptData<std::string>("HuRight6").Value(),
		ScriptData<std::string>("HuRight7").Value(),
		ScriptData<std::string>("HuRight8").Value(),
		ScriptData<std::string>("HuRight9").Value(),
		ScriptData<std::string>("HuRight10").Value(),
		ScriptData<std::string>("HuRight11").Value(),
		ScriptData<std::string>("HuRight12").Value(),
		ScriptData<std::string>("HuRight13").Value(),
		ScriptData<std::string>("HuRight14").Value(),
	};
	std::string kTxt;
	XZDD::CChiHuRight	kChiHuRight;	
	kChiHuRight.SetRightData(&kValue,MAX_RIGHT_COUNT );
	for( BYTE j = 0; j < CountArray(pszRight); j++ )
	{
		if( !(kChiHuRight&dwRight[j]).IsEmpty() )
		{
			kTxt+=pszRight[j];
		}
	}
	if (nGenCount>0)
	{
		kTxt += utility::toString(ScriptData<std::string>("HuRight_Gen").Value(),nGenCount,ScriptData<std::string>("HuRight_Fan").Value());
	}
	return kTxt;
}
void XZDDGameScence::showSaiZi(unsigned int iValue)
{
	word wSice1 = word(iValue>>16);
	word wSice2 = word(iValue);
	//if (wSice1 > 0)
	//{
	//	BYTE SiceFirst = (wSice1 >> 8);
	//	BYTE SiceSecond = (wSice1);
	//	std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
	//	WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
	//	WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
	//	WidgetFun::setWidgetUserInfo(this,"SaiZiNode","NextValue",utility::toString((int)wSice2));
	//	WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart1");
	//}
	//else if (wSice2 > 0)
	{
		BYTE SiceFirst = (wSice2 >> 8);
		BYTE SiceSecond = (wSice2);
		std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
		WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
		WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
		WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart2");
		getPlayerByChairID(m_iBankerUser)->setZhuang();
	}
}

void XZDDGameScence::showSaiZi_HuanPai(word wValue)
{
	BYTE SiceFirst = (wValue >> 8);
	BYTE SiceSecond = (wValue);
	std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
	WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
	WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
	WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart_HuanPai");
}

void XZDDGameScence::setCurrentPlayer(int iCurrentPlayer,int iUserAction,int icbCardData)
{
	m_iCurrentUser = iCurrentPlayer;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");

	XZDDPlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->runAction(
		cocos2d::MoveExTxtTime::create(TIME_OPERATE_CARD));
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	if (iUserAction == WIK_NULL)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",pPlyer->getIdex()),true);
	}

	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		WidgetFun::setVisible(this,"SelfActionNode",true);
		int iCout = 0;
		int iStartPos = 0;
		int iNextPos = 0;
		if (BYTE(iUserAction)&WIK_GANG)
		{
			iCout++;
		}
		if (BYTE(iUserAction)&WIK_PENG)
		{
			iCout++;
		}
		if (BYTE(iUserAction)&BYTE(iUserAction)&WIK_CHI_HU || BYTE(iUserAction)&WIK_ZI_MO)
		{
			iCout++;
		}
		if (iCout == 1)
		{
			iStartPos = -140;
			iNextPos = 280;
		}
		if (iCout == 2)
		{
			iStartPos = -200;
			iNextPos = 280;
		}
		if (iCout == 3)
		{
			iStartPos = -300;
			iNextPos = 280;
		}
		if (BYTE(iUserAction)&WIK_CHI_HU || BYTE(iUserAction)&WIK_ZI_MO)
		{
			WidgetFun::setPos(this,"XZDDButton_HuAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (BYTE(iUserAction)&WIK_GANG)
		{
			WidgetFun::setPos(this,"XZDDButton_HuAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (BYTE(iUserAction)&WIK_PENG)
		{
			WidgetFun::setPos(this,"XZDDButton_PengAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		WidgetFun::setPos(this,"XZDDButton_GuoAction",cocos2d::Vec2(iStartPos,0));

		WidgetFun::setVisible(this,"XZDDButton_GangAction",BYTE(iUserAction)&WIK_GANG);
		WidgetFun::setVisible(this,"XZDDButton_PengAction",BYTE(iUserAction)&WIK_PENG);
		WidgetFun::setVisible(this,"XZDDButton_HuAction",BYTE(iUserAction)&WIK_CHI_HU || BYTE(iUserAction)&WIK_ZI_MO);
		WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString(icbCardData));
	}
}
void XZDDGameScence::setGameResoultStateInfo(cocos2d::Node* pNode,int iIdex,std::string kName,std::string kDes,int lSocre)
{
	cocos2d::Node* pStatusNode = WidgetFun::getChildWidget(pNode,utility::toString("GameResoultInfo",iIdex));
	WidgetFun::setText(pStatusNode,"Name",kName);
	WidgetFun::setText(pStatusNode,"Des",kDes);
	if (lSocre>0)
	{
		WidgetFun::setText(pStatusNode,"GoldTxt",utility::toString("+",lSocre));
	}
	else
	{
		WidgetFun::setText(pStatusNode,"GoldTxt",utility::toString(lSocre));
	}
	pStatusNode->setVisible(true);
}

void XZDDGameScence::showClock(int iTime,bool bShow /*= true*/)
{
	iTime = iTime+1;
	WidgetFun::setVisible(this,"ActPlayerLastTime",bShow);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"ActPlayerLastTime");
	if (!bShow)
	{
		pNode->setVisible(false);
		return;
	}
	pNode->setVisible(true);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTime::create(iTime,iTime,0));
}

void XZDDGameScence::showClockTimeCallBack( const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}

void XZDDGameScence::killClock( const std::string& kName )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
}


void XZDDGameScence::setGameResoultPlayerInfo(CMD_S_GameEnd* pGameEnd,XZDDPlayer* pPlayer,cocos2d::Node* pNode)
{
	for (int i = 0;i<6;i++)
	{
		WidgetFun::setVisible(pNode,utility::toString("GameResoultInfo",i),false);
	}
	int iChirID = pPlayer->GetChairID();
	int iIdex = 0;
	if (pGameEnd->lGangScore[iChirID] != 0)
	{
		setGameResoultStateInfo(pNode,iIdex,m_pPlayer[iChirID]->GetNickName(),utility::getScriptString("GFXY"),pGameEnd->lGangScore[iChirID]);
		iIdex++;
	}

	for (int i=0;i<MAX_PLAYER;i++)
	{
		if (iChirID == i || pGameEnd->wLostFanShu[i][iChirID] == 0 )
		{
			continue;
		}
		std::string kTemp;
		LONGLONG lScoreTemp = 0;

		if (pGameEnd->lChaJiaoScore[i] !=0 )
		{
			kTemp = ScriptData<std::string>("HuRight_ChaDaJiao").Value();
			lScoreTemp = -pGameEnd->lChaJiaoScore[i];
		}
		else if(pGameEnd->lHuaZhuScore[i] !=0)
		{
			kTemp = ScriptData<std::string>("HuRight_ChaHuaZhu").Value();
			lScoreTemp = -pGameEnd->lHuaZhuScore[i];
		}
		else
		{
			if (pGameEnd->wProvideUser[i] == i)
			{
				kTemp = ScriptData<std::string>("HuRight_ZiMo").Value();
			}
			else
			{
				kTemp = ScriptData<std::string>("HuRight_PingHu").Value();
			}
			kTemp += getStringHuRight(pGameEnd->dwChiHuRight[i],pGameEnd->cbGenCount[i]);
			lScoreTemp = pGameEnd->wLostFanShu[i][iChirID];
		}
		setGameResoultStateInfo(pNode,iIdex,m_pPlayer[i]->GetNickName(),kTemp,lScoreTemp);
		iIdex++;
	}

	// 自己赢的总分
	word selfChairID =  m_pLocal->GetChairID();
	LONGLONG lSelfSocre  = 0;
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		if (i== iChirID)
		{
			continue;
		}
		lSelfSocre += pGameEnd->wLostFanShu[iChirID][i];
	}


	std::string kTemp;
	LONGLONG lScoreTemp = 0;

	if (pGameEnd->lChaJiaoScore[iChirID] !=0 )
	{
		kTemp = ScriptData<std::string>("HuRight_ChaDaJiao").Value();
		lScoreTemp = -pGameEnd->lChaJiaoScore[iChirID];
	}
	else if(pGameEnd->lHuaZhuScore[iChirID] !=0)
	{
		kTemp = ScriptData<std::string>("HuRight_ChaHuaZhu").Value();
		lScoreTemp = -pGameEnd->lHuaZhuScore[iChirID];
	}
	else if(lSelfSocre !=0)
	{
		if (pGameEnd->wProvideUser[iChirID] == selfChairID)
		{
			kTemp = ScriptData<std::string>("HuRight_ZiMo").Value();
		}
		else
		{
			kTemp = ScriptData<std::string>("HuRight_PingHu").Value();
		}
		kTemp += getStringHuRight(pGameEnd->dwChiHuRight[iChirID],pGameEnd->cbGenCount[iChirID]);
		lScoreTemp = -lSelfSocre;
	}

	if (lScoreTemp != 0)
	{
		setGameResoultStateInfo(pNode,iIdex,m_pPlayer[iChirID]->GetNickName(),kTemp,lScoreTemp);
		iIdex++;
	}


	lSelfSocre  = pGameEnd->lGameScore[selfChairID];
	WidgetFun::setVisible(this,"WinTitle",lSelfSocre >= 0);
	WidgetFun::setVisible(this,"LoseTitle",lSelfSocre < 0);
}

void XZDDGameScence::XuanQueTimeOut()
{
	CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();

	cocos2d::log("Send XuanQue Color int :%d",(int)kInfo.nQueChoice);
	cocos2d::log("Send XuanQue Color byty:%d",kInfo.nQueChoice);

	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));

}

void XZDDGameScence::DoXuanQueNotice()
{
	showClockTimeCallBack("ActPlayerLastTime",m_nLeftTime,CC_CALLBACK_0(XZDDGameScence::XuanQueTimeOut,this),0);
	for (int i=0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showXuanQue(true);
		m_pPlayer[i]->blackAllHandCard(true);
	}
}


void XZDDGameScence::showRoomUserInfo( int nIndex )
{
	ASSERT(nIndex >=0 && nIndex<MAX_PLAYER);
	if (nIndex <0 || nIndex>=MAX_PLAYER)
	{
		return;
	}

	WidgetFun::setText(this,"RoomUser_name",m_pPlayer[nIndex]->GetNickName());
	WidgetFun::setText(this,"RoomUser_ID",m_pPlayer[nIndex]->GetUserID());
	WidgetFun::setText(this,"RoomUser_Gold",m_pPlayer[nIndex]->GetUserScore());
	int nGender = m_pPlayer[nIndex]->GetGender();

	if (nGender == 0)
	{
		WidgetFun::setVisible(this,"RoomUser_gender0",true);
		WidgetFun::setVisible(this,"RoomUser_gender1",false);
	}
	else
	{
		WidgetFun::setVisible(this,"RoomUser_gender0",false);
		WidgetFun::setVisible(this,"RoomUser_gender1",true);
	}
}

void XZDDGameScence::setGameType(int nType)
{
	ASSERT(nType == GAME_TYPE_SICHUAN || nType == GAME_TYPE_CHENGDU);
	m_nGameType = nType;
}

void XZDDGameScence::defaultPlayerActionState()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}