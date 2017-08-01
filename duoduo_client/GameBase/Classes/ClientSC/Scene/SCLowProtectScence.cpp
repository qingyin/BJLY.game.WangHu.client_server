#include "SCLowProtectScence.h"
#include "SCScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(SCLowProtectScence);

SCLowProtectScence::SCLowProtectScence()
	:m_kLowProtectMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_lScoreCondition(0)
	,m_cbTakeTimes(0)
{
	init();
	m_kLowProtectMission.setMissionSink(this);
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(SCLowProtectScence::LogonSucess));
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(SCLowProtectScence::upPlayerInfo));
}
SCLowProtectScence::~SCLowProtectScence()
{

}
bool SCLowProtectScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	
	return true;
}
void SCLowProtectScence::LogonSucess()
{
	m_kLowProtectMission.requickLowProtectInfo();
}
void SCLowProtectScence::upPlayerInfo()
{
	utility::log("SCLowProtectScence::upPlayerInfo()");
	if (UserInfo::Instance().getUserScore() < m_lScoreCondition)
	{
		m_kLowProtectMission.obtainLowProtect();
	}
}
void SCLowProtectScence::onLowProtectQueryInfoResult(SCORE lScoreCondition, SCORE lScoreAmount, byte cbTakeTimes)
{
	m_lScoreCondition = lScoreCondition;
	m_cbTakeTimes = cbTakeTimes;

	upPlayerInfo();
}
void SCLowProtectScence::onLowProtectDoneResult(bool bSuccessed, SCORE lGameScore, const char* szDescription)
{
	if (bSuccessed)
	{
		EffectScence::Instance().ShowGoldEffect();
		PopScence::Instance().showAccessPlane(utility::a_u8(szDescription),this,NULL);
	}
}