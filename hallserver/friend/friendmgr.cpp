#include "../pch.h"

#include "friendmgr.h"

INSTANCE_SINGLETON(FriendMgr)


void FriendMgr::Init()
{
}

void FriendMgr::UnInit()
{
	
}

void FriendMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	
}

void FriendMgr::OnLogin(UINT64 dwActorid)
{
	
}

void FriendMgr::OnLogout(UINT64 dwActorid)
{
	
}


void FriendMgr::SetState(UINT64, eGameState state)
{
	
}

Friend* FriendMgr::GetFriend(UINT64 actorId)
{
	return NULL;
}

bool FriendMgr::DeleteFriend(UINT64 actorId)
{
	return true;
}

void FriendMgr::AddFriend(UINT64 actorId, Friend* pFriend)
{
	
}