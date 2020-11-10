#include "pch.h"
#include <time.h>
#include "sessionmanager.h"
#include "clientlink.h"
#include "timeutil.h"
#include "config.h"
#include "netproc.h"
#include "LogInit.h"
#include "define.h"
#include "cmdline.h"
#include "protocol.h"
#include "login/ptct2h_loginoutntf.h"
#include "network/halllink.h"
#include "game/ptct2c_heartbeat.h"


#define SESSION_KEEPALIVE_TIME 300
#define SESSION_KEEPALIVE_TRYTIMES 3


INSTANCE_SINGLETON(SessionManager)



SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{	
}



bool SessionManager::Init()
{
	m_dwTime = TimeUtil::GetTime();
	return true;
}

void SessionManager::Uninit()
{
	m_mapUnAuthToken.clear();
	m_mapSessionsInfo.cbegin();
}

void SessionManager::Update()
{
	ClearExpireTimeToken();
	ClearExpireTimeSession();
	ClearZoobie();
	//处理下心跳
}


void SessionManager::ClearZoobie()
{
	UINT32 dwTime = TimeUtil::GetTime();
	if (dwTime < m_dwTime)
	{
		return;
	}
	m_dwTime = m_dwTime + SESSION_KEEPALIVE_TIME;
	for (auto iter = m_mapSessionsInfo.begin();iter!=m_mapSessionsInfo.end();iter++)
	{
		if (iter->second.m_state != SESSION_INVALID)
		{
			if (iter->second.m_keepAliveTryTimes >= SESSION_KEEPALIVE_TRYTIMES)
			{
				PtcT2H_LoginOutNtf ntf;
				ntf.m_Data.set_session(iter->second.m_sessionID);
				HallLink::Instance()->SendTo(ntf);
				m_mapSessionsInfo.erase(iter++);
			}else
			{
				iter++;
				SessionInfo &info = iter->second;
				++info.m_keepAliveTryTimes;
				PtcT2C_Heartbeat ptc;
				ptc.m_Data.set_time(dwTime);
				CClientLink::Instance()->Send(iter->first, ptc);
			
			}
		}
	}
}



void SessionManager::ClearExpireTimeToken()
{
	//淘汰未验证的fd
	UINT32 dwTime = TimeUtil::GetTime();
	for (auto iter = m_mapUnAuthToken.begin();iter!=m_mapUnAuthToken.end();)
	{
		if (dwTime  >=  iter->second.m_expireTime)
		{
			LogInfo("clear expire time token=[%llu] account=[%llu] createtime=[%u] ",iter->second.m_tokenId,iter->second.m_accountId,iter->second.m_creatime);
			m_mapUnAuthToken.erase(iter++);
		}else
		{
			iter++;
		}
	}
}

void SessionManager::ClearExpireTimeSession()
{
	//如果超时了并且是非法的
	UINT32 dwTime = TimeUtil::GetTime();
	for (auto iter = m_mapSessionsInfo.begin();iter!=m_mapSessionsInfo.end();)
	{
		if ( dwTime >= iter->second.m_expireTime   && iter->second.m_state == SESSION_INVALID)
		{
			LogInfo("clear expire time fd %u",iter->second.m_connID);
			CClientLink::Instance()->Close(iter->second.m_connID);
			m_mapSessionsInfo.erase(iter++);
		
		}else
		{
			iter++;
		}
	}
}


void SessionManager::OnClose(UINT32 dwConnID)
{
	auto iter = m_mapSessionsInfo.find(dwConnID);
	if (iter !=m_mapSessionsInfo.end())
	{
		PtcT2H_LoginOutNtf ntf;
		ntf.m_Data.set_session(iter->second.m_sessionID);
		HallLink::Instance()->SendTo(ntf);
		m_mapSessionsInfo.erase(iter);
	}
}


UINT32 SessionManager::GetSessionConnID(SessionType sessionID)
{
	for (auto iter = m_mapSessionsInfo.begin();iter!=m_mapSessionsInfo.end();iter++)
	{
		if (iter->second.m_sessionID==sessionID)
		{
			return iter->first;
		}
	}
	return 0;
}

SessionInfo * SessionManager::FindSession(SessionType sessionID)
{
	for (auto iter = m_mapSessionsInfo.begin();iter!=m_mapSessionsInfo.end();iter++)
	{
		if (iter->second.m_sessionID==sessionID)
		{
			return &iter->second;
		}
	}
	return NULL;
}

SessionInfo * SessionManager::FindByConnID(UINT32 dwConnID)
{
	auto iter = m_mapSessionsInfo.find(dwConnID);
	if (iter !=m_mapSessionsInfo.end())
	{
		return &iter->second;
	}
	return NULL;

}


void SessionManager::SetSessionState(UINT32 dwConnID,SessionState dwSessionState)
{
	auto it = m_mapSessionsInfo.find(dwConnID);
	if (it == m_mapSessionsInfo.end())
	{
		LogError("set session error reason is fd[%u] cannot find",dwConnID);
		return ;
	}
	SessionInfo &info = it->second;
	info.m_state = SESSION_INGAME;
}

void SessionManager::OnNewConnection(UINT32 dwConnID)
{
	SessionInfo sInfo;
	sInfo.m_createTime = TimeUtil::GetTime();
	sInfo.m_expireTime = TimeUtil::GetTime() + TOKEN_EXPIRETGIME;
	sInfo.m_state      = SESSION_INVALID;
	sInfo.m_connID	   = dwConnID;
	m_mapSessionsInfo[dwConnID] = sInfo;

}


void SessionManager::RemoveSession(SessionType sessionID)
{

	for (auto iter = m_mapSessionsInfo.begin();iter!=m_mapSessionsInfo.end();iter++)
	{
		if (iter->second.m_sessionID==sessionID)
		{
			m_mapSessionsInfo.erase(iter);
			break;
		}
	}
}




void SessionManager::AddToken(CBR::LoginSession2Gate &data)
{
	auto iter = m_mapUnAuthToken.find(data.session());
	if (iter == m_mapUnAuthToken.end())
	{
		LogInfo("login process is  sucessful after addToken accountid[%llu] token[%llu] name[%s] time[%u] ",data.accountid(),data.session(),data.accountname().c_str(),TimeUtil::GetTime());
		stToken token(data.accountid(),data.accountname(),TimeUtil::GetTime(),TimeUtil::GetTime()+TOKEN_EXPIRETGIME,data.session());
		m_mapUnAuthToken.insert(std::pair<UINT64, stToken>(data.session(),token));
	}else
	{
		LogError("login process is  erorr, because the token is repeat, Token accountid[%llu] token[%llu] name[%s] time[%u] ",data.accountid(),data.session(),data.accountname().c_str(),TimeUtil::GetTime());
	}

}


CBR::ErrorCode SessionManager::AuthToken(UINT32 dwConnID,UINT64 qwToken)
{
	auto it = m_mapSessionsInfo.find(dwConnID);
	if (it == m_mapSessionsInfo.end())
	{
		LogError("login session error reason is fd[%u] cannot find",dwConnID);
		return CBR::ERR_SESSION_INVALID;
	}

	auto iter = m_mapUnAuthToken.find(qwToken);
	if (iter == m_mapUnAuthToken.end())
	{
		CClientLink::Instance()->Close(dwConnID);
		LogError("login token error reason is token[%llu] cannot find",qwToken);
		return CBR::ERR_TOKEN_INVALID;
	}
	UINT64 accountid = iter->second.m_accountId;
	m_mapUnAuthToken.erase(iter);

	SessionInfo &sInfo = it->second;
	if (sInfo.m_state == SESSION_LOGIN)
	{
		LogError("client  is repeat login [%llu]",qwToken);
		return CBR::ERR_STATE_INVALID;
	}
	sInfo.m_sessionID = qwToken;
	sInfo.m_state = SESSION_LOGIN;
	sInfo.m_accountId = accountid;
	sInfo.m_connID = dwConnID;
	UINT32 dwIP;
	UINT16 wPort;
	CNetProcessor::Instance()->GetRemoteAddr(dwConnID, dwIP, wPort);
	char IPBuf[64] = {0};
	unsigned char *p = (unsigned char *)&dwIP;
	sprintf(IPBuf, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
	sInfo.m_strIP = IPBuf;
	LogInfo("a client come from [%s] with session => [%llu : %d]", IPBuf, sInfo.m_sessionID, dwConnID);
	return CBR::ERR_SUCCESS;

}


SessionType SessionManager::SessionConvert(UINT32 dwConnID, UINT32 dwProtoType)
{
	auto it = m_mapSessionsInfo.find(dwConnID);
	if (it == m_mapSessionsInfo.end())
	{
		return INVALID_SESSION_ID;
	}
	SessionInfo &info= it->second;
	info.m_lastRecvTime = TimeUtil::GetTime();
	info.m_keepAliveTryTimes = 0;
	return info.m_sessionID;
}

