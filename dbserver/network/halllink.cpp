#include "pch.h"
#include "halllink.h"
#include "network.h"
#include "netproc.h"

INSTANCE_SINGLETON(HallLink);


HallLink::HallLink()
{
}

HallLink::~HallLink()
{

}

void HallLink::OnPassiveConnect(UINT32 dwConnID)
{
	LogInfo("a hallserver is connected to db!");
}

void HallLink::OnClose(UINT32 dwConnID, int err)
{
	bool IsFindConnID = false;
	for (auto i = m_gsConnections.begin(); i != m_gsConnections.end(); ++i)
	{
		if (i->m_dwConnID == dwConnID)
		{
			LogInfo("remove hallserver [%d:%s]", i->m_dwServerID, i->m_serverName.c_str());
			m_gsConnections.erase(i);
			IsFindConnID = true;
			break;
		}
	}

	if (!IsFindConnID)
	{
		LogError("error: OnClose Not found connID!", dwConnID);
	}
}


void HallLink::Close(UINT32 dwConnID)
{
	CNetProcessor::Instance()->Close(dwConnID);
}

const std::string& HallLink::GetName()
{
	static const std::string HalllinkName = "halllink";
	return HalllinkName;
}

HallServerInfo * HallLink::GetServerInfo(UINT32 dwConnID)
{
	for (auto i = m_gsConnections.begin(); i != m_gsConnections.end(); ++i)
	{
		if (i->m_dwConnID == dwConnID)
		{
			return &*i;
		}
	}

	return NULL;
}

HallServerInfo * HallLink::GetServerInfoByServerID(UINT32 dwServerID)
{
	for (auto i = m_gsConnections.begin(); i != m_gsConnections.end(); ++i)
	{
		if (i->m_dwServerID == dwServerID)
		{
			return &*i;
		}
	}

	return NULL;
}

UINT32 HallLink::AddServerInfo(UINT32 dwConnID, UINT32 dwServerID, const std::string &serverName)
{
	if (GetServerInfoByServerID(dwServerID) == NULL)
	{
		HallServerInfo oInfo;
		oInfo.m_dwConnID = dwConnID;
		oInfo.m_serverName = serverName;
		oInfo.m_dwServerID = dwServerID;
		m_gsConnections.push_back(oInfo);
		LogInfo("add hallserver [%d:%s]", dwServerID, serverName.c_str());
		return HALLREGDBSERVER_ERR_SUCCESS;
	}else
	{
		return HALLREGDBSERVER_ERR_FAILED;
	}

}

void HallLink::ShowHSLinkInfo()
{
	LogDebug("Begin Show HSLink Info, ListSize:%u", m_gsConnections.size());
	for( auto i=this->m_gsConnections.begin(); i!= this->m_gsConnections.end(); i++ )
	{
		LogDebug("HallServer Link Info, ConnID:%u, GSID:%u, HSName:%s", (*i).m_dwConnID, (*i).m_dwServerID, (*i).m_serverName.c_str());
	}
	LogDebug("Finish Show HSLink Info, ListSize:%u", m_gsConnections.size());
}
