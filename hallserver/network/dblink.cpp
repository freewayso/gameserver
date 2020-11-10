#include "pch.h"
#include "dblink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "util/XRandom.h"
#include "common/rpch2d_registerhalltodb.h"
#include "protocol.h"
#include "LogInit.h"
#include "forward.h"



INSTANCE_SINGLETON(DBLink)


DBLink::DBLink(){}

DBLink::~DBLink(){}

void DBLink::OnConnectSucceed(UINT32 dwConnID)
{
	RpcH2D_RegisterHallToDb* rpc = RpcH2D_RegisterHallToDb::CreateRpc();
	rpc->m_oArg.set_serverid(HallConfig::Instance()->GetServerID());
	rpc->m_oArg.set_name(HallConfig::Instance()->GetServerName());
	AddDBServer(dwConnID);
	Send(dwConnID, *rpc);
}

void DBLink::OnConnectFailed()
{
	LogError("connect to db server failed!");
}

void DBLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}


void DBLink::RemoveConnId(UINT32 connId)
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

void DBLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("db link was disconnected[%u]", dwConnID);
	RemoveConnId(dwConnID);
}

bool DBLink::IsConnected()
{
	return m_connIds.empty()==false;
}

bool DBLink::SendTo(const CProtocol& roPtc)
{
	if (m_connIds.empty()==true)
	{
		return false;
	}
	UINT32 nIdx = XRandom::randInt(0,m_connIds.size());
	return Send(m_connIds[nIdx], roPtc);
}
//≤ªø…”√
bool DBLink::SendTo(CRpc &roRpc)
{
	if (m_connIds.empty()==true)
	{
		LogError("DBLink connect is disabled");
		return false;
	}
	UINT32 nIdx = XRandom::randInt(0,m_connIds.size());
	return Send(m_connIds[nIdx], roRpc);
}

const std::string& DBLink::GetName()
{
	static const std::string LinkName = "dblink";
	return LinkName;
}

void DBLink::AddDBServer(UINT32 dwConnID)
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
