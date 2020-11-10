#include "pch.h"
#include "clientlink.h"
#include "crpc.h"
#include "protocol.h"
#include "connection/connectionmgr.h"
#include "session/sessionmgr.h"


INSTANCE_SINGLETON(CClientLink)

CClientLink::CClientLink()
{ 
}

CClientLink::~CClientLink()
{ 
}

void CClientLink::OnPassiveConnect(UINT32 dwConnID)
{
	LogInfo("a client is connected");
	SessionManager::Instance()->OnNewConnection(dwConnID);
}

void CClientLink::OnClose(UINT32 dwConnID, int err)
{
	SessionManager::Instance()->OnCloseSession(dwConnID);
}

const std::string & CClientLink::GetName()
{
	static const std::string LinkName = "clientlink";
	return LinkName;
}

void CClientLink::Close(UINT32 dwConnID)
{
	CNetProcessor::Instance()->Close(dwConnID);
}


bool CClientLink::Send(UINT32 dwConnID, const CProtocol& roPtc)
{
	MemoryStream *stream = MemoryStream::Get();
	// 发送给客户端的包头，不包含Session字段
	const int HeadSize = sizeof(ClientProtocolHead);
	stream->Seek(HeadSize);

	int len = roPtc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize protocol [%u] failed.", roPtc.m_dwType);
		return false;
	}

	stream->Advance(len);

	ClientProtocolHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roPtc.m_dwType;
	head.dwFlag = PTC_TYPE_FLAG;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());


}

bool CClientLink::Send(UINT32 dwConnID, CRpc &roRpc)
{
	MemoryStream *stream = MemoryStream::Get();

	const int HeadSize = sizeof(ClientRpcHead);
	stream->Seek(HeadSize);

	int len = roRpc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize rpc [%u] failed.", roRpc.m_dwType);
		return false;
	}

	stream->Advance(len);
	roRpc.OnSend();

	ClientRpcHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roRpc.m_dwType;
	head.dwTagID = roRpc.m_dwTag;
	head.dwFlag = (roRpc.GetEndType() == RPC_CLIENT) ? RPC_TYPE_REQUEST : RPC_TYPE_REPLY;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());


}

HeadInfo CClientLink::GetNewHeadInfo(const ProtocolHead &head, UINT32 dataLen)
{
	// dwLen = 包体长度 + 包头长度 - 4 字节
	PtcChangeHead.dwLen = dataLen + sizeof(PtcChangeHead) - sizeof(UINT32);
	PtcChangeHead.dwType = head.m_dwType;
	PtcChangeHead.dwFlag = head.m_dwFlag;

	HeadInfo oInfo;
	oInfo.m_ptr = (char *)&PtcChangeHead;
	oInfo.m_size = sizeof(PtcChangeHead);
	return oInfo;
}

HeadInfo CClientLink::GetNewHeadInfo(const RpcHead &head, UINT32 dataLen)
{
	// dwLen = 包体长度 + 包头长度 - 4 字节
	RpcChangeHead.dwLen = dataLen + sizeof(RpcChangeHead) - sizeof(UINT32);
	RpcChangeHead.dwFlag = head.m_dwFlag;
	RpcChangeHead.dwTagID = head.m_dwTagID;
	RpcChangeHead.dwType = head.m_dwType;

	HeadInfo oInfo;
	oInfo.m_ptr = (char *)&RpcChangeHead;
	oInfo.m_size = sizeof(RpcChangeHead);
	return oInfo;
}


UINT32 CClientLink::MinSize()
{
	return sizeof(ClientProtocolHead);
}
