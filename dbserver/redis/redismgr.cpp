#include "pch.h"
#include <string>
#include <sstream>
#include "redismgr.h"
#include "bytestream.h"
#include "config.h"
#include "util.h"
#include "util/dbutil.h"
#include "timeutil.h"
#include "util/XRandom.h"
#include "redis/redisthread.h"
#include "redis/hiredis.h"


INSTANCE_SINGLETON(CRedisMgr)


CRedisMgr::CRedisMgr()
:m_poRedisConn(NULL)
{ 
}

CRedisMgr::~CRedisMgr()
{
}

bool CRedisMgr::Init()
{
	const RedisInfo& redisInfo = DBConfig::Instance()->GetRedisInfo();
	int dwThreadCount = redisInfo.m_threads;
	m_oThreadList.reserve(dwThreadCount);
	for(UINT32 i = 0; i < dwThreadCount; ++i)
	{
		CRedisThread* poThread = new CRedisThread();
		if(!poThread->Init(redisInfo))
		{
			poThread->Uninit();
			delete poThread;
			return false;
		}

		poThread->Start();
		m_oThreadList.push_back(poThread);
	}

	return true;
}

void CRedisMgr::Uninit()
{
	if(m_poRedisConn != NULL)
	{
		redisFree(m_poRedisConn);
		m_poRedisConn = NULL;
	}
}

void CRedisMgr::Process()
{
	for(UINT32 i = 0; i < m_oThreadList.size(); ++i)
	{
		CRedisThread* poThread = m_oThreadList[i];
		poThread->ProcessReply();
	}
}

void CRedisMgr::PushTask(CRedisTask* poTask, UINT32 dwThrdIndex)
{
	int rand = 0;
	rand = dwThrdIndex;
	CRedisThread* poThread = GetThread(rand);
	if(poThread == NULL)
	{
		return;
	}
	UINT64 qwStartTime = TimeUtil::GetMilliSecond();
	poTask->SetBeginTime(qwStartTime);
	poThread->PushTask(poTask);
}

CRedisThread* CRedisMgr::GetThread(UINT32 dwIndex)
{
	if(dwIndex >= m_oThreadList.size())
	{
		return NULL;
	}
	return m_oThreadList[dwIndex];
}
