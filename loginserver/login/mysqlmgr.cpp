#include "pch.h"
#include "mysqlmgr.h"
#include "util.h"
#include "util/dbutil.h"
#include "config.h"
#include "timeutil.h"
#include "util/XRandom.h"




INSTANCE_SINGLETON(CMysqlMgr)


CMysqlMgr::CMysqlMgr()
:m_bMysqlOK(false)
,m_bKickNtfSended(false)
,m_bMSSaveDBEnd(true)
{ 
}

CMysqlMgr::~CMysqlMgr()
{
}


bool CMysqlMgr::Init()
{
	const DBInfo& dbInfo = LoginConfig::Instance()->GetDBInfo();
	UINT32 dwThreadCount = dbInfo.m_threads;
	if(dwThreadCount == 0)
	{
		LogError("thread count [%u] is invalid", dwThreadCount);
		return false;
	}
	for(UINT32 i = 0; i < dwThreadCount; ++i)
	{
		CMysqlThread* poThread = new CMysqlThread();
		if(!poThread->Init(dbInfo))
		{
			poThread->Uninit();
			delete poThread;
			return false;
		}
		poThread->GetMysqlConn()->SetMysqlLostCallBack(3, 3, CMysqlMgr::MysqlEventCallBack);
		poThread->Start();
		m_oThreadList.push_back(poThread);
	}

	return true;
}

void CMysqlMgr::Uninit()
{
	for(UINT32 i = 0; i < m_oThreadList.size(); ++i)
	{
		CMysqlThread* poThread = m_oThreadList[i];
		poThread->Cancel();
		poThread->Join();
		poThread->Uninit();
		delete poThread;
	}

	m_oOnlineThread.Cancel();
	m_oOnlineThread.Join();
	m_oOnlineThread.Uninit();

	m_oThreadList.clear();
}

void CMysqlMgr::Process()
{
	for(UINT32 i = 0; i < m_oThreadList.size(); ++i)
	{
		CMysqlThread* poThread = m_oThreadList[i];
		poThread->ProcessReply();
	}
	m_oOnlineThread.ProcessReply();

	UpdateMysqlStatus();
}

void CMysqlMgr::PushTask(CMysqlTask* poTask, UINT32 dwThrdIndex)
{
	CMysqlThread* poThread = GetThread(dwThrdIndex);
	if(poThread == NULL)
	{
		LogError("cannot find thread %u ",dwThrdIndex);
		return;
	}

	UINT64 qwStartTime = TimeUtil::GetMilliSecond();
	poTask->SetBeginTime(qwStartTime);
	poThread->PushTask(poTask);
}

CMysqlThread* CMysqlMgr::GetThread(UINT32 dwIndex)
{
	if(dwIndex >= m_oThreadList.size())
	{
		LogError("cannot find thread %u ",dwIndex);
		return NULL;
	}
	return m_oThreadList[dwIndex];
}

void CMysqlMgr::PushOnlineNumTask(CMysqlTask* poTask)
{
	UINT64 qwStartTime = TimeUtil::GetMilliSecond();
	poTask->SetBeginTime(qwStartTime);
	m_oOnlineThread.PushTask(poTask);
}




void CMysqlMgr::UpdateMysqlStatus()
{
	static UINT64 sqwLastTime = 0;
	UINT64 qwNowTime = TimeUtil::GetMilliSecond();

	if(qwNowTime < sqwLastTime + 1000) return;

	sqwLastTime = qwNowTime;
	MutexGuard guard(&m_oMysqlEvtMux);
	for(std::vector<INT32>::size_type i = 0; i < m_oMysqlEvtList.size(); ++i)
	{
		std::pair<EMysqlConnEvent, std::string>& event = m_oMysqlEvtList[i];
		EMysqlConnEvent nEvent = event.first;
		std::string& sql = event.second;

		if(m_bMysqlOK)
		{
			switch(nEvent)
			{
			case MYSQL_CONN_LOST_EVENT:
				{
					m_bMysqlOK = false;
					LogError("!!!!!!! MysqlError: Mysql Connection Lost, sql[%s]", sql.c_str());
				}
				break;

			default:
				break;
			}
		}
		else
		{
			switch(nEvent)
			{
			case MYSQL_RECONNECT_TRY_MAX_EVENT:
				{
					if(!m_bKickNtfSended)
					{
						m_bKickNtfSended = true;
						LogFatal("!!!!!! MysqlError: Reconnect to mysql with max try, send kickout role protocol to control server, sql[%s]", sql.c_str());
					}
				}
				break;

			case MYSQL_RECONNECT_SUCCESS_EVENT:
				{
					m_bMysqlOK = true;
					m_bKickNtfSended = false;
					LogInfo("!!!!!! MysqlInfo: Reconnect to mysql successful, sql[%s]", sql.c_str());
				}
				break;

			default:
				break;
			}
		}
	}
	m_oMysqlEvtList.clear();
}

void CMysqlMgr::MysqlEventCallBack(EMysqlConnEvent nEvent, const std::string& sql)
{
	MutexGuard guard(&CMysqlMgr::Instance()->m_oMysqlEvtMux);
	CMysqlMgr::Instance()->m_oMysqlEvtList.push_back(std::make_pair(nEvent, sql));
}

UINT32 GetUserQueryThreadIndex()
{
	UINT32 dwTotal = CMysqlMgr::Instance()->GetThreadNum();
	return XRandom::randInt(0,dwTotal);
}

UINT32 GetUserInsertThreadIndex()
{
	UINT32 dwStart = CMysqlMgr::Instance()->GetThreadNum()/2;
	UINT32 dwEnd = CMysqlMgr::Instance()->GetThreadNum();
	return XRandom::randInt(dwStart,dwEnd);
}





UINT32 GetAccountThreadIndex()
{
	UINT32 dwTotal = CMysqlMgr::Instance()->GetThreadNum();
	if(dwTotal < 2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
