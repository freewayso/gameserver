#include "pch.h"
#include "memorysyncdb.h"
#include "mysql/mysqlmgr.h"
#include "mysql/task/actorupdatetask.h"
#include "redis/redistask/actordatawritetask.h"
#include "timeutil.h"
#include "crpc.h"
#include "redis/redisthread.h"
#include "redis/redismgr.h"



INSTANCE_SINGLETON(MemorySyncDb)

#define UPDATE_TIME  2
#define UPDATE_COUNT 100

MemorySyncDb::MemorySyncDb()
{
	
}

MemorySyncDb::~MemorySyncDb()
{
	
}

bool MemorySyncDb::Init()
{
	m_dwTime = TimeUtil::GetTime() + UPDATE_TIME;
	return true;
}


void MemorySyncDb::Uninit()
{
}

void MemorySyncDb::Update()
{
	UINT32 dwTime = TimeUtil::GetTime();
	if (dwTime >=  m_dwTime || m_mapActors.size() >= UPDATE_COUNT)
	{
		dwTime =  dwTime + UPDATE_TIME;
	}
	for (auto iter = m_mapActors.begin(); iter != m_mapActors.end();iter++)
	{
		if (iter->second.state == DATA_DELAY_SUBMIT)
		{
			stActorData &sInfo = iter->second;
			sInfo.state = DATA_FINISH_SUBMIT;
			ActorUpdateTask *pTask = new ActorUpdateTask;
			pTask->m_oReq.CopyFrom(sInfo.data);
			CMysqlMgr::Instance()->PushTask(pTask);
		}
	}
}

void MemorySyncDb::OnLogin(const CBR::ActorBase &data)
{

}


void MemorySyncDb::OnLoginOut(UINT64 qwActorId)
{
	auto iter = m_mapActors.find(qwActorId);
	if (iter != m_mapActors.end())
	{
		m_mapActors.erase(iter);
	}else
	{
		LogError("login out cannot find qwActorid=[%llu]",qwActorId);
	}

}

void MemorySyncDb::AddData(const CBR::ActorBase &data,eCacheState state)
{
	stActorData sInfo;
	sInfo.data.CopyFrom(data);
	UINT32 dwTime = TimeUtil::GetTime();
	sInfo.dwTime = dwTime;
	sInfo.state = state;
	UINT64 qwActorId = data.app().actid();
	UINT32 dwVersion = data.verion();
	auto iter = m_mapActors.find(qwActorId);
	if (iter == m_mapActors.end())
	{
		m_mapActors[qwActorId] =  sInfo;
	}else
	{
		m_mapActors[qwActorId] =  sInfo;
		LogInfo("Memory already exists actorid=[%llu] version=[%u] time=[%u]",qwActorId,dwVersion,dwTime);
	}
}


bool MemorySyncDb::SyncData2Redis(const CBR::ActorBase &data, bool flag)
{
	CRedisActorDataWriteTask *pTask = new CRedisActorDataWriteTask;
	CBR::UpdateActorDataArg oArg;
	oArg.mutable_act()->CopyFrom(data);
	if (flag)
	{
		UINT32 dwVersion = data.verion() + 1;
		oArg.mutable_act()->set_verion(dwVersion);
		oArg.mutable_act()->set_time(TimeUtil::GetTime());
	}
	pTask->SetData(oArg);
	CRedisMgr::Instance()->PushTask(pTask);
	AddData(data);
	return true;
}


CBR::ActorBase * MemorySyncDb::GetActorBase(UINT64 qwActorId)
{
	auto iter = m_mapActors.find(qwActorId);
	if (iter != m_mapActors.end())
	{
		return &((iter->second).data);
	}
	return NULL;
}
