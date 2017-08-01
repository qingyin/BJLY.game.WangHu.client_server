#include "FZMJGameScence.h"
#include "Game/GameLib.h"
#include "FZMJPlayer.h"
#include "FZMJSoundFun.h"
#include "Game/Widget/SliderMenu.h"


void FZMJGameScence::initSetButton()
{
	if (WidgetFun::getChildWidget(this,"GameSetPlane") == NULL)
	{
		return;
	}
	WidgetManager::addButtonCB("HNMJ_SetBarSider1",this,button_selector(FZMJGameScence::SetBarSider1));
	WidgetManager::addButtonCB("HNMJ_SetBarSider2",this,button_selector(FZMJGameScence::SetBarSider2));
	WidgetManager::addButtonCB("HNMJ_Button_HideSet",this,button_selector(FZMJGameScence::Button_HideSet));
	WidgetManager::addButtonCB("HNMJ_Button_GameSet",this,button_selector(FZMJGameScence::Button_GameSet));
	WidgetManager::addButtonCB("HNMJ_Button_Cheak_ChangSha",this,button_selector(FZMJGameScence::HNMJ_Button_Hua));
	WidgetManager::addButtonCB("HNMJ_Button_Cheak_PuTong",this,button_selector(FZMJGameScence::HNMJ_Button_Hua));
	WidgetManager::addButtonCB("HNMJ_Button_GameSet",this,button_selector(FZMJGameScence::Button_GameSet));
	

	std::string kIdex = cocos2d::UserDefault::getInstance()->getStringForKey("Hua","1");
	if (kIdex == "0")
	{
		FZMJSoundFun::setPuTong(false);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_ChangSha",true);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_PuTong",false);
	}
	else
	{
		FZMJSoundFun::setPuTong(true);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_ChangSha",false);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_PuTong",true);
	}
	{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar1",0.7f);
		SoundFun::Instance().SetSoundEffect(fTime);
	}
	{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar2",0.7f);
		SoundFun::Instance().SetSoundBackground(fTime); 
	}
}
void FZMJGameScence::HNMJ_Button_Hua(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kIdex = WidgetFun::getUserInfoValue(pUserInfo,"Idex");
	if (kIdex == "0")
	{
		FZMJSoundFun::setPuTong(false);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_ChangSha",true);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_PuTong",false);
	}
	else
	{
		FZMJSoundFun::setPuTong(true);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_ChangSha",false);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_PuTong",true);
	}
	cocos2d::UserDefault::getInstance()->setStringForKey("Hua",kIdex);
}
void FZMJGameScence::SetBarSider1(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>((Node*)pNode);
	SoundFun::Instance().SetSoundEffect(pSlider->getValue()/pSlider->getMaxValue());
	cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar1",pSlider->getValue()/100.0f);
}
void FZMJGameScence::SetBarSider2(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>((Node*)pNode);
	SoundFun::Instance().SetSoundBackground(pSlider->getValue()/pSlider->getMaxValue());
	cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar2",pSlider->getValue()/100.0f);
}
void FZMJGameScence::Button_HideSet(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	cocos2d::Node* pRootNode = WidgetManager::Instance().getBaseNode((cocos2d::Node*)pNode);
	{
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider1"));
		cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar1",pSlider->getValue()/100.0f);
	}
	{
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider2"));
		cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar2",pSlider->getValue()/100.0f);
	}
}
void FZMJGameScence::Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	cocos2d::Node* pRootNode = this;
	{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar1",0.7f);
		fTime =fTime*100;
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider1"));
		pSlider->setActValue(fTime);
		WidgetFun::setProgress(pRootNode,"SetBar1",fTime,100);
		SoundFun::Instance().SetSoundEffect(fTime/pSlider->getMaxValue());
	}

	{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar2",0.7f);
		fTime =fTime*100;
		cocos2d::SliderMenu* pSlider = WidgetFun::castNode<cocos2d::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider2"));
		pSlider->setActValue(fTime);
		WidgetFun::setProgress(pRootNode,"SetBar2",fTime,100);
		SoundFun::Instance().SetSoundBackground(fTime/pSlider->getMaxValue());
	}
}