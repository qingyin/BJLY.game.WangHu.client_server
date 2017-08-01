#pragma once
#include "cocos2d.h"
#include "Kernel/kernel/user/IClientUserItem.h"

class GamePlayer
{
public:
	GamePlayer(IClientUserItem* pUserItem);
	~GamePlayer();
public:
	void setUserItem(IClientUserItem* pItem);
	IClientUserItem* getUserItem(bool bAssert = true);
public:
	word GetTableID();
	word GetChairID();
	byte GetUserStatus();
	SCORE GetUserScore();
	word GetFaceID();
	byte GetGender();
	dword GetUserID();
	dword GetGameID();
	std::string GetHeadHttp();
	//用户昵称
	std::string GetNickName();
	//用户信息
	tagUserInfo * GetUserInfo();
public:
	virtual void PlayerEnter(){};
	virtual void PlayerLeave(){};
	virtual void upPlayerInfo(){};
	virtual void upPlayerState(){};
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo){}
protected:
	IClientUserItem* m_pUserItem;
};