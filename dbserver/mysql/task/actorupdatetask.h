#ifndef _CHICK_DINNER_QUERY_TASK_H__
#define _CHICK_DINNER_QUERY_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class ActorUpdateTask : public CMysqlTask
{
public:
	ActorUpdateTask();
	virtual ~ActorUpdateTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64 m_dwStoreKey;
	UINT32 m_dwRpcDelayed;
	CBR::ActorBase m_oReq;
};

#endif