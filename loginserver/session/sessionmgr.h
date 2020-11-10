#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include "table/ActorBaseConfig.h"
#include "mutex.h"

enum SessionSate
{
	E_INVALID_STATE				= 0,//未经过验证
	E_LOGIN_STATE				= 1,//验证通过
	E_LOGIN_FIN_STATE    = 2,//创角完成
};

struct SessionInfo
{
	SessionInfo()
	{
		m_connID		= 0;
		m_lastRecvTime	= 0;
		m_createTime	= 0;
		m_strIP			= "";
		state			= E_INVALID_STATE;
	}
	UINT32 m_connID;
	UINT32  m_lastRecvTime;
	time_t m_createTime;
	std::string	m_strIP;
	SessionSate state;
};

#define EXPIRATION_TIME 120


class SessionManager : public ITimer
{
	SessionManager();
	virtual ~SessionManager();
	DECLARE_SINGLETON(SessionManager)

public:
	bool Init();
	void Uninit();
	UINT64 MakeSessionID();
	std::string getIp(UINT32 dwConnID);
	SessionSate GetSessionState(UINT32 dwConnID);
	void SetSessionState(UINT32 dwConnID,SessionSate state);
	void OnNewConnection(UINT32 dwConnID);
	void OnCloseSession(UINT32 dwConnID);
	void AllocClientIpPort(UINT32 dwConnID,UINT64 ddwAccountId);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	UINT64 CreateUUID();
private:
	UINT32 m_dwSessionCounnter;
	std::unordered_map<UINT64, SessionInfo > m_mapSessionInfo;
	HTIMER m_handler;
	ActorBaseConfig m_config;
	Mutex m_mutex;
	UINT32 m_uidbegin;

};





#endif

