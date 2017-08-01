#include "CarGameScence.h"

USING_NS_CC;
class SignScenceAction : public cocos2d::ActionInterval
{
public:
	SignScenceAction()
	{

	}
	static SignScenceAction* create(float fTime)
	{
		SignScenceAction *pRet = new SignScenceAction();
		pRet->initWithDuration(fTime);
		pRet->autorelease();
		return pRet;
	}
public:
	virtual void update(float fTime)
	{
		float fValue = (sin(fTime*3.14159f-3.14159f/2.0f)+1)/2.0f;
		CarGameScence::Instance().updateAction(fValue);
	}
private:
};
void CarGameScence::updateAction(float dt)
{
	int nTempIndex = (int)(dt*(m_nEndIndex+MAX_CAR_NUM*5))%MAX_CAR_NUM;
	showCarItem(nTempIndex,1);
}
void CarGameScence::run_start()
{
	CallFunc* pFun = CallFunc::create(CC_CALLBACK_0(CarGameScence::run_end,this));
	CallFunc* pFun1 = CallFunc::create(CC_CALLBACK_0(CarGameScence::carLogoAni,this));
	CallFunc* pFun2 = CallFunc::create(CC_CALLBACK_0(CarGameScence::setGameHistory,this));
	runAction(Sequence::create(SignScenceAction::create(8),pFun,
		DelayTime::create(4.f),pFun1,DelayTime::create(2.f),pFun2,NULL));
}

void CarGameScence::run_end()
{
	addRecordIndex(m_nEndIndex);
	blinkItem(m_nEndIndex);
	setUserResoult(m_nWinScore);
	setWinScore(m_nWinScore>=0?m_nWinScore:0);
	setUserScore(m_nUserScore + m_nWinScore);
}
