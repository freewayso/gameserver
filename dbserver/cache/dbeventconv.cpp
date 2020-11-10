#include "pch.h"
#include "dbeventconv.h"
#include "timeutil.h"
#include "redis/redismgr.h"
#include "redis/redistask/actordatareadtask.h"
#include "mysql/task/actorquerytask.h"
#include "mysql/mysqlmgr.h"
#include "crpc.h"
#include "cache/memorysyncdb.h"




INSTANCE_SINGLETON(DbEventConvMgr)


DbEventConvMgr::DbEventConvMgr()
{
	m_dwId = 0;
}

DbEventConvMgr::~DbEventConvMgr()
{

}

bool DbEventConvMgr::Init()
{
	return true;
}

void DbEventConvMgr::Uninit()
{
}

UINT64 DbEventConvMgr::CreateUuid()
{
	m_dwId++;
	UINT32 HiDWord = TimeUtil::GetTime();
	UINT32 LoDWord = m_dwId;
	return ((UINT64)HiDWord << 32 | LoDWord);
}

void DbEventConvMgr::AddEvent(DbReadEvent *pEvent,const CBR::ActorDataArg &oData)
{
	UINT64 qwId = CreateUuid();
	pEvent->qwPostTime = TimeUtil::GetTime();
	pEvent->oArg.CopyFrom(oData);
	pEvent->qwUid = m_dwId;
	m_mapEvents[qwId] = pEvent;
	CRedisActorReadTask *pRedisTask =  new CRedisActorReadTask();
	pRedisTask->SetEventId(qwId);
	pRedisTask->SetData(oData);
	CRedisMgr::Instance()->PushTask(pRedisTask);
}

void DbEventConvMgr::OnMysqlEvent(UINT64 qwEventId,const CUserData &userData)
{
	auto iter = m_mapEvents.find(qwEventId);
	if(iter != m_mapEvents.end())
	{
		DbReadEvent *pInfo = iter->second;
		CBR::ActorDataRes *pRes = (CBR::ActorDataRes *)userData.m_pUserPtr;
		if (pRes->error() == CBR::ERR_SUCCESS)
		{
			if(pRes->actorlist_size() >= 1)
			{
				MemorySyncDb::Instance()->SyncData2Redis(pRes->actorlist(0),false);
			}
		}
		CRpc::ReplyDelayRpc(iter->second->dwRpcId, userData);
		delete pInfo;
		m_mapEvents.erase(iter);
	}else
	{
		LogError("Event sucess [%llu] are lost ", qwEventId);
	}

}

void DbEventConvMgr::OnRedisEvent(UINT64 qwEventId,const CUserData &userData)
{
	CBR::ActorDataRes *pRes =  (CBR::ActorDataRes *)(userData.m_pUserPtr);
	auto iter = m_mapEvents.find(qwEventId);
	if (iter == m_mapEvents.end())
	{
		LogError("Event eventid=[%llu] are lost ", qwEventId);
		return;
	}
	if (pRes->error() == CBR::ERR_SUCCESS)
	{
		CRpc::ReplyDelayRpc(iter->second->dwRpcId, userData);
		DbReadEvent *pInfo = iter->second;
		delete pInfo;
		m_mapEvents.erase(iter);

	}else
	{
		LogInfo("Redis has no actor data,the actid=[%llu] ",iter->second->qwActorId);
		CActorQueryTask* task = new CActorQueryTask;
		task->m_qwStoreKey = iter->second->oArg.accountid();
		task->m_qwEventId = qwEventId;
		CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_qwStoreKey));
	}
	
}
