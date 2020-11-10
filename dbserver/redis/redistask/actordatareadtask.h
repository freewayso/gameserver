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
	void SetData(const CBR::ActorDataArg &oData);
	virtual void OnReply();
	void SetRpcID(const UINT32 nRpcID);
	void SetEventId(const UINT64 dwEventId);
private:
	CBR::ActorBase m_actorData;
	UINT32	m_dwRpcDelayed;
	CBR::ActorDataArg m_oArg;
	CBR::ActorDataRes m_oData;
	UINT64 m_dwEventId;
};

#endif
