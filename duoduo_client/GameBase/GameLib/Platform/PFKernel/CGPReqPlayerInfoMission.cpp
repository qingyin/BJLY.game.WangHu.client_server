#include "CGPReqPlayerInfoMission.h"


CGPReqPlayerInfoMission::CGPReqPlayerInfoMission(const char* url, int port)
: CCallMission("CGPReqPlayerInfoMission",url, port)
{
	mIGPReqPlayerInfoSink = 0;

	addNetCall(CC_CALLBACK_2(CGPReqPlayerInfoMission::Net_queryInfo,this),SUB_GP_USER_INDIVIDUAL);
}

// 设置回调接口
void CGPReqPlayerInfoMission::setMissionSink(ICGPReqPlayerInfoSink* pReqPlayerInfoSink)
{
	mIGPReqPlayerInfoSink = pReqPlayerInfoSink;
}

void CGPReqPlayerInfoMission::query(int iAccountID)
{
	PLAZZ_PRINTF("CGPAddRankMission::GetRankAwardConfig\n");

	addLinkCallFun(CC_CALLBACK_0(CGPReqPlayerInfoMission::CB_queryInfo,this,iAccountID));

	start();
}
void CGPReqPlayerInfoMission::CB_queryInfo(int iAccountID)
{
	PLAZZ_PRINTF("CGPReqPlayerInfoMission::CB_queryInfo\n");


	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GP_QueryIndividual QueryIndividual;
	zeromemory(&QueryIndividual,sizeof(QueryIndividual));
	QueryIndividual.dwUserID = iAccountID;

	send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INDIVIDUAL,&QueryIndividual,sizeof(QueryIndividual));
}
void CGPReqPlayerInfoMission::Net_queryInfo(void* data, int dataSize)
{
	PLAZZ_PRINTF("CGPReqPlayerInfoMission::Net_queryInfo\n");

	CMD_GP_UserIndividual* pModifyIndividual = (CMD_GP_UserIndividual*)data;


	void * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pModifyIndividual+1,dataSize-sizeof(CMD_GP_UserIndividual));

	bool bUpdate = false;
	std::string kIP,kHttp,kChannel;
	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_UI_HEAD_HTTP:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_USER_NOTE)
				{
					bUpdate = true;
					kHttp.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		case DTP_GP_UI_IP:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					bUpdate = true;
					kIP.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		case DTP_GP_UI_CHANNEL:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					bUpdate = true;
					kChannel.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		}
	}
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pModifyIndividual->dwUserID == pGlobalUserInfo->getUserID())
	{
		if (kIP != "")
		{
			strncpy(pGlobalUserData->szLogonIP, (kIP.c_str()), kIP.size());
		}
		if (kHttp != "")
		{
			strncpy(pGlobalUserData->szHeadHttp, (kHttp.c_str()), kHttp.size());
		}
		if (kChannel != "")
		{
			strncpy(pGlobalUserData->szUserChannel, (kChannel.c_str()), kChannel.size());
		}
		//金币信息
		pGlobalUserInfo->upPlayerInfo();
	}

	if (bUpdate && mIGPReqPlayerInfoSink)
		mIGPReqPlayerInfoSink->onGPAccountInfoHttpIP(pModifyIndividual->dwUserID,kIP,kHttp);

	stop();

}