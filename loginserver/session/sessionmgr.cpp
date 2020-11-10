#include "pch.h"
#include "sessionmgr.h"
#include "netproc.h"
#include "LogInit.h"
#include "timeutil.h"
#include "config.h"
#include "network/clientlink.h"
#include "network/gatelink.h"



INSTANCE_SINGLETON(SessionManager)

SessionManager::SessionManager()
{

}

SessionManager::~SessionManager()
{

}


bool SessionManager::Init()
{
	//if(!m_config.LoadFile("table/ActorBaseConfig.csv"))
	//{
	//	SSWarn<<"load file table/ActorBaseConfig.txt failed"<<END;
	//	return false;
	//}
	m_uidbegin  = 0;
	m_dwSessionCounnter= 0;
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	return true;
}


void SessionManager::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

UINT64 SessionManager::MakeSessionID()
{
	time_t ServerTime = TimeUtil::GetTime();
	UINT64 serverID = LoginConfig::Instance()->GetServerID();
	serverID = ((serverID << 32) | ServerTime);
	serverID = ((serverID << 16) | ((++m_dwSessionCounnter) & 0xFFFF));
	return serverID;
}



UINT64 SessionManager::CreateUUID()
{
	MutexGuard guard(&m_mutex);
	m_uidbegin++;
	return ((UINT64)TimeUtil::GetTime() << 32 | m_uidbegin);
}



std::string SessionManager::getIp(UINT32 dwConnID)
{
	for (auto iter = m_mapSessionInfo.begin(); iter != m_mapSessionInfo.end(); iter++  )
	{
		if (iter->second.m_connID == dwConnID)
		{
			return iter->second.m_strIP;
		}
	}
	return "";

}

SessionSate SessionManager::GetSessionState(UINT32 dwConnID)
{
	for (auto iter = m_mapSessionInfo.begin(); iter != m_mapSessionInfo.end(); iter++  )
	{
		if (iter->second.m_connID == dwConnID)
		{
			return iter->second.state;
		}
	}
	return E_INVALID_STATE;
}



void SessionManager::SetSessionState(UINT32 dwConnID,SessionSate state)
{
	for (auto iter = m_mapSessionInfo.begin(); iter != m_mapSessionInfo.end(); iter++  )
	{
		if (iter->second.m_connID == dwConnID)
		{
			SessionInfo &info = iter->second;
			info.state = state;
			break;
		}
	}
}



void SessionManager::OnNewConnection(UINT32 dwConnID)
{	

	UINT64 newSessionID = MakeSessionID();
	UINT32 dwIP = 0;
	UINT16 wPort;
	CNetProcessor::Instance()->GetRemoteAddr(dwConnID, dwIP, wPort);
	char ip[64] = {0};
	unsigned char *p = (unsigned char *)&dwIP;
	sprintf(ip, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
	LogInfo("a client come from [%s] with session => [%llu : %d]", ip, newSessionID, dwConnID);
	SessionInfo sInfo;
	sInfo.m_connID		 = dwConnID;
	sInfo.m_createTime	 = TimeUtil::GetTime();
	sInfo.m_lastRecvTime = 0;
	sInfo.m_strIP		 = ip;
	m_mapSessionInfo[newSessionID] = sInfo;

}

void SessionManager::OnCloseSession(UINT32 dwConnID)
{
	for (auto iter = m_mapSessionInfo.begin(); iter != m_mapSessionInfo.end(); iter++  )
	{
		if (iter->second.m_connID == dwConnID)
		{

			LogInfo("link close %u",dwConnID);
			m_mapSessionInfo.erase(iter);
			break;
		}
	}
}


void SessionManager::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	//120秒没发送登录、注册相关的信息，自动断开
	UINT32 nTime = TimeUtil::GetTime();
	for (auto iter = m_mapSessionInfo.begin(); iter != m_mapSessionInfo.end();  )
	{
		if ((nTime - iter->second.m_createTime >= EXPIRATION_TIME ))
		{
			LogInfo("login session limit 5min so close socket fd=%u ",iter->second.m_connID);
			CClientLink::Instance()->Close(iter->second.m_connID);
			iter = m_mapSessionInfo.erase(iter);

		}else
		{
			iter++;
		}
	}

}
