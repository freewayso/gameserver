#include "pch.h"
#include "actordatareadtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "redis/redishelper.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "cache/dbeventconv.h"


CRedisActorReadTask::CRedisActorReadTask():CRedisTask("CRedisActorReadTask"),m_dwRpcDelayed(0)
{
	m_dwEventId = 0;
}

CRedisActorReadTask::~CRedisActorReadTask()
{ 
}

void CRedisActorReadTask::Execute(RedisConn* poRedisConn)
{
	std::vector<std::string> vecUid;
	vecUid.push_back(ToString(m_oArg.actorid()));

	SetRedisError(REDIS_ERR_FAILED);
	RedisResult* result = CRedisHelper::MGetRedisString(poRedisConn, "actordata", vecUid);
	int size = result->GetElementsSize();
	if (size != vecUid.size())
	{
		result->Free();
		GetErrStream() << "ErrDesc: " << "wrong size,"<<size<<","<<vecUid.size() << "," << poRedisConn->GetError()<< ", ErrNo: " << poRedisConn->GetErrno();
		return;
	}
	bool flag = false;
	std::string tempStr;
	for (int i = 0; i < size; ++i)
	{
		if(result->GetElement(i, tempStr))
		{
			if (m_oData.add_actorlist()->ParseFromString(tempStr))
			{
				flag = true;
			}else
			{
				LogError("parse stream error actorid=%s", vecUid[i].c_str());
				break;
			}		
		}
	}
	if (flag)
	{
		m_oData.set_error(CBR::ERR_SUCCESS);
	}else
	{
		m_oData.set_error(CBR::ERR_REDIS_NOTDATA);
	}
	result->Free();
	SetRedisError(REDIS_ERR_SUCESS);
}

void CRedisActorReadTask::SetData(const CBR::ActorDataArg &oData)
{
	m_oArg = oData;
}

void CRedisActorReadTask::OnReply()
{
	CBR::ErrorCode nErrCode = CBR::ERR_SUCCESS;
	if(GetRedisError() == REDIS_ERR_FAILED)
	{
		nErrCode = CBR::ERR_FAILED;
	}

	CUserData oUser(NULL, &m_oData);
	DbEventConvMgr::Instance()->OnRedisEvent(m_dwEventId,oUser);
	
}

void CRedisActorReadTask::SetRpcID(const UINT32 nRpcID)
{
	m_dwRpcDelayed = nRpcID;
}

void CRedisActorReadTask::SetEventId(const UINT64 dwEventId)
{
	m_dwEventId = dwEventId;
}
