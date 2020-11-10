#include "pch.h"
#include "actorreadtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "redis/redishelper.h"
#include "util/gametime.h"
#include "util/XCommon.h"


CRedisActorReadTask::CRedisActorReadTask():CRedisTask("CRedisActorReadTask"),m_dwRpcDelayed(0)
{
}

CRedisActorReadTask::~CRedisActorReadTask()
{ 
}

void CRedisActorReadTask::Execute(RedisConn* poRedisConn)
{
	std::vector<std::string> vecUid;
	for (auto iter = m_vecActorData.begin(); iter!=m_vecActorData.end();iter++)
	{
		vecUid.push_back(ToString(iter->app().actid()));
	}
	SetRedisError(REDIS_ERR_FAILED);
	RedisResult* result = CRedisHelper::MGetRedisString(poRedisConn, "actordata", vecUid);
	int size = result->GetElementsSize();
	if (size != vecUid.size())
	{
		result->Free();
		GetErrStream() << "ErrDesc: " << "wrong size,"<<size<<","<<vecUid.size() << "," << poRedisConn->GetError()<< ", ErrNo: " << poRedisConn->GetErrno();
		return;
	}
	std::string tempStr;
	for (int i = 0; i < size; ++i)
	{
		if(result->GetElement(i, tempStr))
		{
			CBR::ActorBase info;
			if (info.ParseFromString(tempStr))
			{
				m_vecActorData.push_back(info);
			}
		}
	}
	result->Free();
	SetRedisError(REDIS_ERR_SUCESS);
}



void CRedisActorReadTask::OnReply()
{
	CBR::ErrorCode nErrCode = CBR::ERR_SUCCESS;
	if(GetRedisError() == REDIS_ERR_FAILED)
	{
		nErrCode = CBR::ERR_FAILED;
	}
	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
	//直接改造成异步回调..


}

void CRedisActorReadTask::SetRpcID(const UINT32 nRpcID)
{
	m_dwRpcDelayed = nRpcID;
}

void CRedisActorReadTask::SetQueryParam(std::vector<UINT64> &vec)
{
	m_vecActorId = vec;
}
