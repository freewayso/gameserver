#ifndef __LOGIN_TASK_H
#define __LOGIN_TASK_H

#include "mysqltask.h"
#include "pb/project.pb.h"

#include <vector>
#include <string>

//采取的策略，查询线程，写入线程分开
class QueryUserTask : public CMysqlTask
{
public:
	QueryUserTask();
	virtual ~QueryUserTask();

	void GetActorByAccount(UINT64 qwAccountId,IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();
	void SetData(CBR::LoginDataArg &data);
public:
	std::string m_szStoreKey;
	UINT32 m_dwRpcDelayed;
	CBR::LoginDataRes m_res;
};



#endif