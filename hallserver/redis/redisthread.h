#ifndef _H_Redis_Thread_H__
#define _H_Redis_Thread_H__


#include "threads.h"
#include "mempipe.h"
#include "redistask.h"
#include "config.h"
#include "redishelper.h"


class CRedisThread : public Thread
{
public:
	CRedisThread();
	~CRedisThread();

	bool Init(const RedisInfo& redisInfo);
	void Uninit();

	virtual void Run();

	void ProcessReply();
	void PushTask(CRedisTask* poTask); 

	void ShowThreadInfo();
private:
	void PushReply(CRedisTask* poTask);
	bool PopTask(CRedisTask** ppoTask); 
	bool PopReply(CRedisTask** poTask);

private:
	UINT32		m_UseTotalTime;
	UINT32		m_ProcessedTask;
	UINT32		m_TaskInQueue;
	UINT32		m_MaxTaskInQueue;
	CMemPipe	m_oReqPipe;
	CMemPipe	m_oReplyPipe;
	RedisConn	m_redisConn;
};

#endif

