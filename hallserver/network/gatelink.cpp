#include "../pch.h"
#include "gatelink.h"
#include "netproc.h"
#include "protocol.h"


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

CBR::ErrorCode GateLink::AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwZoneid)
{
	if(IsLineConnected(dwServerID))
	{
		LogError("Gate server serverid [%u] has been registered", dwServerID);
		return CBR::ERR_FAILED;
	}

	GateServerInfo oInfo;
	oInfo.dwServerid = dwServerID;
	oInfo.dwZoneid   = dwZoneid;

	m_oGtInfoMap[dwServerID] = oInfo;
	RegisterLine(dwServerID, dwConnID);
	return CBR::ERR_SUCCESS;
}
