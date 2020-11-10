#include "pch.h"
#include "LogInit.h"
#include "usermgr.h"
#include "timeutil.h"
#include "util.h"
#include "crpc.h"

#include "util/gametime.h"
#include "network/loginlink.h"

#include "util/XCommon.h"
#include "account/rpcc2l_regaccount.h"
#include "login/rpcc2l_loginreq.h"



INSTANCE_SINGLETON(SessionMgr);


SessionMgr::SessionMgr()
{

}


SessionMgr::~SessionMgr()
{
}

bool SessionMgr::Init()
{ 
	m_nTime = GameTime::GetTime();
	m_nId   = 0;
	m_qwTime = 0;
	m_bFlag = true;
	return true;
}


void SessionMgr::Uninit()
{
	for(auto iter = m_mapUserSession.begin(); iter != m_mapUserSession.end(); iter++)
	{
		delete iter->second;
	}
	m_mapUserSession.clear();
}

void SessionMgr::Update()
{
}

AccountSession*  SessionMgr::GetUserSession(UINT64 llUserId)
{
	return m_mapUserSession[llUserId];
}

void SessionMgr::AddUserSession(AccountSession* pSession)
{
	if (pSession==NULL) return;
	m_mapUserSession[pSession->qwAccountId] = pSession;
}



void SessionMgr::AddConnSession(AccountSession* pSession,UINT32 dwConnID)
{
	if (pSession==NULL) return;
	auto iter = m_mapConnSession.find(dwConnID);
	if (iter != m_mapConnSession.end())
	{
		LogError("-------connid is exist[%u]",dwConnID);
	}
	m_mapConnSession[dwConnID] = pSession;
}


AccountSession *SessionMgr::GetConnSession(UINT32 dwConnID)
{
	auto iter = m_mapConnSession.find(dwConnID);
	if (iter==m_mapConnSession.end())
	{
		return NULL;
	}
	return iter->second;
}
