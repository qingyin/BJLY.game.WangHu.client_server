#include "NNSoundFun.h"
#include "SimpleAudioEngine.h"
#include "Game/Script/utility.h"
#include "Game/Script/SoundFun.h"
#include "NNGameLogic.h"
namespace NNSoundFun
{

	static const std::string LG_FLAG_NC = "putong/";

	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	
	void playBackMusic(std::string kName)
	{
		SoundFun::Instance().playBackMusic(utility::a_u8(kName));
		cocos2d::log("NCMJSoundFun playBackMusic :%s",utility::a_u8(kName).c_str());
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::a_u8(LG_FLAG_NC + kName + RAW_KIND);

		SoundFun::Instance().playEffect(kName,bPreload);
		cocos2d::log("NCMJSoundFun:%s",kName.c_str());
	}

	void playCardByGender(int iGender,int nCard)
	{
	}

	void playEffectByGender(int iGender,std::string kName)
	{
		if (iGender)
		{
			kName =utility::toString("g_",kName);
		}
		else
		{
			kName =utility::toString("",kName);
		}

		playEffect(kName);
	}

	void playEffectEX(std::string kName)
	{
		SoundFun::Instance().playEffect(kName);
	}

} 
