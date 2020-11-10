#ifndef __VERIFYMGR_H__
#define __VERIFYMGR_H__

#include "SQLThread.h"
#include "pb/project.pb.h"

#define SQLTHREAD_COUNT 4

class VerifyMgr
{
	VerifyMgr();
	~VerifyMgr();
	DECLARE_SINGLETON(VerifyMgr)

public:

	bool Init();
	void Uninit();

	void Update();

	void AddDBTask(ISQLTask *pTask);
	UINT32 GetDbTaskNum() { return m_dbTaskNum; }
	void ClearDbTaskNum() { m_dbTaskNum = 0; }

private:
	int m_sqlRandIndex;
	SQLThread m_dbThread[SQLTHREAD_COUNT];
	time_t m_NextCheckTime;
	UINT32 m_dbTaskNum;


};

#endif 
