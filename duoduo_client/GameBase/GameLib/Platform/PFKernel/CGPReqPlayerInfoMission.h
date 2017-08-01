#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class ICGPReqPlayerInfoSink
{
public:
	virtual ~ICGPReqPlayerInfoSink(){}
	virtual void onGPAccountInfoHttpIP(dword dwUserID, std::string strIP,std::string strHttp){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPReqPlayerInfoMission
	: public CCallMission
{
public:
	CGPReqPlayerInfoMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(ICGPReqPlayerInfoSink* pReqPlayerInfoSink);

	void query(int iAccountID);
	void CB_queryInfo(int iAccountID);
	void Net_queryInfo(void* data, int dataSize);

private:
	// 回调
	ICGPReqPlayerInfoSink* mIGPReqPlayerInfoSink;
}; 
