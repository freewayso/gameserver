#ifndef _USER_AUTH_TASK_H_
#define _USER_AUTH_TASK_H_

#include "SQLTask.h"
#include "pb/project.pb.h"
class UserAuthTask : public ISQLTask
{
public:
	UserAuthTask(UINT32 rpcid);
	~UserAuthTask();

public:
	virtual int  Execute(MYSQL *mysql);

	virtual void FinishTask();
	void SetData(CBR::LoginDataArg &data);
	static UINT64 SessionCounter;
private:
	UINT32	m_rpcid;
	CBR::LoginDataArg m_arg;
	CBR::LoginDataRes m_res;
	UINT64	m_qwBeginTime;

};

#endif
