#ifndef _H_KeepAlive_Mgr_H__
#define _H_KeepAlive_Mgr_H__

#include <map>
#include "timer.h"
#include "network.h"
#include "define.h"
#include "singleton.h"
#include "tinyxml2.h"


class CKeepAliveInfo
{
public:
	CKeepAliveInfo()
		:m_bListen(false)
		,m_bRecvFlag(false)
		,m_dwConnID(INVALID_CONN_ID)
		,m_hTimer(INVALID_HTIMER)
	{
	}
	CKeepAliveInfo(UINT32 dwConnID, bool bIsListen)
		:m_bListen(bIsListen)
		,m_bRecvFlag(false)
		,m_dwConnID(dwConnID)
		,m_hTimer(INVALID_HTIMER)
	{
	}

	~CKeepAliveInfo()
	{
	}

	void Start();
	void Stop();
	void SetRecvFlag(bool bVal) { m_bRecvFlag = bVal; }
private:
	class CKeepAliveTimer : public ITimer
	{
	public:
		CKeepAliveTimer() :m_poInfo(NULL){}
		virtual ~CKeepAliveTimer() {}
		virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

		CKeepAliveInfo*	m_poInfo;
	};

	bool			m_bListen;
	bool			m_bRecvFlag;

	UINT32			m_dwConnID;
	HTIMER			m_hTimer;
	CKeepAliveTimer	m_oTimer;
};


class CKeepAliveMgr
{
	CKeepAliveMgr();
	~CKeepAliveMgr();
	DECLARE_SINGLETON(CKeepAliveMgr);

public:
	bool Init();
	void Uninit();
	void ReadKeepAliveSetting(tinyxml2::XMLElement* poRootNode);

	void Add(UINT32 dwConnID, bool bIsListenPeer);
	void Del(UINT32 dwConnID);

	CKeepAliveInfo* Get(UINT32 dwConnID);
	void OnRecvKeepAlivePacket(UINT32 dwConnID);

	void SetEnabled(bool bVal)	{ m_bEnabled = bVal; }
	bool IsEnabled() const		{ return m_bEnabled; }
private:
	typedef std::map<UINT32, CKeepAliveInfo*>	CKeepAliveInfoMap;

	bool				m_bEnabled;
	CKeepAliveInfoMap	m_oMap;
};

#endif
