#include "pch.h"
#include "loginlink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "util/XRandom.h"
#include "protocol.h"
#include "LogInit.h"
#include "forward.h"
#include "reconnetmgr.h"

#include "common/rpct2l_registergate2login.h"



INSTANCE_SINGLETON(LoginLink)


LoginLink::LoginLink(){}

LoginLink::~LoginLink(){}

void LoginLink::OnConnectSucceed(UINT32 dwConnID)
{
	RpcT2L_RegisterGate2Login* rpc = RpcT2L_RegisterGate2Login::CreateRpc();
	rpc->m_oArg.set_serverid(GateConfig::Instance()->GetServerID());
	std::string link = "loginlink";
	PeerInfo* peerInfo = ReconnectManager::Instance()->getPeerInfo(link);
	rpc->m_oArg.set_name(GateConfig::Instance()->GetServerName());
	rpc->m_oArg.set_ip(peerInfo->m_address.m_ip);
	rpc->m_oArg.set_port(peerInfo->m_address.m_port);

	AddDBServer(dwConnID);
	Send(dwConnID, *rpc);
}

void LoginLink::OnConnectFailed()
{
	LogError("connect to login server failed!");
}

void LoginLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}


void LoginLink::RemoveConnId(UINT32 connId)
{
	for (auto it = m_connIds.begin(); it != m_connIds.end(); ++it)
	{
		if (*it == connId)
		{
			m_connIds.erase(it);
			break;
		}
	}
}

void LoginLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("login link was disconnected[%u]", dwConnID);
	RemoveConnId(dwConnID);
}

bool LoginLink::IsConnected()
{
	return m_connIds.empty()==false;
}

bool LoginLink::SendTo(const CProtocol& roPtc)
{
	if (m_connIds.empty()==true)
	{
		return false;
	}
	UINT32 nIdx = 0; /*XRandom::randInt(0,m_connIds.size())*/
	return Send(m_connIds[0], roPtc);
}
//≤ªø…”√
bool LoginLink::SendTo(CRpc &roRpc)
{
	if (m_connIds.empty()==true)
	{
		LogError("login connect is disabled");
		return false;
	}
//	UINT32 nIdx = XRandom::randInt(0,m_connIds.size());
	return Send(m_connIds[0], roRpc);
}

const std::string& LoginLink::GetName()
{
	static const std::string LinkName = "loginlink";
	return LinkName;
}

void LoginLink::AddDBServer(UINT32 dwConnID)
{
	bool found = false;
	for (size_t i = 0; i < m_connIds.size(); ++i)
	{
		if (m_connIds[i] == dwConnID)
		{
			found = true;
			break;
		}
	}
	if (found == false)
	{
		m_connIds.push_back(dwConnID);
	}
}
