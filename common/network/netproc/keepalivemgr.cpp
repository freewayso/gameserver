#include "keepalivemgr.h"
#include "timermgr.h"
#include "netproc.h"
#include "coder.h"
#include "LogInit.h"


#define KEEP_ALIVE_CHECK_INTERVAL (120 * 1000)
#define KEEP_ALIVE_SEND_INTERVAL  (30 * 1000)

INSTANCE_SINGLETON(CKeepAliveMgr)


void CKeepAliveInfo::Start()
{
	Stop();

	m_oTimer.m_poInfo = this;
	m_hTimer = CTimerMgr::Instance()->SetTimer(&m_oTimer, 0, m_bListen ? KEEP_ALIVE_CHECK_INTERVAL : KEEP_ALIVE_SEND_INTERVAL, -1, __FILE__, __LINE__);
}

void CKeepAliveInfo::Stop()
{
	if(m_hTimer != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_hTimer);
		m_hTimer = INVALID_HTIMER;
	}
}

void CKeepAliveInfo::CKeepAliveTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if(m_poInfo->m_bListen)
	{
		if(!m_poInfo->m_bRecvFlag)
		{
			LogWarn("Close connect [%u] by keep alive checking", m_poInfo->m_dwConnID);
			CNetProcessor::Instance()->Close(m_poInfo->m_dwConnID);
			m_poInfo->Stop();
			return;
		}

		m_poInfo->m_bRecvFlag = false;
	}
	else
	{
		ProtocolHead head;
		head.m_dwLen = sizeof(ProtocolHead) - sizeof(UINT32);
		head.m_dwType = 0;
		head.m_dwFlag = PTC_TYPE_FLAG;
		head.m_dwFlag |= PTC_TYPE_KEEP_ALIVE;
		head.m_dwSessionID = 0;
		if(!CNetProcessor::Instance()->Send(m_poInfo->m_dwConnID, &head, sizeof(ProtocolHead)))
		{
			CNetProcessor::Instance()->Close(m_poInfo->m_dwConnID);
			m_poInfo->Stop();
		}
	}
}


CKeepAliveMgr::CKeepAliveMgr()
:m_bEnabled(true)
{
}

CKeepAliveMgr::~CKeepAliveMgr()
{
}

bool CKeepAliveMgr::Init()
{
	return true;
}

void CKeepAliveMgr::Uninit()
{
	for(CKeepAliveInfoMap::iterator it = m_oMap.begin(); it != m_oMap.end(); ++it)
	{
		CKeepAliveInfo* poInfo = it->second;
		poInfo->Stop();
		delete poInfo;
	}
	m_oMap.clear();
}

void CKeepAliveMgr::Add(UINT32 dwConnID, bool bIsListenPeer)
{
	if(!IsEnabled()) return;

	CKeepAliveInfoMap::iterator it = m_oMap.find(dwConnID);
	if(it != m_oMap.end())
	{
		it->second->Stop();
		delete it->second;
		m_oMap.erase(it);
	}

	CKeepAliveInfo* poInfo = new CKeepAliveInfo(dwConnID, bIsListenPeer);
	poInfo->Start();
	m_oMap.insert(std::make_pair(dwConnID, poInfo));
}

void CKeepAliveMgr::Del(UINT32 dwConnID)
{
	CKeepAliveInfoMap::iterator it = m_oMap.find(dwConnID);
	if(it != m_oMap.end())
	{
		it->second->Stop();
		delete it->second;
		m_oMap.erase(it);
	}
}

CKeepAliveInfo* CKeepAliveMgr::Get(UINT32 dwConnID)
{
	CKeepAliveInfoMap::iterator it = m_oMap.find(dwConnID);
	return it == m_oMap.end() ? NULL : it->second;
}

void CKeepAliveMgr::OnRecvKeepAlivePacket(UINT32 dwConnID)
{
	CKeepAliveInfo* poInfo = Get(dwConnID);
	if(poInfo == NULL) return;

	poInfo->SetRecvFlag(true);
}

void CKeepAliveMgr::ReadKeepAliveSetting(tinyxml2::XMLElement* poRootNode)
{
	if(poRootNode == NULL) return;

	tinyxml2::XMLElement* poNode = poRootNode->FirstChildElement("KeepAlive");
	if(poNode != NULL)
	{
		INT32 nVal = poNode->IntAttribute("enabled");
		SetEnabled(nVal != 0);
	}
}