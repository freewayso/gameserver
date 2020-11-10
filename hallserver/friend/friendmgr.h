#ifndef __FRIENDMGR__H
#define __FRIENDMGR__H

#include "timer.h"
#include "singleton.h"
#include <unordered_map>
#include "friend.h"



class FriendMgr : public ITimer
{
	DECLARE_SINGLETON(FriendMgr)
public:
	FriendMgr();
	~FriendMgr();
public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);//定时器刷新定时事件
	void OnLogin(UINT64 dwActorid);
	void OnLogout(UINT64 dwActorid);
	void Init();
	void UnInit();
	void SetState(UINT64, eGameState state);
	Friend* GetFriend(UINT64 actorId);
	bool DeleteFriend(UINT64 actorId);
	void AddFriend(UINT64 actorId, Friend* pFriend);
public:
	std::unordered_map<UINT64, Friend*> m_id2friend;

};

#endif