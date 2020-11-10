#include "pch.h"
#include "mysqlmgr.h"
#include "util.h"
#include "util/dbutil.h"
#include "config.h"
#include "timeutil.h"




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
	const DBInfo& dbInfo = DBConfig::Instance()->GetDBInfo();
	UINT32 dwThreadCount = dbInfo.m_threads;
	if(dwThreadCount == 0 || dwThreadCount > 20)
	{
		LogError("thread count [%u] is invalid", dwThreadCount);
		return false;
	}
	if(DBConfig::Instance()->AutoCreateDB())
	{
		IMysqlConn* poMysqlConn = CreateMysqlConn();
		if(poMysqlConn == NULL)
		{
			return false;
		}

		if(!poMysqlConn->Connect(dbInfo.m_ip.c_str(), convert<UINT32>(dbInfo.m_port), dbInfo.m_user.c_str(), dbInfo.m_pswd.c_str(), "mysql"))
		{
			LogError("Can't connect to database %s:%s:mysql, ErrDesc: %s", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), poMysqlConn->GetError());
			return false;
		}

		std::stringstream ss;
		ss << "CREATE DATABASE IF NOT EXISTS " << dbInfo.m_database << " " << "DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_general_ci;";
		if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
		{
			LogError("Create database %s failed", dbInfo.m_database.c_str());
			return false;
		}

		poMysqlConn->Close();
		if(!poMysqlConn->Connect(dbInfo.m_ip.c_str(), convert<UINT32>(dbInfo.m_port), dbInfo.m_user.c_str(), dbInfo.m_pswd.c_str(), dbInfo.m_database.c_str()))
		{
			LogError("Can't connect to database %s:%s:%s, ErrDesc: %s", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str(), poMysqlConn->GetError());
			return false;
		}

		const char* pszSqlScript = "dbinit.sql";
		if(!RunSqlScript(pszSqlScript, poMysqlConn))
		{
			LogError("Run script: %s failed, %s:%u", pszSqlScript, poMysqlConn->GetError(), poMysqlConn->GetErrno());
			return false;
		}

		poMysqlConn->Close();
		poMysqlConn->Release();
		LogInfo("Run script %s succ", pszSqlScript);
	}

	//m_oThreadList.reserve(dwThreadCount);
	for(UINT32 i = 0; i < dwThreadCount; ++i)
	{
		CMysqlThread* poThread = new CMysqlThread();
		if(!poThread->Init(dbInfo))
		{
			poThread->Uninit();
			delete poThread;
			return false;
		}
		poThread->GetMysqlConn()->SetMysqlLostCallBack(DBConfig::Instance()->GetReconTryInterval(), DBConfig::Instance()->GetReconMaxTryTimes(), CMysqlMgr::MysqlEventCallBack);
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
						//PtcD2N_MysqlConnectLost oPtc;
						//ControlLink::Instance()->SendTo(oPtc);
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

UINT32 GetRoleThreadIndex(UINT64 qwActorId)
{
	UINT32 dwTotal = CMysqlMgr::Instance()->GetThreadNum();

	return qwActorId % (dwTotal-1);
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
