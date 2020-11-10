#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include "multiindex_map.h"
#include "cmdline.h"
#include "commondef.h"
#include "pb/project.pb.h"


enum SessionState
{
	SESSION_INVALID,
	SESSION_LOGIN,				// 登录角色成功
	SESSION_INGAME,				//进入游戏
};



inline const char *GetSessionStateString(SessionState state)
{
	switch(state) {
	case SESSION_INVALID: return "[invalid]";
	case SESSION_LOGIN: return "[login]";
	default:return "[Unknow]";
	}
	return "[unknow]";
}


//token确保唯一性
struct stToken
{
	UINT64 m_accountId;
	std::string  m_accountName;
	UINT32 m_creatime;
	UINT32 m_expireTime;
	UINT64 m_tokenId;
	stToken(UINT64 accountId,std::string  accountName,UINT32 creatime,UINT32 expireTime,UINT64 tokenId)
	{
		m_accountId		= accountId;
		m_accountName	= accountName;
		m_creatime		= creatime;
		m_expireTime	= expireTime;
		m_tokenId		= tokenId;
	}
};

#define TOKEN_EXPIRETGIME  300



struct SessionInfo
{
	SessionInfo()
	{
		m_connID = 0;
		m_sessionID = 0;
		m_lastRecvTime = 0;
		m_keepAliveTryTimes = 0;
		m_protoCount = 0;
		m_createTime = 0;
		m_expireTime = 0;
		m_accountId = 0;
	}
	UINT32 m_connID;
	UINT32  m_lastRecvTime;
	UINT32 m_keepAliveTryTimes;
	UINT32 m_protoCount;
	time_t m_createTime;
	time_t m_expireTime;
	SessionState m_state;
	SessionType m_sessionID;
	std::string	m_strIP;//记录ip地址
	UINT64 m_accountId;

};


class CProtocol;


class SessionManager
{
	SessionManager();
	virtual ~SessionManager();
	DECLARE_SINGLETON(SessionManager)

public:
	bool Init();
	void Uninit();
	void ClearExpireTimeToken();
	void ClearExpireTimeSession();
	void Update();
	void ClearZoobie();
	SessionType GetSession(UINT32 dwConnID);
	SessionType SessionConvert(UINT32 dwConnID, UINT32 dwProtoType);
	void OnNewConnection(UINT32 dwConnID);
	void RemoveSession(SessionType sessionID);
	void AddToken(CBR::LoginSession2Gate &data);
	CBR::ErrorCode AuthToken(UINT32 dwConnID,const UINT64 qwToken);
	void OnClose(UINT32 dwConnID);
	UINT32 GetSessionConnID(SessionType sessionID);
	SessionInfo *FindSession(SessionType sessionID);
	SessionInfo *FindByConnID(UINT32 dwConnID);
	UINT32 GetSessionNum() { return m_mapSessionsInfo.size(); }
	void SetSessionState(UINT32 dwConnID,SessionState dwSessionState);
private:
	std::map<UINT32,SessionInfo> m_mapSessionsInfo;
	std::unordered_map<UINT64, stToken> m_mapUnAuthToken;
	UINT32 m_dwTime;
};

#endif
