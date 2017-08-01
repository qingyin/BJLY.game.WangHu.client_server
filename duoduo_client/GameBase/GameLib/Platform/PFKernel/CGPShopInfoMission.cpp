#include "CGPShopInfoMission.h"


CGPShopInfoMission::CGPShopInfoMission(const char* url, int port)
: CCallMission("CGPShopInfoMission",url, port)
{
	mIGPShopInfoMissionSink = 0;

	addNetCall(CC_CALLBACK_2(CGPShopInfoMission::Net_ShopInfoList,this),SUB_GP_SHOPINFO_BACK);
}

// 设置回调接口
void CGPShopInfoMission::setMissionSink(CGShopInfoMissionSink* pIGPKefuMissionSink)
{
	mIGPShopInfoMissionSink = pIGPKefuMissionSink;
}

void CGPShopInfoMission::GetShopInfoList()
{
	addLinkCallFun(CC_CALLBACK_0(CGPShopInfoMission::CB_GetShopInfoList,this));
	start();
}

void CGPShopInfoMission::CB_GetShopInfoList()
{
	CMD_GP_GetShopInfo kNetInfo;
	kNetInfo.dwUserID = CGlobalUserInfo::getUserID();
	strncpy(kNetInfo.szPassword,CGlobalUserInfo::getPassword(),countarray(kNetInfo.szPassword));
	send(MDM_GP_USER_SERVICE, SUB_GP_SHOPINFO_GET_LIST_INFO,&kNetInfo,sizeof(CMD_GP_GetShopInfo));
}

void CGPShopInfoMission::Net_ShopInfoList( void* data, int dataSize )
{
	PLAZZ_PRINTF("CGPShopInfoMission::Net_ShopInfoList\n");

	datastream kStream(data,dataSize);
	CMD_GP_BackShopInfo kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	stop();

	if (mIGPShopInfoMissionSink)
	{
		mIGPShopInfoMissionSink->onGPShopInfoListResult(&kNetInfo);
	}
}
