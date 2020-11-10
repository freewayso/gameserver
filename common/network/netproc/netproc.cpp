#include "netproc.h"
#include "network.h"
#include "baselink.h"
#include "protocol.h"
#include "define.h"
#include "forward.h"
#include "LogInit.h"
#include "crpc.h"
#include "keepalivemgr.h"


INSTANCE_SINGLETON(CNetProcessor)


CNetProcessor::CNetProcessor()
:m_poNetwork(NULL)
{
}

CNetProcessor::~CNetProcessor()
{ 
}

void CNetProcessor::Init(INetWork* poNetWork)
{
	m_poNetwork = poNetWork; 
}

void CNetProcessor::UnInit()
{
}

void CNetProcessor::OnConnectSucceed(UINT32 dwConnID, void* pUserData)
{
	BaseLink *link = reinterpret_cast<BaseLink *>(pUserData);
	m_Links[dwConnID] = link;
	link->OnConnectSucceed(dwConnID);
	CKeepAliveMgr::Instance()->Add(dwConnID, false);
}

void CNetProcessor::OnConnectFailed(INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData)
{
	BaseLink *link = reinterpret_cast<BaseLink *>(pUserData);
	link->OnConnectFailed();
}

void CNetProcessor::OnPassiveConnect(UINT32 dwConnID, void* pUserData)
{
	BaseLink *link = reinterpret_cast<BaseLink *>(pUserData);
	m_Links[dwConnID] = link;
	link->OnPassiveConnect(dwConnID);
	if(link->UseKeepAlive())
	{
		CKeepAliveMgr::Instance()->Add(dwConnID, true);
	}
}

static const char *NetErrorString(INT32 nNetErrCode)
{
#define ENUMMAP(X) if (nNetErrCode == X) return #X
	ENUMMAP(NET_NO_ERROR);
	ENUMMAP(NET_SYS_ERROR);
	ENUMMAP(NET_INVALID_PACKET_SIZE);
	ENUMMAP(NET_CONNECT_FAILED);
	ENUMMAP(NET_LISTEN_FAILED);
	ENUMMAP(NET_ACCEPT_FAILED);
	ENUMMAP(NET_SEND_FAILED);
	ENUMMAP(NET_RECV_FAILED);
	ENUMMAP(NET_ALLOC_FAILED);
	ENUMMAP(NET_SEND_BUFF_OVERFLOW);
	ENUMMAP(NET_RECV_BUFF_OVERFLOW);
	ENUMMAP(NET_ENCODE_BUFF_OVERFLOW);
	ENUMMAP(NET_DECODE_BUFF_OVERFLOW);
	ENUMMAP(NET_SEND_PIPE_OVERFLOW);
	ENUMMAP(NET_RECV_PIPE_OVERFLOW);
	return "NET_UNKNOW_ERROR";
#undef ENUMMAP
}

void CNetProcessor::OnClose(UINT32 dwConnID, INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData)
{
	//LogInfo("ConnID: %u Closed", dwConnID);

	BaseLink *link = reinterpret_cast<BaseLink *>(pUserData);
	if (nNetErrCode != NET_NO_ERROR || nSysErrCode != 0)
	{
		LogInfo("socket closed reason [%s] errno=[%d] for link [%s] ConnID:[%d]", NetErrorString(nNetErrCode), nSysErrCode, link->GetName().c_str(), dwConnID);
	}
	
	link->OnClose(dwConnID, nSysErrCode);
	m_Links.erase(dwConnID);
	CKeepAliveMgr::Instance()->Del(dwConnID);
}

void CNetProcessor::OnError(INT32 nNetErrCode, INT32 nSysErrCode)
{
	LogError("Error, NetErrCode: %d, SysErrCode: %d", nNetErrCode, nSysErrCode);
}


void CNetProcessor::OnRecv(UINT32 dwConnID, const char* pcData, UINT32 dwLen, void* pUserData)
{
	//LogInfo("recv %d bytes on ConnID %u", dwLen, dwConnID);
	BaseLink *link = reinterpret_cast<BaseLink *>(pUserData);
	link->OnRecv(dwConnID, pcData, dwLen);
}


bool CNetProcessor::Send(UINT32 dwConnID, const CProtocol& roPtc)
{
	auto i = m_Links.find(dwConnID);
	if (i == m_Links.end())
	{
		LogWarn("ConnID %u not found!", dwConnID);
		return false;
	}

	return i->second->Send(dwConnID, roPtc);
}

bool CNetProcessor::Send(UINT32 dwConnID, CRpc &roRpc)
{
	auto i = m_Links.find(dwConnID);
	if (i == m_Links.end())
	{
		LogWarn("ConnID %u not found!", dwConnID);
		return false;
	}

	return i->second->Send(dwConnID, roRpc);
}

// forward 的时候会调用这个函数
bool CNetProcessor::Send(UINT32 dwConnID, const void* pData, UINT32 dwLen)
{
	return m_poNetwork->Send(dwConnID, pData, dwLen);
}

bool CNetProcessor::GetRemoteAddr(UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort)
{
	return m_poNetwork->GetRemoteAddr(dwConnID, rdwIP, rwPort);
}


void CNetProcessor::Close(UINT32 dwConnID)
{
	m_poNetwork->DisConnect(dwConnID);
}
