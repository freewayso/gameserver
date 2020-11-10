#include "protocol.h"
#include "LogInit.h"



CProtocol::CProtocolMap	CProtocol::sm_oPtcMap;

bool CProtocol::Register(CProtocol* poProtocol, bool debug)
{
	sm_oPtcMap[poProtocol->m_dwType] = poProtocol;
	poProtocol->m_debug = debug;
	return true;
}

void CProtocol::UnRegister() 
{
	for(CProtocolMap::iterator it = sm_oPtcMap.begin(); it != sm_oPtcMap.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	sm_oPtcMap.clear(); 
}

CProtocol* CProtocol::GetProtocol(UINT32 dwType)
{
	auto i = sm_oPtcMap.find(dwType);
	return (i == sm_oPtcMap.end()) ? NULL : i->second;
}

CProtocol::CProtocol(UINT32 dwType) : m_dwType(dwType)
{
	m_message = NULL;
	m_sessionID = 0;
	m_debug = false;
	m_Compress = false;
}


CProtocol::~CProtocol()
{

}

bool CProtocol::DeSerialize(const void *data, int len)
{
	// 消息没有消息体
	if (m_message == NULL)
	{
		if(len > 0)
		{
			LogWarn("receive data %d bytes with null message, type [%u]", len, m_dwType);
		}
		return true;
	}

	m_message->Clear();
	if (m_message->ParseFromArray(data, len))
	{
		//LogInfo("%s", m_message->DebugString().c_str());
		return true;
	}

	return false;
}

int CProtocol::Serialize(void *data, int len) const
{
	if (m_message == NULL)
	{
		return 0;
	}
	
	try 
	{
		m_message->SerializeToArray(data, len);
		return m_message->ByteSize();
	}
	catch (google::protobuf::FatalException &ex)
	{
		LogWarn("serialize protocol [%u] failed! : %s", m_dwType, ex.what());
		return -1;
	}
}

static std::unordered_map<UINT32, const char *> s_ProtoOrRpcName;

void CProtocol::RegistProtoName(UINT32 dwType, const char *Name)
{
	s_ProtoOrRpcName[dwType] = Name;
}

const char * CProtocol::GetProtoName(UINT32 dwType)
{
	auto i = s_ProtoOrRpcName.find(dwType);
	if (i == s_ProtoOrRpcName.end())
	{
		static char buf[64];
		sprintf(buf, "%u", dwType);
		return buf;
	}

	return i->second;
}

void CProtocol::DebugPrint()
{
#ifndef PROTO_MESSAGE_LITE
	if (m_message && m_debug)
	{
		LogDebug("[%s] \n%s", GetProtoName(m_dwType), m_message->DebugString().c_str());
	}
#endif
}

void CProtocol::SetDontCompress()
{
	m_Compress = false;
}

