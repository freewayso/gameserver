#include "pch.h"

#include "loginlink.h"
#include "LogInit.h"
#include "network.h"
#include "crpc.h"
#include "util/XRandom.h"
#include "protocol.h"
#include "LogInit.h"
#include "forward.h"
#include "clientlink.h"
#include "timeutil.h"
#include "protocolcoder.h"
#include "reconnetmgr.h"
#include "robot/robotmgr.h"
#include "login/rpcc2l_loginreq.h"
#define ONE_CLIENT_DEF 1

LoginLink::LoginLink()
{
}


LoginLink::LoginLink(PeerInfo* peer)
{
	m_pPeerInfo = peer;
	m_dwConnId = INVALID_CONN_ID;
	SetForwardInterface(GetForwardInterface());
}

LoginLink::~LoginLink()
{
	LogInfo("delete connect link to peer (%s:%d) by link [%u]", m_pPeerInfo->m_address.m_ip.c_str(),m_pPeerInfo->m_address.m_port, m_pPeerInfo->m_id);
	delete m_pPeerInfo;

}



void LoginLink::OnConnectSucceed(UINT32 dwConnID)
{
	m_dwConnId = dwConnID;
	RpcC2L_LoginReq *pRpc =   RpcC2L_LoginReq::CreateRpc();
	std::string devid = RobotMgr::Instance()->BuildUserName();
	if(RobotMgr::Instance()->m_dwPressActor)
	{
		pRpc->m_oArg.set_name(devid);
		pRpc->m_oArg.set_deviceid(devid);
	}else
	{
		pRpc->m_oArg.set_name(RobotMgr::Instance()->m_szDeviceId);
		pRpc->m_oArg.set_deviceid(RobotMgr::Instance()->m_szFixName);

	}
	Send(dwConnID, *pRpc);
}

void LoginLink::OnConnectFailed()
{
	m_dwConnId = INVALID_CONN_ID;
	LogWarn("Connect to [%s:%d] failed by link handler [%s], will reconnect later\n",
		m_pPeerInfo->m_address.m_ip.c_str(), m_pPeerInfo->m_address.m_port, m_pPeerInfo->m_handler.c_str());
	RobotMgr::Instance()->DelLoginLinker(this);

	delete this;
}

void LoginLink::OnClose(UINT32 dwConnID, int err)
{
	m_dwConnId = INVALID_CONN_ID;
	RobotMgr::Instance()->DelLoginLinker(this);
	delete this;
}


bool LoginLink::Send(UINT32 dwConnID, const CProtocol& roPtc)
{
	MemoryStream *stream = MemoryStream::Get();
	const int HeadSize = sizeof(RClientProtocolHead);
	stream->Seek(HeadSize);

	int len = roPtc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize protocol [%u] failed.", roPtc.m_dwType);
		return false;
	}

	stream->Advance(len);

	RClientProtocolHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roPtc.m_dwType;
	head.dwFlag = PTC_TYPE_FLAG;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}




bool LoginLink::Send(UINT32 dwConnID, CRpc &roRpc)
{
	MemoryStream *stream = MemoryStream::Get();
	const int HeadSize = sizeof(RClientRpcHead);
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
	RClientRpcHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roRpc.m_dwType;
	head.dwTagID = roRpc.m_dwTag;
	head.dwFlag = (roRpc.GetEndType() == RPC_CLIENT) ? RPC_TYPE_REQUEST : RPC_TYPE_REPLY;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}


void LoginLink::RpcNullReply(UINT32 dwConnID, UINT32 dwRpcType, UINT32 dwRpcTagID)
{
	MemoryStream *stream = MemoryStream::Get();

	const int HeadSize = sizeof(RClientRpcHead);
	stream->Seek(0);

	RClientRpcHead head;
	head.dwLen = HeadSize - sizeof(UINT32);
	head.dwType = dwRpcType;
	head.dwTagID = dwRpcTagID;
	head.dwFlag = (RPC_TYPE_REPLY | RPC_TYPE_NULL_REPLY);

	stream->Write((char *)&head, sizeof(head));
	CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}

void LoginLink::GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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

void LoginLink::GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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


HeadStruct LoginLink::DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen)
{
	HeadStruct RetInfo;

	RClientProtocolHead head;
	memcpy(&head, pcData, sizeof(head));

	if ((head.dwFlag & RPC_TYPE_FLAG))
	{
		RClientRpcHead rpcHead;
		memcpy(&rpcHead, pcData, sizeof(RClientRpcHead));

		RetInfo.m_IsPtcHead = false;
		RetInfo.m_dwHeadSize = sizeof(RClientRpcHead);
		RetInfo.m_oRpcHead.m_dwType = rpcHead.dwType;
		RetInfo.m_oRpcHead.m_dwLen = rpcHead.dwLen;
		RetInfo.m_oRpcHead.m_dwFlag = rpcHead.dwFlag;
		RetInfo.m_oRpcHead.m_dwTagID = rpcHead.dwTagID;
		RetInfo.m_oRpcHead.m_dwSessionID = 0;
	}
	else
	{
		RetInfo.m_IsPtcHead = true;
		RetInfo.m_dwHeadSize = sizeof(RClientRpcHead);
		RetInfo.m_oPtcHead.m_dwType = head.dwType;
		RetInfo.m_oPtcHead.m_dwLen = head.dwLen;
		RetInfo.m_oPtcHead.m_dwFlag = head.dwFlag;
		RetInfo.m_oPtcHead.m_dwSessionID = 0;
	}

	return RetInfo;
}


UINT32 LoginLink::MinSize()
{
	return sizeof(RClientProtocolHead);
}


const std::string & LoginLink::GetName()
{
	static std::string name = "loginlink";
	return  name;
}