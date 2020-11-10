#ifndef _USER_REG_TASK_H_
#define _USER_REG_TASK_H_

#include "SQLTask.h"
#include "pb/project.pb.h"
class UserRegTask : public ISQLTask
{
public:
	UserRegTask(UINT32 rpcid);
	~UserRegTask();

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();
	void SetData(CBR::RegAccountArg &data);

private:
	UINT32	m_rpcid;
	CBR::RegAccountArg m_arg;
	CBR::RegAccountRes m_res;
	UINT64	m_qwBeginTime;
};

#endif
