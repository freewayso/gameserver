#ifndef __INSERT_USER_TASK_H
#define __INSERT_USER_TASK_H

#include "mysqltask.h"
#include "pb/project.pb.h"

class InsertUserTask : public CMysqlTask
{
		//加入到插入队列，提高总体的并发
	public:
	InsertUserTask();	
	virtual ~InsertUserTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_id;
	CBR::UserSaveData  m_data;
	
};




#endif
