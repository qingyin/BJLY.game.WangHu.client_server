#include "CGPLowProtectMission.h"

#define MISSION_REQUERK_LOW_NULL	0
#define MISSION_REQUICK_LOW__LOAD	1
#define MISSION_REQUICK_LOW_TAKE	2

CGPLowProtectMission::CGPLowProtectMission(const char * url, int port)
: CSocketMission(url, port)
{
	mIGPLowProtectMissionSink = 0;
	mMissionType = MISSION_REQUERK_LOW_NULL;
}

void CGPLowProtectMission::setMissionSink(IGPLowProtectMissionLink* pIGPLowProtectMissionSink)
{
	mIGPLowProtectMissionSink = pIGPLowProtectMissionSink;
}

void CGPLowProtectMission::requickLowProtectInfo()
{
	mMissionType = MISSION_REQUICK_LOW__LOAD;
	start();
}

void CGPLowProtectMission::obtainLowProtect()
{
	mMissionType = MISSION_REQUICK_LOW_TAKE;
	start();
}

void CGPLowProtectMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketLink\n");

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{
		case MISSION_REQUICK_LOW__LOAD:
		{
			send(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_LOAD, NULL, 0);
			break;
		}
		case MISSION_REQUICK_LOW_TAKE:
		{
			CMD_GP_BaseEnsureTake kNetInfo;
			kNetInfo.dwUserID = pGlobalUserData->dwUserID;
			strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
			strncpy(kNetInfo.szMachineID,DF::shared()->GetMachineID(),countarray(kNetInfo.szMachineID));
			send(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_TAKE, &kNetInfo, sizeof(kNetInfo));
			break;
		}
		default:
		break;
	}
}

void CGPLowProtectMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketShut\n");


}

void CGPLowProtectMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPLowProtectMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, dataSize);

	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}
	switch (sub)
	{
		case SUB_GP_BASEENSURE_PARAMETER:
		{
			return onRequickLowProtectInfo(data, dataSize);
		}
		case SUB_GP_BASEENSURE_RESULT:
		{
		
			return  onObtainLowProtect(data, dataSize);
		}
	default:
		break;
	}
	return true;
}


bool CGPLowProtectMission::onRequickLowProtectInfo(void * data, int size)
{
	PLAZZ_PRINTF("CGPLowProtectMission::onRequickLowProtectInfo");
	if (size != sizeof(CMD_GP_BaseEnsureParamter))
	{
		ASSERT(false);
		return false;
	}
	CMD_GP_BaseEnsureParamter* pNetInfo = (CMD_GP_BaseEnsureParamter*)data;

	stop();

	if (mIGPLowProtectMissionSink)
		mIGPLowProtectMissionSink->onLowProtectQueryInfoResult(pNetInfo->lScoreCondition, pNetInfo->lScoreAmount, pNetInfo->cbTakeTimes);
	return true;
}

bool CGPLowProtectMission::onObtainLowProtect(void * data, int size)
{
	PLAZZ_PRINTF("CGPLowProtectMission::onRequickLowProtectInfo");

	CMD_GP_BaseEnsureResult* pNetInfo = (CMD_GP_BaseEnsureResult*)data;

	CGlobalUserInfo * pGlobalUserInfo   = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pUserData=pGlobalUserInfo->GetGlobalUserData();

	//更新
	if (pNetInfo->bSuccessed)
	{
		pUserData->lUserScore = pNetInfo->lGameScore;
		pGlobalUserInfo->upPlayerInfo();
	}

	if (mIGPLowProtectMissionSink)
		mIGPLowProtectMissionSink->onLowProtectDoneResult(pNetInfo->bSuccessed, pNetInfo->lGameScore, pNetInfo->szNotifyContent);

	return true;
}
