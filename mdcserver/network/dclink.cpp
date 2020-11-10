#include "pch.h"
#include "dclink.h"


INSTANCE_SINGLETON(DCLink)

DCLink::DCLink()
{

}

DCLink::~DCLink()
{

}

const std::string & DCLink::GetName()
{
	static std::string name = "dclink";
	return name;
}


void DCLink::OnPassiveConnect(UINT32 dwConnID)
{

}


void DCLink::OnClose(UINT32 dwConnID, int err)
{

}

bool DCLink::SendToSession(const CProtocol &roPtc)
{
	return true;
}

CBR::ErrorCode DCLink::AddDCServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwZoneid)
{
	return CBR::ERR_SUCCESS;
}
