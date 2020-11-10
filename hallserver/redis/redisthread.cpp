#include "pch.h"
#include "redisthread.h"
#include "timeutil.h"
#include "LogInit.h"
#include "util.h"
#include "timeutil.h"


static const UINT32	RedisTask_Pipe_Len = 10*1024 * 1024 * sizeof(CRedisTask*);


CRedisThread::CRedisThread()
{
	m_UseTotalTime = 0;
	m_ProcessedTask = 0;
	m_TaskInQueue = 0;
	m_MaxTaskInQueue = 0;
}

CRedisThread::~CRedisThread()
{
}

bool CRedisThread::Init(const RedisInfo& redisInfo)
{
	if(!m_redisConn.Connect(redisInfo.m_ip.c_str(), convert<UINT32>(redisInfo.m_port), redisInfo.m_password.c_str()))
		return false;
	m_oReqPipe.Init(RedisTask_Pipe_Len);
	m_oReplyPipe.Init(RedisTask_Pipe_Len);
	return true;
}

void CRedisThread::Uninit()
{

}

enum RedisThreadState
{
	TH_IDLE,
	TH_WORKING,
};

void CRedisThread::Run()
{
	while (true)
	{
		CRedisTask* poTask = NULL;
		if(PopTask(&poTask))
		{
			UINT64 qwBegin = TimeUtil::GetMilliSecond();
			poTask->SetRedisBeginTime(qwBegin);
			poTask->Execute(&m_redisConn);
			UINT64 qwEnd = TimeUtil::GetMilliSecond();
			poTask->SetRedisEndTime(qwEnd);

			PushReply(poTask);
		}
		else
		{
			if (m_isCancel == true)
			{
				break;
			}

			TimeUtil::Sleep(10);
		}
	}
	LogDebug("thread [%x] stopped!", this);
}

void CRedisThread::ProcessReply()
{
	CRedisTask* poTask = NULL;
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	while(PopReply(&poTask))
	{
		poTask->SetEndTime(qwEndTime);
		poTask->OnReply();

		if(poTask->GetTimeUsed() > 20 || poTask->GetRedisTimeUsed() > 10)
		{
			LogWarn("task %s finished, redis use %d(ms), total use %d(ms)", poTask->GetName().c_str(), poTask->GetRedisTimeUsed(), poTask->GetTimeUsed());
		}
		else
		{
			LogWarn("task %s finished, redis use %d(ms), total use %d(ms)", poTask->GetName().c_str(), poTask->GetRedisTimeUsed(), poTask->GetTimeUsed());
		}

		++m_ProcessedTask;
		delete poTask;
	}
}

void CRedisThread::PushTask(CRedisTask* poTask)
{
	while(!m_oReqPipe.Write((char*)(&poTask), sizeof(poTask)))
	{
		TimeUtil::Sleep(1);
	}

	++m_TaskInQueue;
	if(m_TaskInQueue > m_MaxTaskInQueue)
	{
		m_MaxTaskInQueue = m_TaskInQueue;
	}
}

bool CRedisThread::PopTask(CRedisTask** ppoTask)
{
	if(ppoTask == NULL)
	{
		return false;
	}

	if(!m_oReqPipe.Read((char*)ppoTask, sizeof(CRedisTask*)))
	{
		return false;
	}

	--m_TaskInQueue;
	return true;
}

void CRedisThread::PushReply(CRedisTask* poTask)
{
	while(!m_oReplyPipe.Write((char*)(&poTask), sizeof(poTask)))
	{
		TimeUtil::Sleep(1);
	}
}

bool CRedisThread::PopReply(CRedisTask** ppoTask)
{
	if(ppoTask == NULL)
	{
		return false;
	}

	if(!m_oReplyPipe.Read((char*)ppoTask, sizeof(CRedisTask*)))
	{
		return false;
	}

	return true;
}

void CRedisThread::ShowThreadInfo()
{
	UINT32 avgTime = 0;
	if (m_ProcessedTask > 0)
	{
		avgTime = m_UseTotalTime/m_ProcessedTask;
	}

	LogWarn("thread[%x] task in queue: curr %u, processed %u, max %u, process agv time: %u", 
		this, m_TaskInQueue, m_ProcessedTask, m_MaxTaskInQueue, avgTime);
}
