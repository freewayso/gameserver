#ifndef __FRIEND_H
#define __FRIEND_H

#include <string>
#include "pb/project.pb.h"


enum eGameState
{
	eGameIdleState = 1,
	eGameTeamState = 2,
	eGameInGameState = 3,
	eGame7DState  = 4,
	eGame2HState  = 5,	
};


class Apply
{
	UINT64 dwActorId;		//申请用户id
	std::string szName;		//申请用户名字
	UINT32 dwTime;			//申请时间
	UINT32 dwIcon;			//固定ICON
};

class Backlist
{
	UINT64 dwActorId;		//申请用户id
	std::string szName;		//申请用户名字
	UINT32 dwTime;			//黑名单时间
	UINT32 dwIcon;			//固定ICON
};

struct FriendInfo
{
	bool m_bChange;	//存档标签
	UINT64 m_dwActorId;
	std::string m_szName;
	eGameState m_eState;
	UINT32 dwTime;			
};


class Friend
{
public:
	Friend();
	~Friend();
	
public:
	void OnLogin();
	void OnLogout();
	FriendInfo *getFriendInfo();
	void Save(CBR::Friend2Db &data);//保存好友数据
	void Load(CBR::Friend2Db &data);//加载好友数据
	bool DeleteApply(UINT64 actorId);// 删除申请列表
	bool AddApply(UINT64 actorId);	//增加申请
	bool AcceptApply(UINT64 actorId);//接受申请
	UINT32 RemoveFriend(UINT64 actorId);//删除好友			
private:
	FriendInfo *m_pFriendInfo; //玩家自己的信息
	std::vector<Apply *> m_vecApply;	   //申请列表
	std::vector<Backlist *> m_vecBacklist; //黑名单
	std::vector<FriendInfo *> m_vecFriendInfo; //其他好友信息
	
};



#endif

