#include "NCSetScence.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"

FV_SINGLETON_STORAGE(NCSetScence);

NCSetScence::NCSetScence(){
	init();
}
NCSetScence::~NCSetScence()
{

}
bool NCSetScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNSetScence.xml",this);
	WidgetManager::addButtonCB("Button_EffectGuan",this,button_selector(NCSetScence::Button_EffectGuan));
	WidgetManager::addButtonCB("Button_EffectKai",this,button_selector(NCSetScence::Button_EffectKai));
	WidgetManager::addButtonCB("Button_SoundGuan",this,button_selector(NCSetScence::Button_SoundGuan));
	WidgetManager::addButtonCB("Button_SoundKai",this,button_selector(NCSetScence::Button_SoundKai));
	WidgetManager::addButtonCB("Button_SheZhi",this,button_selector(NCSetScence::Button_SheZhi));
	WidgetManager::addButtonCB("Button_SetChangeAccount",this,button_selector(NCSetScence::Button_SetChangeAccount));
	
	return true;
}
void NCSetScence::EnterScence()
{
	ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(this,"HeadImagic"),
		UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());

	float fEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("Effect",1);
	float fSound = cocos2d::UserDefault::getInstance()->getFloatForKey("Sound",1);
	SoundFun::Instance().SetSoundEffect(fEffect);
	SoundFun::Instance().SetSoundEffect(fSound);
	WidgetFun::setVisible(this,"Button_EffectGuan",fEffect>0);
	WidgetFun::setVisible(this,"Button_EffectKai",!(fEffect>0));
	WidgetFun::setVisible(this,"Button_SoundGuan",(fSound>0));
	WidgetFun::setVisible(this,"Button_SoundKai",!(fSound>0));
}
void NCSetScence::hideAll()
{
	WidgetFun::setVisible(this,"SetPlane",false);
}
void NCSetScence::Button_EffectGuan(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	SoundFun::Instance().SetSoundEffect(0);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Effect",0);
}
void NCSetScence::Button_EffectKai(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	SoundFun::Instance().SetSoundEffect(1);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Effect",1);
}
void NCSetScence::Button_SoundKai(cocos2d::Ref*,WidgetUserInfo*)
{
	SoundFun::Instance().SetSoundBackground(1);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Sound",1);
}
void NCSetScence::Button_SoundGuan(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	SoundFun::Instance().SetSoundBackground(0);
	cocos2d::UserDefault::getInstance()->setFloatForKey("Sound",0);
}
void NCSetScence::Button_SheZhi(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"SetPlane",true);
	EnterScence();
}
void NCSetScence::Button_SetChangeAccount(cocos2d::Ref*,WidgetUserInfo*)
{
	MissionWeiXin::Instance().restWeiXin();
	NCScenceManager::Instance().InLogonScence();
}