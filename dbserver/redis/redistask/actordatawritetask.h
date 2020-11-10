#ifndef _REDIS_ROLEDATA_WRITE_TASK__
#define _REDIS_ROLEDATA_WRITE_TASK__

#include "redistask.h"
#include "pb/project.pb.h"



class CRedisActorDataWriteTask : public CRedisTask
{
public:
	CRedisActorDataWriteTask(); 
	virtual ~CRedisActorDataWriteTask();
	virtual void Execute(RedisConn* poRedisConn); 
	void SetData(CBR::UpdateActorDataArg &oData);
	virtual void OnReply();
	void SetRpcID(const UINT32 nRpcID);
private:
	UINT32	m_dwRpcDelayed;
	CBR::UpdateActorDataArg m_oData;

};

#endif
