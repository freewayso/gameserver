#include "pch.h"
#include "actordatawritetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "redis/redishelper.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "util.h"
#define FORMATPBDATA 100


CRedisActorDataWriteTask::CRedisActorDataWriteTask():CRedisTask("CRedisRoleDataWriteTask"),m_dwRpcDelayed(0)
{
}

CRedisActorDataWriteTask::~CRedisActorDataWriteTask()
{ 
}

void CRedisActorDataWriteTask::Execute(RedisConn* poRedisConn)
{
	UINT64 dwKey = m_oData.act().app().actid();
	std::string newstring;
	newstring.assign(m_oData.act().SerializeAsString());
	RedisResult* addResult = CRedisHelper::SetRedisString(poRedisConn, "actordata", ToString(dwKey).c_str(), newstring);
	addResult->Free();
}	 


void CRedisActorDataWriteTask::SetData(CBR::UpdateActorDataArg &oData)
{
	m_oData = oData;
}

void CRedisActorDataWriteTask::OnReply()
{
	CBR::ErrorCode nErrCode = CBR::ERR_SUCCESS;
	//CUserData oUser((UINT32)nErrCode, &roRes);
	//CBR::UpLoadAudioRes roRes;
	//if(GetRedisError() == REDIS_ERR_FAILED)
	//	nErrCode = CBR::ERR_FAILED;
	//else
	//	nErrCode = CBR::ERR_SUCCESS;
	//CUserData oUser((UINT32)nErrCode, &roRes);
	//CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}

void CRedisActorDataWriteTask::SetRpcID(const UINT32 nRpcID)
{
	m_dwRpcDelayed = nRpcID;
}
