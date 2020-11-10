#include "reconnetmgr.h"
#include "network.h"
#include "LogInit.h"
#include "timer.h"
#include "baselink.h"
#include "packetcoder.h"
#ifdef WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include "timermgr.h"


// 检查断线重连的Timer
class ReconnectTimer : public ITimer
{
public:
	ReconnectManager *m_reconnectmgr;
	HTIMER  m_handler;

	ReconnectTimer()
	{
		m_reconnectmgr = NULL;
		m_handler = INVALID_HTIMER;
	}

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		m_reconnectmgr->CheckLinkState();
	}
};

static ReconnectTimer gReconnectTimer;

class ConnectLink : public BaseLink
{
public:
	ConnectLink(BaseLink *link, PeerInfo& peer)
	{
		m_pBaseLink = link;
		m_pPeerInfo = peer;

		SetForwardInterface(link->GetForwardInterface());
	}
	~ConnectLink()
	{
		//LogInfo("delete connect link to peer (%s:%d) by link [%s]", m_pPeerInfo->m_address.m_ip.c_str(),
		//	m_pPeerInfo->m_address.m_port, GetName().c_str());
	}
	virtual void OnConnectSucceed(UINT32 dwConnID) 
	{ 
		m_pBaseLink->OnConnectSucceed(dwConnID);
		ReconnectManager::Instance()->SetLinkState(this, m_pPeerInfo, PS_CONNECT_OK);
	}
	virtual void OnConnectFailed() 
	{
		//LogWarn("Connect to [%s:%d] failed by link handler [%s], will reconnect later\n",
		//	m_pPeerInfo->m_address.m_ip.c_str(), m_pPeerInfo->m_address.m_port, m_pPeerInfo->m_handler.c_str());

		m_pBaseLink->OnConnectFailed();
		ReconnectManager::Instance()->SetLinkState(this, m_pPeerInfo, PS_CONNECT_FAILED);
		delete this;
	}
	virtual void OnClose(UINT32 dwConnID, int err)
	{
		m_pBaseLink->OnClose(dwConnID, err);
		ReconnectManager::Instance()->SetLinkState(this, m_pPeerInfo, PS_CONNECT_FAILED);
		delete this;
	}

	virtual void OnPassiveConnect(UINT32 dwConnID) { assert(false); }

	virtual const std::string &GetName() { return m_pBaseLink->GetName(); }

	void SetPeerInfo(PeerInfo& peer) { m_pPeerInfo = peer; }
	const PeerInfo& GetPeerInfo() const { return m_pPeerInfo; }
	void SetBaseLink(BaseLink *link) { m_pBaseLink = link; }

private:
	BaseLink *m_pBaseLink;
	PeerInfo m_pPeerInfo;
};

INSTANCE_SINGLETON(ReconnectManager)

ReconnectManager::ReconnectManager()
{
	m_Network = NULL;
	m_PacketCoder = NULL;
}

ReconnectManager::~ReconnectManager()
{

}

void ReconnectManager::SetLinkState(BaseLink *link, PeerInfo& peerInfo, ConnectState newState)
{
	for (auto i = m_PeersInfo.begin(); i != m_PeersInfo.end(); ++i)
	{
		if (peerInfo == (*i))
		{
			i->m_state = newState;
			return;
		}
	}

	LogWarn("Link [%s] not found!\n", link->GetName().c_str());
}

//添加多一种方式是主动连接服务器
void ReconnectManager::CheckLinkState()
{
	Config *config = Config::GetConfig();
	for (auto i = m_PeersInfo.begin(); i != m_PeersInfo.end(); ++i)
	{
		if (i->m_state == PS_CONNECT_FAILED && i->m_state == true)
		{
			//LogInfo("reconnect handler [%s]", i->m_handler.c_str());
			BaseLink *link = config->FindLinkHandler(i->m_handler);
			ConnectLink *connLink = new ConnectLink(link, *i);
			i->m_state = PS_CONNECTING;
			if (!m_Network->Connect(inet_addr(i->m_address.m_ip.c_str()), htons(i->m_address.m_port), connLink, m_PacketCoder, i->m_dwSendBuffSize, i->m_dwRecvBuffSize))
			{
				i->m_state = PS_CONNECT_FAILED;
				LogWarn("Connect to [%s:%d] failed by link handler [%s], will reconnect later\n", i->m_address.m_ip.c_str(), i->m_address.m_port, i->m_handler.c_str());
			}	
		}
	}
}

void ReconnectManager::TiggerPeerInfo(std::string  &peername)
{
	Config *config = Config::GetConfig();
	for (auto it = m_PeersInfo.begin(); it != m_PeersInfo.end(); ++it)
	{
		PeerInfo& peerInfo = *it;
		if (peerInfo.m_handler==peername)
		{
			BaseLink *link = config->FindLinkHandler(peername);
			ConnectLink *connLink = new ConnectLink(link, *it);
			it->m_state = PS_CONNECTING;

			if (!m_Network->Connect(inet_addr(it->m_address.m_ip.c_str()), htons(it->m_address.m_port), connLink, m_PacketCoder, it->m_dwSendBuffSize, it->m_dwRecvBuffSize))
			{
				it->m_state = PS_CONNECT_FAILED;
				LogWarn("Connect to [%s:%d] failed by link handler [%s], will  later\n", it->m_address.m_ip.c_str(), it->m_address.m_port, it->m_handler.c_str());
			}else
			{
				LogWarn("Connect to [%s:%d]  is sucessful by link handler [%s]\n", it->m_address.m_ip.c_str(), it->m_address.m_port, it->m_handler.c_str());
			}
		}
	}
}


PeerInfo* ReconnectManager::getPeerInfo(std::string &hander)
{
	
	for (auto it = m_PeersInfo.begin(); it != m_PeersInfo.end(); ++it)
	{
		if(it->m_handler==hander)
		{
			PeerInfo *pInfo = new PeerInfo;
			pInfo->m_action = it->m_action;
			pInfo->m_id = it->m_id;
			pInfo->m_state = it->m_state;
			pInfo->m_handler = it->m_handler;
			pInfo->m_address.m_ip = it->m_address.m_ip;
			pInfo->m_address.m_port = it->m_address.m_port;
			pInfo->m_dwRecvBuffSize = it->m_dwRecvBuffSize;
			pInfo->m_dwSendBuffSize = it->m_dwSendBuffSize;
			return pInfo;

		}
	}
	return NULL;

}

void ReconnectManager::AddPeerInfo(PeerInfo &info)
{
	m_PeersInfo.push_back(info);
}


void ReconnectManager::RemovePeerInfo(PeerInfo& info)
{
	for (auto it = m_PeersInfo.begin(); it != m_PeersInfo.end(); ++it)
	{
		PeerInfo& peerInfo = *it;
		if (peerInfo == info)
		{
			m_PeersInfo.erase(it);
			break;
		}
	}
}

bool ReconnectManager::InitNetwork(INetWork *network, IPacketCoder *coder)
{
	m_Network = network;
	m_PacketCoder = coder;
	Config *config = Config::GetConfig();
	for (auto i = m_PeersInfo.begin(); i != m_PeersInfo.end(); ++i)
	{
		BaseLink *link = config->FindLinkHandler(i->m_handler);
		if (link == NULL)
		{
			LogError("can't find link handler [%s]", i->m_handler.c_str());
			return false;
		}

		// Listen 失败就没戏了，Connect失败还可以重连
		if (i->m_action == NA_LISTEN)
		{
			if (!m_Network->Listen(inet_addr(i->m_address.m_ip.c_str()), htons(i->m_address.m_port), link, coder, i->m_dwSendBuffSize, i->m_dwRecvBuffSize))
			{
				LogError("Listen by link handler [%s] on [%s:%d] failed!\n", i->m_handler.c_str(), i->m_address.m_ip.c_str(), i->m_address.m_port);
				return false;
			}
			LogInfo("Listen by link handler [%s] on [%s:%d] success", i->m_handler.c_str(), i->m_address.m_ip.c_str(), i->m_address.m_port);
		}
		else if (i->m_action == NA_CONNECT && i->m_trigger)
		{
			ConnectLink *connLink = new ConnectLink(link, *i);
			i->m_state = PS_CONNECTING;
			if (!m_Network->Connect(inet_addr(i->m_address.m_ip.c_str()), htons(i->m_address.m_port), connLink, coder, i->m_dwSendBuffSize, i->m_dwRecvBuffSize))
			{
				i->m_state = PS_CONNECT_FAILED;
				LogWarn("Connect to [%s:%d] failed by link handler [%s], will reconnect later\n", i->m_address.m_ip.c_str(), i->m_address.m_port, i->m_handler.c_str());
			}
			else
			{
				LogInfo("Begin connect to [%s:%d] by link %s", i->m_address.m_ip.c_str(), i->m_address.m_port, i->m_handler.c_str());
			}
		}
	}

	gReconnectTimer.m_reconnectmgr = this;
	gReconnectTimer.m_handler = CTimerMgr::Instance()->SetTimer(&gReconnectTimer, 0, config->GetReconnectInterval(), -1, __FILE__, __LINE__);
	return true;
}

void ReconnectManager::UnInit()
{
	CTimerMgr::Instance()->KillTimer(gReconnectTimer.m_handler);
}

bool ReconnectManager::Init()
{
	return true;
}
