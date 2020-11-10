#include "crpc.h"
#include "timermgr.h"
#include "LogInit.h"
#include <assert.h>
#include <vector>

static UINT32 sTagID = 0;
static UINT32 sDelayRpcID = 0;
static bool   sRpcTimeoutEnabled = true;

enum CRpcTimeoutType
{
	CRPC_REPLY_TIMEOUT,
	CRPC_DELAY_REPLY_TIMEOUT,
};

CRpc::CRpc(UINT32 dwType)
{
	m_dwType = dwType;
	m_dwTag = 0;
	m_dwDelayID = 0;
	m_sessionID = 0;
	m_dwTimeout = 5000;
	m_handler = INVALID_HTIMER;
	m_isDelayed = false;
	m_compress = false;

	m_ArgPtr = NULL;
	m_ResPtr = NULL;
	//LogDebug("Create Rpc at %x", this);
}

CRpc::~CRpc()
{
	//LogDebug("delete Rpc at %x", this);

	if (INVALID_HTIMER != m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = INVALID_HTIMER;
	}
}

static std::unordered_map<UINT32, CRpc*> gRpcRegister;
static std::unordered_map<UINT32, CRpc*> gRpcSavedByTags;
static std::unordered_map<UINT32, CRpc*> gRpcDelayRpc;
static std::vector<CRpc*>				 gRpcDeleteList;


bool CRpc::RegistRpc(CRpc *poRpc)
{
	auto i = gRpcRegister.find(poRpc->m_dwType);
	if (i == gRpcRegister.end())
	{
		gRpcRegister[poRpc->m_dwType] = poRpc;
		return true;
	}

	return false;
}

void CRpc::UnRegister()
{
	for(std::unordered_map<UINT32, CRpc*>::iterator it = gRpcRegister.begin(); it != gRpcRegister.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	for(std::unordered_map<UINT32, CRpc*>::iterator it = gRpcSavedByTags.begin(); it != gRpcSavedByTags.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	for(std::unordered_map<UINT32, CRpc*>::iterator it = gRpcDelayRpc.begin(); it != gRpcDelayRpc.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}

	gRpcRegister.clear();
	gRpcSavedByTags.clear();
	gRpcDelayRpc.clear();

	CRpc::Update();
}

CRpc * CRpc::GetRpc(UINT32 dwType)
{
	auto i = gRpcRegister.find(dwType);
	if (i == gRpcRegister.end())
		return NULL;

	return i->second;
}

CRpc * CRpc::GetRpcByTag(UINT32 dwTag)
{
	auto i = gRpcSavedByTags.find(dwTag);
	if (i == gRpcSavedByTags.end())
		return NULL;

	CRpc *rpc = i->second;
	return rpc;
}

void CRpc::RemoveRpcByTag(UINT32 dwTag)
{
	gRpcSavedByTags.erase(dwTag);
}

void CRpc::OnSend()
{
	if (GetEndType() == RPC_CLIENT)
	{
		gRpcSavedByTags[m_dwTag] = this;
		if(sRpcTimeoutEnabled)
		{
			assert(m_handler == INVALID_HTIMER);
			m_handler = CTimerMgr::Instance()->SetTimer(this, CRPC_REPLY_TIMEOUT, m_dwTimeout, 1, __FILE__, __LINE__);
		}
	}
}

void CRpc::SetRpcTag()
{
	if (GetEndType() == RPC_CLIENT)
	{
		m_dwTag = ++sTagID;
	}
}


UINT32 CRpc::DelayRpc()
{
	CRpc *poRpc = NewRpc();
	++sDelayRpcID;
	gRpcDelayRpc[sDelayRpcID] = this;
	gRpcRegister[m_dwType] = poRpc;
	m_isDelayed = true;
	m_dwDelayID = sDelayRpcID;
	if(sRpcTimeoutEnabled)
	{
		assert(m_handler == INVALID_HTIMER);
		m_handler = CTimerMgr::Instance()->SetTimer(this, CRPC_DELAY_REPLY_TIMEOUT, m_dwTimeout, 1, __FILE__, __LINE__);
	}
	return sDelayRpcID;
}

void CRpc::ReplyDelayRpc(UINT32 dwRpcID, const CUserData &userData)
{
	auto i = gRpcDelayRpc.find(dwRpcID);
	if (i == gRpcDelayRpc.end())
	{
		LogWarn("delay rpc delayid [%u] not found!", dwRpcID);
		return;
	}

	CRpc *poRpc = i->second;
	gRpcDelayRpc.erase(i);
	poRpc->OnDelayRpc(userData);
	CNetProcessor::Instance()->Send(poRpc->m_dwConnID, *poRpc);
	//delete poRpc;

	if(INVALID_HTIMER != poRpc->m_handler)
	{
		CTimerMgr::Instance()->KillTimer(poRpc->m_handler);
		poRpc->m_handler = INVALID_HTIMER;
	}
	gRpcDeleteList.push_back(poRpc);
}

void CRpc::SetUserData(const CUserData &userData)
{
	m_oUser = userData;
}

CUserData & CRpc::GetUserData()
{
	return m_oUser;
}

void CRpc::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (CRPC_REPLY_TIMEOUT == dwID)
	{
		OnReplyTimeout();

		LogWarn("Rpc tag [%u] type [%u] is time out!\n", m_dwTag, m_dwType);

		auto i = gRpcSavedByTags.find(m_dwTag);
		if (i == gRpcSavedByTags.end())
		{
			LogWarn("Remove Rpc tag [%u] type [%u]  in gRpcSavedByTags failed!", m_dwTag, m_dwType);
		}
		else
		{
			gRpcSavedByTags.erase(i);
		}

		delete this;
	}
	else if (CRPC_DELAY_REPLY_TIMEOUT == dwID)
	{
		OnDelayReplyTimeout();

		LogWarn("Delay Rpc delayID [%u] type [%u] is time out!\n", m_dwDelayID, m_dwType);

		auto i = gRpcDelayRpc.find(m_dwDelayID);
		if (i == gRpcDelayRpc.end())
		{
			LogWarn("Remove Delay Rpc delayID [%u] in gRpcDelayRpc failed!", m_dwTag);
		}
		else
		{
			gRpcDelayRpc.erase(i);
		}

		delete this;
	}
}

bool CRpc::DeSerialize(const void *data, int len)
{
	if (GetEndType() == RPC_CLIENT)
	{
		m_ResPtr->Clear();
		if (m_ResPtr->ParseFromArray(data, len))
		{
			//LogInfo("%s", m_ResPtr->DebugString().c_str());
			return true;
		}
		
		return false;
	}
	else
	{
		m_ArgPtr->Clear();
		if (m_ArgPtr->ParseFromArray(data, len))
		{
			//LogInfo("%s", m_ArgPtr->DebugString().c_str());
			return true;
		}

		return false;
	}
}

int CRpc::Serialize(void *data, int len) const
{
	google::protobuf::MessageLite *message = (GetEndType() == RPC_CLIENT) ? m_ArgPtr : m_ResPtr;

	try 
	{
		message->SerializeToArray(data, len);
		return message->ByteSize();
	}
	catch (google::protobuf::FatalException &ex)
	{
		LogWarn("serialize rpc [%u] failed! %s", m_dwType, ex.what());
		return -1;
	}
}

void CRpc::SetDontCompress()
{
	m_compress = false;
}

void CRpc::SetTimeoutEnabled(bool bEnabled)
{
	sRpcTimeoutEnabled = bEnabled;
}

void CRpc::Update()
{
	if(gRpcDeleteList.empty()) return;

	for(std::vector<CRpc*>::size_type i = 0; i < gRpcDeleteList.size(); ++i)
	{
		CRpc* poRpc = gRpcDeleteList[i];
		delete poRpc;
	}
	gRpcDeleteList.clear();
}
