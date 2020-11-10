#include "define.h"
#include "forward.h"
#include <unordered_map>

static std::unordered_map<UINT32, BaseLink*> gForwardProtocolMap;

void ProtocolForward::RegistForward(UINT32 dwType, BaseLink *link)
{
	gForwardProtocolMap[dwType] = link;
}

BaseLink * ProtocolForward::GetProtocolForwardLink(UINT32 dwType)
{
	auto i = gForwardProtocolMap.find(dwType);
	return (i == gForwardProtocolMap.end()) ? NULL : i->second;
}

struct RpcForwardLink
{
	BaseLink *m_reqForwardLink;
	BaseLink *m_rplForwardLink;
};

static std::unordered_map<UINT32, RpcForwardLink> gForwardRpcMap;

void RpcForward::RegistForward(UINT32 dwType, BaseLink *reqForwardLink, BaseLink *rplForwardLink)
{
	RpcForwardLink forward;
	forward.m_reqForwardLink = reqForwardLink;
	forward.m_rplForwardLink = rplForwardLink;
	gForwardRpcMap[dwType] = forward;
}

BaseLink * RpcForward::GetRpcForwardLink(UINT32 dwType, bool isRequest)
{
	auto i = gForwardRpcMap.find(dwType);
	if (i == gForwardRpcMap.end())
	{
		return NULL;
	}

	if (isRequest)
	{
		return i->second.m_reqForwardLink;
	}
	else
	{
		return i->second.m_rplForwardLink;
	}
}

