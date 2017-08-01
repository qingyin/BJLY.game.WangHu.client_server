#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CSocketMission.h"


///< 回调
class IGPLowProtectMissionLink
{
public:
	virtual ~IGPLowProtectMissionLink(){}
	virtual void onLowProtectQueryInfoResult(SCORE lScoreCondition, SCORE lScoreAmount, byte cbTakeTimes) {};
	virtual void onLowProtectDoneResult(bool bSuccessed, SCORE lGameScore, const char* szDescription) {};
};

class CGPLowProtectMission : public CSocketMission
{
public:
	CGPLowProtectMission(const char * url, int port);
	// 设置回调接口
	void setMissionSink(IGPLowProtectMissionLink* pIGPLowProtectMissionSink);
	///< 请求账户低保信息
	void requickLowProtectInfo();
	///< 请求领取低保
	void obtainLowProtect();
	// ISocketEngineSink//这个是虚方法
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);


private:
	///< 获取低保信息
	bool onRequickLowProtectInfo(void * data, int size);
	///< 获取低保成功
	bool onObtainLowProtect(void * data, int size);
private:
	// 签到类型
	uint8 mMissionType;	

private:
	// 回调
	IGPLowProtectMissionLink* mIGPLowProtectMissionSink;
};