#include "pch.h"
#include "clientlink.h"
#include "network.h"
#include "netproc.h"
#include "crpc.h"
#include "protocol/common/ptcc2t_createnpc.h"
#include "account/rpcc2l_regaccount.h"
#include "game/rpcc2t_entergametest.h"
#include "protocolcoder.h"
#include "session/usermgr.h"
#include "game/rpcc2t_createactorcli2gate.h"
#include "robot/robotmgr.h"
#include "game/rpcc2t_clienttokenauth.h"




ClientLink::ClientLink()
{
	m_dwConnID = INVALID_CONN_ID;
	m_pPeerInfo = NULL;
}

ClientLink::ClientLink(PeerInfo* peer)
{
	m_pPeerInfo = peer;
	m_dwConnID = INVALID_CONN_ID;
	SetForwardInterface(GetForwardInterface());
}

ClientLink::~ClientLink()
{
	delete m_pPeerInfo;
}

void ClientLink::OnConnectSucceed(UINT32 dwConnID)
{
	m_dwConnID = dwConnID;
	UINT64 qwUserId = GetUserId();
	if (qwUserId == 0)
		return;
	AccountSession *pSession = SessionMgr::Instance()->GetUserSession(qwUserId);
	if (pSession)
	{
		SessionMgr::Instance()->AddConnSession(pSession, dwConnID);
		RpcC2T_ClientTokenAuth *pTokenRpc = RpcC2T_ClientTokenAuth::CreateRpc();
		pTokenRpc->m_oArg.set_token(pSession->qwToken);
		pTokenRpc->m_oArg.set_userid(qwUserId);
		Send(dwConnID, *pTokenRpc);

	}

}




bool ClientLink::Send(UINT32 dwConnID, CRpc &roRpc)
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
	roRpc.SetRpcTag();
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


void ClientLink::RpcNullReply(UINT32 dwConnID, UINT32 dwRpcType, UINT32 dwRpcTagID)
{
	MemoryStream *stream = MemoryStream::Get();

	const int HeadSize = sizeof(ClientRpcHead);
	stream->Seek(0);

	ClientRpcHead head;
	head.dwLen = HeadSize - sizeof(UINT32);
	head.dwType = dwRpcType;
	head.dwTagID = dwRpcTagID;
	head.dwFlag = (RPC_TYPE_REPLY | RPC_TYPE_NULL_REPLY);

	stream->Write((char *)&head, sizeof(head));
	CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}


HeadStruct ClientLink::DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen)
{
	HeadStruct RetInfo;

	ClientProtocolHead head;
	memcpy(&head, pcData, sizeof(head));

	if ((head.dwFlag & RPC_TYPE_FLAG))
	{
		ClientRpcHead rpcHead;
		memcpy(&rpcHead, pcData, sizeof(ClientRpcHead));

		RetInfo.m_IsPtcHead = false;
		RetInfo.m_dwHeadSize = sizeof(ClientRpcHead);
		RetInfo.m_oRpcHead.m_dwType = rpcHead.dwType;
		RetInfo.m_oRpcHead.m_dwLen = rpcHead.dwLen;
		RetInfo.m_oRpcHead.m_dwFlag = rpcHead.dwFlag;
		RetInfo.m_oRpcHead.m_dwTagID = rpcHead.dwTagID;
		RetInfo.m_oRpcHead.m_dwSessionID = 0;
	}
	else
	{
		RetInfo.m_IsPtcHead = true;
		RetInfo.m_dwHeadSize = sizeof(ClientProtocolHead);
		RetInfo.m_oPtcHead.m_dwType = head.dwType;
		RetInfo.m_oPtcHead.m_dwLen = head.dwLen;
		RetInfo.m_oPtcHead.m_dwFlag = head.dwFlag;
		RetInfo.m_oPtcHead.m_dwSessionID = 0;
	}

	return RetInfo;
}


UINT32 ClientLink::MinSize()
{
	return sizeof(ClientProtocolHead);
}


void ClientLink::GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
{
	// dwLen = 包体长度 + 包头长度 - 4 字节
	RpcChangeHead.dwLen = dwDataLen + sizeof(RpcChangeHead) - sizeof(UINT32);
	RpcChangeHead.dwFlag = srcHead.m_dwFlag;
	RpcChangeHead.dwTagID = srcHead.m_dwTagID;
	RpcChangeHead.dwType = srcHead.m_dwType;

	newHead.m_ptr = (char *)&RpcChangeHead;
	newHead.m_size = sizeof(RpcChangeHead);

	CProtocolCoder::Instance()->Encode(pcData, dwDataLen, newData, RpcChangeHead.dwType, RpcChangeHead.dwFlag);
	if(!newData.IsNull())
	{
		RpcChangeHead.dwLen = newData.m_size + sizeof(RpcChangeHead) - sizeof(UINT32);
	}
}

void ClientLink::GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
{
	// dwLen = 包体长度 + 包头长度 - 4 字节
	PtcChangeHead.dwLen = dwDataLen + sizeof(PtcChangeHead) - sizeof(UINT32);
	PtcChangeHead.dwType = srcHead.m_dwType;
	PtcChangeHead.dwFlag = srcHead.m_dwFlag;

	newHead.m_ptr = (char *)&PtcChangeHead;
	newHead.m_size = sizeof(PtcChangeHead);

	CProtocolCoder::Instance()->Encode(pcData, dwDataLen, newData, PtcChangeHead.dwType, PtcChangeHead.dwFlag);
	if(!newData.IsNull())
	{
		PtcChangeHead.dwLen = newData.m_size + sizeof(PtcChangeHead) - sizeof(UINT32);
	}
}





bool ClientLink::Send(UINT32 dwConnID, const CProtocol& roPtc)
{
	MemoryStream *stream = MemoryStream::Get();
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

void ClientLink::OnConnectFailed()
{
	LogError("connect to gate server failed!");
	m_dwConnID = INVALID_CONN_ID;
	RobotMgr::Instance()->delClientLinker(this);
	delete this;
}

void ClientLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}

void ClientLink::OnClose(UINT32 dwConnID, int err)
{
	LogError("gate server link closed, error code: %d", err);
	m_dwConnID = INVALID_CONN_ID;
	RobotMgr::Instance()->delClientLinker(this);
	delete this;
}



const std::string& ClientLink::GetName()
{
	static const std::string LinkName = "clientlink";
	return LinkName;
}
