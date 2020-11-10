#include "baselink.h"
#include "protocol.h"
#include "netproc.h"
#include "crpc.h"
#include "LogInit.h"
#include "forward.h"
#include <memory>
#include "util.h"
#include "coder.h"
#include "protostatistics.h"
#include "timeutil.h"
#include "baseconfig.h"
#include "keepalivemgr.h"

#define HEAD_BUF_SIZE 128
#define BIG_PROTOCOL_WARN  Config::GetConfig()->GetProtocolWarningSize()

BaseLink::BaseLink()
{
	m_pForwardInterface = NULL;
	m_printProtocol = false;	
}

BaseLink::~BaseLink()
{
}

bool BaseLink::Send(UINT32 dwConnID, const CProtocol& roPtc)
{
	UINT64 startProcessTime = TimeUtil::GetMicroSecond();

	MemoryStream *stream = MemoryStream::Get();
	stream->Seek(PTC_HEAD_SIZE);

	int len = roPtc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize protocol [%s] failed.", CProtocol::GetProtoName(roPtc.m_dwType));
		return false;
	}

	stream->Advance(len);

	if (roPtc.m_Compress)
	{
		if (!stream->Compress(PTC_HEAD_SIZE))
		{
			LogWarn("compress protocol [%s] failed!", CProtocol::GetProtoName(roPtc.m_dwType));
			//return false;
		}
	}

	ProtocolHead &head = *(ProtocolHead *)stream->GetBuffer();
	head.m_dwLen = stream->GetLength() - sizeof(UINT32);
	head.m_dwType = roPtc.m_dwType;
	head.m_dwFlag = PTC_TYPE_FLAG;
	head.m_dwSessionID = roPtc.m_sessionID;
	if (stream->IsCompressed())
	{
		head.m_dwFlag |= PTC_TYPE_COMPRESS;
	}

	if (stream->GetLength() > BIG_PROTOCOL_WARN)
	{
		if (m_printIgnoreProtocol.find(head.m_dwType) == m_printIgnoreProtocol.end())
		{
			SSWarn << "Large Protocol: " << CProtocol::GetProtoName(head.m_dwType) << " " << stream->GetLength()
				<< " bytes" << END;
		}
	}

	UINT64 endProcessTime = TimeUtil::GetMicroSecond();
	ProtoStatistics::Instance()->AddProtoSendBytes(head.m_dwType, stream->GetLength(), endProcessTime - startProcessTime);
	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}

bool BaseLink::Send(UINT32 dwConnID, CRpc &roRpc)
{
	UINT64 startProcessTime = TimeUtil::GetMicroSecond();

	MemoryStream *stream = MemoryStream::Get();
	stream->Seek(RPC_HEAD_SIZE);

	int len = roRpc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize rpc [%s] failed.", CProtocol::GetProtoName(roRpc.m_dwType));
		return false;
	}

	stream->Advance(len);

	if (roRpc.m_compress == true)
	{
		if (!stream->Compress(RPC_HEAD_SIZE))
		{
			LogWarn("compress protocol [%s] failed!", CProtocol::GetProtoName(roRpc.m_dwType));
			//return false;
		}
	}
	
	roRpc.SetRpcTag();

	RpcHead &head = *(RpcHead *)stream->GetBuffer();
	head.m_dwLen = stream->GetLength() - sizeof(UINT32);
	head.m_dwType = roRpc.m_dwType;
	head.m_dwFlag = (roRpc.GetEndType() == RPC_CLIENT) ? RPC_TYPE_REQUEST :  RPC_TYPE_REPLY;
	head.m_dwSessionID = roRpc.m_sessionID;
	head.m_dwTagID = roRpc.m_dwTag;
	if (stream->IsCompressed())
	{
		head.m_dwFlag |= PTC_TYPE_COMPRESS;
	}

	if (stream->GetLength() > BIG_PROTOCOL_WARN)
	{
		if (m_printIgnoreProtocol.find(head.m_dwType) == m_printIgnoreProtocol.end())
		{
			SSWarn << "Large Protocol: " << CProtocol::GetProtoName(head.m_dwType) << " " << stream->GetLength()
				<< " bytes" << END;
		}
	}

	UINT64 endProcessTime = TimeUtil::GetMicroSecond();
	ProtoStatistics::Instance()->AddProtoSendBytes(head.m_dwType, stream->GetLength(), endProcessTime - startProcessTime);
	bool bSended = CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());



	
	if(bSended)
	{
		roRpc.OnSend();
		return true;
	}
	else
	{
		return false;
	}
}

HeadStruct BaseLink::DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen)
{
	HeadStruct RetInfo;

	memcpy(&RetInfo.m_oPtcHead, pcData, sizeof(ProtocolHead));

	if ((RetInfo.m_oPtcHead.m_dwFlag & RPC_TYPE_FLAG))
	{
		RetInfo.m_IsPtcHead = false;
		RetInfo.m_dwHeadSize = sizeof(RpcHead);
		memcpy(&RetInfo.m_oRpcHead, pcData, sizeof(RpcHead));
	}
	else
	{
		RetInfo.m_IsPtcHead = true;
		RetInfo.m_dwHeadSize = sizeof(ProtocolHead);
	}

	return RetInfo;
}

UINT32 BaseLink::MinSize()
{
	return sizeof(ProtocolHead);
}


void BaseLink::OnRecv(UINT32 dwConnID, const char* pcData, UINT32 dwLen)
{
	//if(dwLen < MinSize())
	//{
	//	//LOG_CRI
	//	return;
	//}


	HeadStruct headStruct = DecodeHead(dwConnID, pcData, dwLen);
	RecvParament parament;
	parament.m_dwConnID = dwConnID;
	parament.m_dwLen = dwLen - headStruct.m_dwHeadSize;
	parament.m_pcData = pcData + headStruct.m_dwHeadSize;
	parament.m_pHead = NULL;
	parament.m_pRpcHead = NULL;

	UINT32 protoType = 0;
	UINT64 startProcessTime = TimeUtil::GetMicroSecond();

	if (headStruct.m_IsPtcHead == false)
	{
		if (dwLen < headStruct.m_dwHeadSize)
		{
			LogWarn("Rpc %s size error, except %d, get %d", CProtocol::GetProtoName(headStruct.m_oRpcHead.m_dwType),
				headStruct.m_dwHeadSize, dwLen);
			return;
		}

		protoType = headStruct.m_oRpcHead.m_dwType;
		parament.m_pRpcHead = &headStruct.m_oRpcHead;
		if (OnRpc(parament) == PTCRES_FORWARD)
		{
			DataBlob rawPacket;
			DataBlob protoBody;
			rawPacket.m_ptr = pcData;
			rawPacket.m_size = dwLen;
			protoBody.m_ptr = parament.m_pcData;
			protoBody.m_size = parament.m_dwLen;
			ForwardRpc(dwConnID, &rawPacket, &protoBody, headStruct.m_oRpcHead);
		}
	}
	else
	{
		//check keep alive falg
		if((headStruct.m_oPtcHead.m_dwFlag & PTC_TYPE_KEEP_ALIVE) != 0)
		{
			CKeepAliveMgr::Instance()->OnRecvKeepAlivePacket(dwConnID);
			return;
		}

		protoType = headStruct.m_oPtcHead.m_dwType;
		parament.m_pHead = &headStruct.m_oPtcHead;
		if (OnProtocol(parament) == PTCRES_FORWARD)
		{
			DataBlob rawPacket;
			DataBlob protoBody;
			rawPacket.m_ptr = pcData;
			rawPacket.m_size = dwLen;
			protoBody.m_ptr = parament.m_pcData;
			protoBody.m_size = parament.m_dwLen;
			ForwardProtocol(dwConnID, &rawPacket, &protoBody, headStruct.m_oPtcHead);
		}
	}

	UINT64 endProcessTime = TimeUtil::GetMicroSecond();
	ProtoStatistics::Instance()->AddProtoRecvBytes(protoType, dwLen, endProcessTime - startProcessTime);
}

void BaseLink::ForwardProtocol(UINT32 dwConnID, DataBlob *rawPacket, DataBlob *protoBody, ProtocolHead &head)
{
	// 检查一下是不是需要转发的协议
	BaseLink *link = ProtocolForward::GetProtocolForwardLink(head.m_dwType);
	if (link != NULL)
	{
		if (m_printProtocol)
		{
			if (m_printIgnoreProtocol.find(head.m_dwType) == m_printIgnoreProtocol.end())
			{
				LogDebug("forward protocol [%s] to link [%s]", CProtocol::GetProtoName(head.m_dwType), link->GetName().c_str());
			}
		}

		if (link->m_pForwardInterface)
		{
			UINT32 dwSendConnID = link->m_pForwardInterface->GetForwardConnID(dwConnID, head);
			if (dwSendConnID == INVALID_CONN_ID)
			{
				//LogWarn("forward ptc to invalid conn id");
				return;
			}

			if (link->m_pForwardInterface->IsChangeHead() == false)
			{
				CNetProcessor::Instance()->Send(dwSendConnID, rawPacket->m_ptr, rawPacket->m_size);
			}
			else
			{
				HeadInfo newHeadInfo;
				DataBlob newDataInfo;
				link->m_pForwardInterface->GetNewHeadInfo(head, protoBody->m_ptr, protoBody->m_size, newHeadInfo, newDataInfo);
				if(newHeadInfo.IsNull())
				{
					LogWarn("link [%s] forward get null new head, can't forward protocol", link->GetName().c_str());
					return;
				}

				MemoryStream *pStream = MemoryStream::Get();
				pStream->Write(newHeadInfo.m_ptr, newHeadInfo.m_size);
				if(newDataInfo.IsNull())
				{
					pStream->Write(protoBody->m_ptr, protoBody->m_size);
				}
				else
				{
					pStream->Write(newDataInfo.m_ptr, newDataInfo.m_size);
				}
				CNetProcessor::Instance()->Send(dwSendConnID, pStream->GetBuffer(), pStream->GetLength());
			}
		}
		else
		{
			LogWarn("link [%s] forward interface is NULL, can't forward protocol", link->GetName().c_str());
		}
	}
	else
	{
		LogWarn("Protocol type [%s] from connid [%u] can't be processed!", CProtocol::GetProtoName(head.m_dwType), dwConnID);
	}
}

BaseLink::PtcResult BaseLink::OnProtocol(RecvParament &parament)
{
	UINT32 dwConnID = parament.m_dwConnID;
	ProtocolHead &head = *parament.m_pHead;
	const char *pcData = parament.m_pcData;
	UINT32 dwLen = parament.m_dwLen;

	// 是不是本server能够处理的协议
	CProtocol* poProtocol = CProtocol::GetProtocol(head.m_dwType);
	if(poProtocol == NULL)
	{
		return PTCRES_FORWARD;
	}

	if (head.m_dwFlag & PTC_TYPE_COMPRESS)
	{
		ZlibTool *pZlib = ZlibTool::Get();
		if (!pZlib->DeCompress(pcData, dwLen))
		{
			LogWarn("decompress protocol [%s] failed!", CProtocol::GetProtoName(head.m_dwType));
			return PTCRES_ERROR;
		}

		pcData = pZlib->GetBuffer();
		dwLen = pZlib->GetLength();
	}

	if (poProtocol->DeSerialize(pcData, dwLen))
	{
		poProtocol->m_sessionID = head.m_dwSessionID;
		poProtocol->Process(dwConnID);
		poProtocol->DebugPrint();
	}
	else
	{
		LogError("decode protocol [%s] failed!, ", CProtocol::GetProtoName(head.m_dwType));
		return PTCRES_ERROR;
	}

	return PTCRES_SUCCESS;
}

BaseLink::PtcResult BaseLink::OnRpc(RecvParament &parament)
{
	UINT32 dwConnID = parament.m_dwConnID;
	const char *pcData = parament.m_pcData;
	int dwLen = (int)parament.m_dwLen;
	RpcHead &head = *parament.m_pRpcHead;

	// 是不是本server能够处理的RPC
	CRpc *poRpc = CRpc::GetRpc(head.m_dwType);
	if (poRpc == NULL)
	{
		return PTCRES_FORWARD;
	}

	std::unique_ptr<CRpc> deleter;
	if (poRpc->GetEndType() == RPC_CLIENT)
	{
		// 这个RPC是一个回复消息，按RPC的tag找到以前缓存的RPC信息
		poRpc = CRpc::GetRpcByTag(head.m_dwTagID);
		if (poRpc == NULL)
		{
			LogWarn("Rpc Tag %u not found!", head.m_dwTagID);
			return PTCRES_ERROR;
		}

		CRpc::RemoveRpcByTag(head.m_dwTagID);
		// 缓存的RPC，在处理完了之后要自动删除掉
		deleter.reset(poRpc);
	}
	else
	{
		poRpc->m_dwTag = head.m_dwTagID;
	}

	if (head.m_dwFlag & PTC_TYPE_COMPRESS)
	{
		ZlibTool *pZlib = ZlibTool::Get();
		if (!pZlib->DeCompress(pcData, dwLen))
		{
			LogWarn("decompress protocol [%s] failed!", CProtocol::GetProtoName(head.m_dwType));
			return PTCRES_ERROR;
		}

		pcData = pZlib->GetBuffer();
		dwLen = pZlib->GetLength();
	}

	if (poRpc->DeSerialize(pcData, dwLen))
	{
		poRpc->m_sessionID = head.m_dwSessionID;
		poRpc->Process(dwConnID);
	}
	else
	{
		LogError("decode rpc [%s] failed!", CProtocol::GetProtoName(head.m_dwType));
		return PTCRES_ERROR;
	}

	return PTCRES_SUCCESS;
}

void BaseLink::ForwardRpc(UINT32 dwConnID, DataBlob *rawPacket, DataBlob *protoBody, RpcHead &head)
{
	// 转发RPC
	bool isRpcRequest = ((head.m_dwFlag & RPC_TYPE_ISREQUEST_MASK) != 0);
	BaseLink *link = RpcForward::GetRpcForwardLink(head.m_dwType, isRpcRequest);
	if (link != NULL)
	{
		if (m_printProtocol)
		{
			if (m_printIgnoreProtocol.find(head.m_dwType) == m_printIgnoreProtocol.end())
			{
				LogDebug("forward rpc [%s] <%s> to link [%s]", CProtocol::GetProtoName(head.m_dwType), isRpcRequest ? "Request" : "Reply", link->GetName().c_str());
			}
		}

		if (link->m_pForwardInterface)
		{
			UINT32 dwSendConnID = link->m_pForwardInterface->GetForwardConnID(dwConnID, head);
			if (dwSendConnID == INVALID_CONN_ID)
			{
				LogWarn("forward rpc [%s] <%s> to invalid conn id [%d]", CProtocol::GetProtoName(head.m_dwType), isRpcRequest ? "Request" : "Reply", dwConnID);
				return;
			}

			if (link->m_pForwardInterface->IsChangeHead() == false)
			{
				CNetProcessor::Instance()->Send(dwSendConnID, rawPacket->m_ptr, rawPacket->m_size);
			}
			else
			{
				HeadInfo newHeadInfo;
				DataBlob newDataInfo;
				link->m_pForwardInterface->GetNewHeadInfo(head, protoBody->m_ptr, protoBody->m_size, newHeadInfo, newDataInfo);
				if(newHeadInfo.IsNull())
				{
					LogWarn("link [%s] forward get null new head, can't forward rpc", link->GetName().c_str());
					return;
				}

				MemoryStream *pStream = MemoryStream::Get();
				pStream->Write(newHeadInfo.m_ptr, newHeadInfo.m_size);
				if(newDataInfo.IsNull())
				{
					pStream->Write(protoBody->m_ptr, protoBody->m_size);
				}
				else
				{
					pStream->Write(newDataInfo.m_ptr, newDataInfo.m_size);
				}
				CNetProcessor::Instance()->Send(dwSendConnID, pStream->GetBuffer(), pStream->GetLength());
			}
		}
		else
		{
			LogWarn("link [%s] forward interface is NULL, can't forward rpc", link->GetName().c_str());
		}
	}
	else
	{
		LogWarn("rpc type [%s] from connid [%u] can't be processed!", CProtocol::GetProtoName(head.m_dwType), dwConnID);
	}
}

void BaseLink::SetPrintProtocol(bool flag)
{
	m_printProtocol = flag;
}

void BaseLink::AddPrintIgnoreProtocol(UINT32 proto)
{
	m_printIgnoreProtocol.insert(proto);
}

void BaseLink::SetForwardInterface(ForwardInterface *i)
{
	m_pForwardInterface = i;
}

ForwardInterface * BaseLink::GetForwardInterface()
{
	return m_pForwardInterface;
}
