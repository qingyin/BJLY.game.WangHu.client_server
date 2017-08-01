#include "HNSetScence.h"
#include "HNScenceManager.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"

FV_SINGLETON_STORAGE(HNSetScence);

HNSetScence::HNSetScence(){
	init();
}
HNSetScence::~HNSetScence()
{

}
bool HNSetScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNSetScence.xml",this);
	WidgetManager::addButtonCB("Button_EffectGuan",this,button_selector(HNSetScence::Button_EffectGuan));
	WidgetManager::addButtonCB("Button_EffectKai",this,button_selector(HNSetScence::Button_EffectKai));
	WidgetManager::addButtonCB("Button_SoundGuan",this,button_selector(HNSetScence::Button_SoundGuan));
	WidgetManager::addButtonCB("Button_SoundKai",this,button_selector(HNSetScence::Button_SoundKai));
	WidgetManager::addButtonCB("Button_SheZhi",this,button_selector(HNSetScence::Button_SheZhi));
	WidgetManager::addButtonCB("Button_SetChangeAccount",this,button_selector(HNSetScence::Button_SetChangeAccount));
	
	return true;
}
void HNSetScence::EnterScence()
{
	ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(this,"HeadImagic"),
		UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());

	bool bEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("Effect",1);
	bool bSound = cocos2d::UserDefault::getInstance()->getFloatForKey("Sound",1);
	WidgetFun::setVisible(this,"Button_EffectGuan",bEffect);
	WidgetFun::setVisible(this,"Button_EffectKai",!bEffect);
	WidgetFun::setVisible(this,"Button_SoundGuan",bSound);
	WidgetFun::setVisible(this,"Button_SoundKai",!bSound);
}
void HNSetScence::hideAll()
{
	WidgetFun::setVisible(this,"SetPlane",false);
}
void HNSetScence::Button_EffectGuan(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	SoundFun::Instance().SetSoundEffect(0);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Effect",0);
}
void HNSetScence::Button_EffectKai(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	SoundFun::Instance().SetSoundEffect(1);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Effect",1);
}
void HNSetScence::Button_SoundKai(cocos2d::Ref*,WidgetUserInfo*)
{
	SoundFun::Instance().SetSoundBackground(1);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Sound",1);
}
void HNSetScence::Button_SoundGuan(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	SoundFun::Instance().SetSoundBackground(0);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Sound",0);
}
void HNSetScence::Button_SheZhi(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"SetPlane",true);
	EnterScence();
}
void HNSetScence::Button_SetChangeAccount(cocos2d::Ref*,WidgetUserInfo*)
{
	MissionWeiXin::Instance().restWeiXin();
	HNScenceManager::Instance().InLogonScence();
}