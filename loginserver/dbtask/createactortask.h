#ifndef _ACTOR_CREATE_TASK_H_
#define _ACTOR_CREATE_TASK_H_
#include "SQLTask.h"
#include "pb/project.pb.h"
class CreateActorTask : public ISQLTask
{
public:
	CreateActorTask(UINT32 rpcid);
	~CreateActorTask();

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();
	void SetData(CBR::CreateActorReq &data);

private:
	UINT32	m_rpcid;
	CBR::CreateActorReq m_arg;
	UINT64	m_qwBeginTime;
};

#endif

