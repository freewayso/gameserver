#ifndef _L_MySql_Mgr_H__
#define _L_MySql_Mgr_H__

#include "mutex.h"
#include "mysqltask.h"
#include "mysqlthread.h"


class CMysqlMgr
{
	CMysqlMgr();
	~CMysqlMgr();
	DECLARE_SINGLETON(CMysqlMgr)

public:
	bool Init();
	void Uninit();


	void Process();

	void PushTask(CMysqlTask* poTask, UINT32 dwThrdIndex = 0); 
	CMysqlThread* GetThread(UINT32 dwIndex);

	void PushOnlineNumTask(CMysqlTask* poTask); 
	UINT32 GetThreadNum() const 
	{ 
		int theads = m_oThreadList.size();
		return m_oThreadList.size(); 
	}

	void UpdateMysqlStatus();
	static void MysqlEventCallBack(EMysqlConnEvent nEvent, const std::string& sql);

	void SetMSSaveDBEnd(bool isEnd);
	bool GetMSSaveDBEnd() { return m_bMSSaveDBEnd; }

private:
	typedef std::vector<CMysqlThread*> CMysqlThreadList;
	CMysqlThreadList	m_oThreadList;
	CMysqlThread		m_oOnlineThread;

	bool				m_bMysqlOK;
	bool				m_bKickNtfSended;
	Mutex				m_oMysqlEvtMux;
	std::vector<std::pair<EMysqlConnEvent, std::string>>	m_oMysqlEvtList;

	bool				m_bMSSaveDBEnd;
};


UINT32 GetAccountThreadIndex();
UINT32 GetUserQueryThreadIndex(/*UINT64 qwUserID*/);
UINT32 GetUserInsertThreadIndex(/*UINT64 qwUserID*/);

#endif