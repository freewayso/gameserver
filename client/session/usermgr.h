#ifndef __TEST_USER_MGR_H__
#define __TEST_USER_MGR_H__


#include <map>
#include <string>
#include "define.h"
#include "singleton.h"


struct AccountSession
{
	UINT64 qwToken;
	UINT64 qwAccountId;
	UINT32 dwState;
	UINT64 qwActorId;
	UINT32 dwTime;
	AccountSession():qwToken(0),qwAccountId(0),dwState(0),qwActorId(0),dwTime(0){}

};

class SessionMgr
{
	SessionMgr();
	~SessionMgr();
	DECLARE_SINGLETON(SessionMgr);
public:
	bool Init();
	void Uninit();
	void Update();
	AccountSession* GetUserSession(UINT64 qwUserId);
	void AddUserSession(AccountSession *pSession);
	void AddConnSession(AccountSession* pSession,UINT32 dwConnID);
	AccountSession *GetConnSession(UINT32 dwConnID);
private:
	std::map<UINT64,AccountSession *> m_mapUserSession; 
	std::map<UINT32,AccountSession *> m_mapConnSession; 
	UINT32 m_nTime;
	UINT64 m_nId;
	UINT64 m_qwTime;
	bool m_bFlag;
};

#endif