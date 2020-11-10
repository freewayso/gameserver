#include "pch.h"
#include "mdclink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "util/XRandom.h"
#include "protocol.h"
#include "LogInit.h"
#include "forward.h"
#include "game/rpch2m_reghall2mdc.h"



INSTANCE_SINGLETON(MdcLink)


MdcLink::MdcLink(){}

MdcLink::~MdcLink(){}

void MdcLink::OnConnectSucceed(UINT32 dwConnID)
{
	RpcH2M_RegHall2Mdc* rpc =  RpcH2M_RegHall2Mdc::CreateRpc();
	rpc->m_oArg.set_serverid(HallConfig::Instance()->GetServerID());
	rpc->m_oArg.set_name(HallConfig::Instance()->GetServerName());
	AddServer(dwConnID);
	Send(dwConnID, *rpc);
}

void MdcLink::OnConnectFailed()
{
	LogError("connect to mdc server failed!");
}

void MdcLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}


void MdcLink::RemoveConnId(UINT32 connId)
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

void MdcLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("db link was disconnected[%u]", dwConnID);
	RemoveConnId(dwConnID);
}

bool MdcLink::IsConnected()
{
	return m_connIds.empty()==false;
}

bool MdcLink::SendTo(const CProtocol& roPtc)
{
	if (m_connIds.empty()==true)
	{
		return false;
	}
	UINT32 nIdx = XRandom::randInt(0,m_connIds.size());
	return Send(m_connIds[nIdx], roPtc);
}
//不可用
bool MdcLink::SendTo(CRpc &roRpc)
{
	if (m_connIds.empty()==true)
	{
		LogError("DBLink connect is disabled");
		return false;
	}
	UINT32 nIdx = XRandom::randInt(0,m_connIds.size());
	return Send(m_connIds[nIdx], roRpc);
}

const std::string& MdcLink::GetName()
{
	static const std::string LinkName = "mdcclink";
	return LinkName;
}

void MdcLink::AddServer(UINT32 dwConnID)
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
