#include "pch.h"
#include "gatelink.h"
#include "netproc.h"
#include "protocol.h"
#include "util/XRandom.h"


INSTANCE_SINGLETON(GateLink)


GateLink::GateLink()
{
}

GateLink::~GateLink()
{ 
}

const std::string& GateLink::GetName()
{
	static const std::string LinkName = "gatelink";
	return LinkName;
}

void GateLink::OnPassiveConnect(UINT32 dwConnID)
{
	LogInfo("gateway connect dwConnId=%u",dwConnID);
}

void GateLink::OnLineClosed(UINT32 dwLine)
{
	m_oGtInfoMap.erase(dwLine);
	LogInfo("Gate server line [%u] disconnected!", dwLine);
}

void GateLink::OnLineConnected(UINT32 dwLine)
{
	LogWarn("Gate server line [%u] connected", dwLine);
}

bool GateLink::SendToSession(const CProtocol &roPtc)
{
	UINT32 dwLine = (UINT32)((roPtc.m_sessionID >> SessionGateIDShift) & 0xFF);
	return SendToLine(dwLine, roPtc);
}

CBR::ErrorCode GateLink::AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwZoneid,std::string ip,UINT32 port)
{
	LogInfo("gateway server connect dwconnid=%u,serverid=%u ip=%s %u",dwConnID,dwServerID,dwZoneid,ip.c_str(),port);
	if(IsLineConnected(dwServerID))
	{
		LogError("Gate server serverid [%u] has been registered", dwServerID);
		return CBR::ERR_FAILED;
	}

	GateServerInfo oInfo;
	oInfo.dwServerid = dwServerID;
	oInfo.dwZoneid   = dwZoneid;
	oInfo.dwConnector = dwConnID;
	oInfo.ip = ip;
	oInfo.port = port;
	m_oGtInfoMap[dwServerID] = oInfo;
	RegisterLine(dwServerID, dwConnID);
	return CBR::ERR_SUCCESS;
}



GateServerInfo GateLink::GetServer()
{
	//UINT32 nIdx = XRandom::randInt(0,m_oGtInfoMap.size());
	UINT32 nPlayers =  (UINT32) - 1;
	UINT32 nServerID= 0;
	for(auto iter = m_oGtInfoMap.begin(); iter != m_oGtInfoMap.end(); iter++ )
	{
		if (iter->second.dwPlayers < nPlayers)
		{
			nPlayers = iter->second.dwPlayers;
			nServerID =iter->second.dwServerid;
		}
			
	}
	return m_oGtInfoMap[nServerID];
}


//定期更新各个网关人数
