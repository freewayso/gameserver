#include "../pch.h"
#include "friend.h"


Friend::Friend()
{
	
}
Friend::~Friend()
{
	
}

void Friend::OnLogin()
{
	
}

void Friend::OnLogout()
{
	
}

FriendInfo *Friend::getFriendInfo()
{
	return NULL;
}

void Friend::Save(CBR::Friend2Db &data)//保存好友数据
{
	
}

void Friend::Load(CBR::Friend2Db &data)//加载好友数据
{
	
}

bool Friend::DeleteApply(UINT64 actorId)// 删除申请列表
{
	return true;
}

bool Friend::AddApply(UINT64 actorId)	//增加申请
{
	return true;
	
}

bool Friend::AcceptApply(UINT64 actorId)//接受申请
{
	
	return true;
}

UINT32 Friend::RemoveFriend(UINT64 actorId)//删除好友	
{
	return true;
}