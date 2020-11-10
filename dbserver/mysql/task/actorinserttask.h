#ifndef _ACTOR_INSERT_TASK_H_
#define _ACTOR_INSERT_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class ActorInsertTask : public CMysqlTask
{
public:
	ActorInsertTask();	
	virtual ~ActorInsertTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_id;
	CBR::CreateActorReq  m_data;
	CBR::CreateActorRes m_info;
};

#endif
