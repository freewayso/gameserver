#ifndef __REDIS_ACTOR_READ_TASK__
#define __REDIS_ACTOR_READ_TASK__

#include "redistask.h"
#include "pb/project.pb.h"



class CRedisActorReadTask : public CRedisTask
{
public:
	CRedisActorReadTask(); 
	virtual ~CRedisActorReadTask();
	virtual void Execute(RedisConn* poRedisConn); 
	virtual void OnReply();
	void SetRpcID(const UINT32 nRpcID);
	void SetQueryParam(std::vector<UINT64> &vec);
private:
	std::vector<CBR::ActorBase> m_vecActorData;// 查出来结果
	UINT32	m_dwRpcDelayed;
	std::vector<UINT64> m_vecActorId;
};

#endif
