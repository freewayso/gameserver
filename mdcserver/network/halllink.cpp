#include "pch.h"
#include "halllink.h"



INSTANCE_SINGLETON(HallLink)

HallLink::HallLink()
{

}

HallLink::~HallLink()
{

}

const std::string & HallLink::GetName()
{
	static std::string name = "halllink";
	return name;
}


void HallLink::OnPassiveConnect(UINT32 dwConnID)
{

}


void HallLink::OnClose(UINT32 dwConnID, int err)
{

}

bool HallLink::SendToSession(const CProtocol &roPtc)
{
	return true;
}



CBR::ErrorCode HallLink::AddHallServer(UINT32 dwConnID, UINT32 dwServerID)
{
	auto iter = m_oDmInfoMap.find(dwServerID);
	if (iter == m_oDmInfoMap.end())
	{
		LogInfo("the serverid [%u] is connected,and the connid=[%u] ",dwServerID,dwConnID);
		m_oDmInfoMap[dwServerID].dwConnId = dwConnID;
	}else
	{
		LogInfo("the serverid [%u] is reconnect,and the connid=[%u] ",dwServerID,dwConnID);
		m_oDmInfoMap[dwServerID].dwConnId = dwConnID;
	}
	return CBR::ERR_SUCCESS;
}
