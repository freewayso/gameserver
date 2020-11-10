#ifndef __ACTOR_QUERY_TASK_H
#define __ACTOR_QUERY_TASK_H

#include "mysqltask.h"
#include "pb/project.pb.h"
#include <vector>
class CRedisActorReadTask;


//typedef void (* pfcFunc) (CBR::ErrorCode nErrCode, CBR::ActorBase &data);

class CActorQueryTask : public CMysqlTask
{
public:
	CActorQueryTask();
	virtual ~CActorQueryTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();
	void SetRedisActorTask(CRedisActorReadTask *pTask);

public:
	UINT64 m_qwStoreKey;
	UINT32 m_dwRpcDelayed;
	UINT64 m_qwEventId;
	CBR::ActorDataRes m_oRes;
};



#endif

