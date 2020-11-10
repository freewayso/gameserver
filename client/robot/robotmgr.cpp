#include "pch.h"
#include "robotmgr.h"

#include "reconnetmgr.h"
#include "tinyxml2.h"
#include "LogInit.h"
#include "baselink.h"
#include "network/clientlink.h"
#include "network/loginlink.h"
#include "timermgr.h"
#include "netpacketcoder.h"
#include "util.h"
#include "protocol.h"
#include "keepalivemgr.h"
#include "protocol.h"
#include "crpc.h"
#include "timeutil.h"




CNetPacketCoder *gRobotPacketCoder = NULL;
using namespace tinyxml2;

INSTANCE_SINGLETON(RobotMgr)
RobotMgr::RobotMgr()
{
	
}

RobotMgr::~RobotMgr()
{
	delete m_pLoginCfg;
	m_pLoginCfg = NULL;
}

bool RobotMgr::Init(const char *config)
{
	bool bRet = LoadFile(config);
	if(!bRet)
		return false;
	CTimerMgr::CreateInstance();
	CTimerMgr::Instance()->Init();
	bRet = InitNetwork();
	return bRet;
}


void RobotMgr::Uninit()
{
	CRpc::UnRegister();
	CProtocol::UnRegister();
	CTimerMgr::Instance()->UnInit();
	CTimerMgr::DestroyInstance();
	CKeepAliveMgr::Instance()->Uninit();
	CKeepAliveMgr::DestroyInstance();
}



bool RobotMgr::LoadFile(const char * config)
{
	tinyxml2::XMLDocument doc;
	XMLError error = doc.LoadFile(config);
	if (error != XML_SUCCESS)
	{
		LogError("parser file %s get XMLError code: %d", config, error);
		return false;
	}

	XMLElement *pRootNode = doc.RootElement();
	XMLElement *elem; 
	XMLElement *node;
	elem = pRootNode->FirstChildElement("Params");
	if (elem)
		m_dwMaxClient =  elem->IntAttribute("maxclient");

	elem = pRootNode->FirstChildElement("Connect");
	if (elem)
	{
		for (node = elem->FirstChildElement("peer"); node != NULL; node = node->NextSiblingElement("peer"))
		{ 
			m_pLoginCfg = new PeerInfo;
			m_pLoginCfg->m_action = NA_CONNECT;
			m_pLoginCfg->m_address.m_ip =  node->Attribute("ip");
			m_pLoginCfg->m_address.m_port = node->IntAttribute("port");
			m_pLoginCfg->m_handler = node->Attribute("handler");
			m_pLoginCfg->m_dwSendBuffSize = 1024*10;
			m_pLoginCfg->m_dwRecvBuffSize = 1024*10;
		}
	}

	m_dwLoginBatchNum = 0;
	m_dwLoginExcuteNum = 100;
	m_qwLoginExcuteTime = 0;



	elem = pRootNode->FirstChildElement("Test");
	if (elem)
	{
		m_dwPressActor =  convert<UINT32>(elem->Attribute("pressactor"));
		m_szFixName    =  elem->Attribute("username");
		m_szDeviceId   =  elem->Attribute("deviceid");
	}else
	{
		m_dwPressActor =   1;
		m_szFixName = "";
		m_szDeviceId = "";
	}

	m_dwNum = 0;
	return true;
}

void RobotMgr::DelLoginLinker(LoginLink* pLink)
{
	for (auto iter = m_LoginHandler.begin(); iter != m_LoginHandler.end(); iter++)
	{
		LoginLink* pCli = dynamic_cast<LoginLink*>(*iter);
		if (pCli == pLink)
		{
			m_LoginHandler.erase(iter);
			break;
		}
	}

}

//分批连接
void RobotMgr::ConnectLoginTask()
{
	for (int i = 0; i < m_dwMaxClient; i++)
	{
		PeerInfo * pInfo = new PeerInfo; 
		pInfo->m_id = i;
		pInfo->m_action = NA_CONNECT;
		pInfo->m_address.m_ip =  m_pLoginCfg->m_address.m_ip;
		pInfo->m_address.m_port = m_pLoginCfg->m_address.m_port;
		pInfo->m_handler = "clientlink";
		pInfo->m_dwSendBuffSize = 1024*10;
		pInfo->m_dwRecvBuffSize = 1024*10;
		LoginLink *connLink = new LoginLink(pInfo);
		m_LoginHandler.push_back(connLink);
		m_Network->Connect(inet_addr(pInfo->m_address.m_ip.c_str()), htons(pInfo->m_address.m_port), connLink, gRobotPacketCoder, pInfo->m_dwSendBuffSize, pInfo->m_dwRecvBuffSize);
	}
	LogInfo("cli %u connect sucessful!!!!!",m_dwMaxClient);
}


void RobotMgr::addClient(const std::string &szIP, UINT32 dwPort,UINT64 llUserId,UINT32 dwSendBuffSize,UINT32 dwRecvBuffSize)
{
	static UINT32  perID = 0;
	PeerInfo * pInfo = new PeerInfo; 
	pInfo->m_action = NA_CONNECT;
	pInfo->m_address.m_ip =  szIP;
	pInfo->m_address.m_port = dwPort;
	pInfo->m_handler = "clientlink";
	pInfo->m_dwSendBuffSize = 1024*10;
	pInfo->m_dwRecvBuffSize = 1024*10;
	pInfo->m_id = ++perID;
	ClientLink *connLink = new ClientLink(pInfo);
	connLink->SetUserId(llUserId);
	m_ClientHandler.push_back(connLink);
	m_Network->Connect(inet_addr(pInfo->m_address.m_ip.c_str()), htons(pInfo->m_address.m_port), connLink, gRobotPacketCoder, pInfo->m_dwSendBuffSize, pInfo->m_dwRecvBuffSize);

}

void RobotMgr::delClientLinker(ClientLink *pLink)
{
	for (auto iter = m_ClientHandler.begin(); iter != m_ClientHandler.end(); iter++)
	{
		ClientLink* pCli = dynamic_cast<ClientLink*>(*iter);
		if (pCli == pLink)
		{
			m_ClientHandler.erase(iter);
			break;
		}
	}
}


ClientLink* RobotMgr::getClientLink(UINT32 dwConnId)
{
	for (auto iter = m_ClientHandler.begin(); iter != m_ClientHandler.end(); iter++)
	{
		ClientLink* pCli = dynamic_cast<ClientLink*>(*iter);
		if (pCli->GetConnId() == dwConnId)
		{
			return pCli;
		}
	}
	return NULL;
}



void RobotMgr::DisConnect(UINT32 dwConnId)
{
	m_Network->DisConnect(dwConnId);
}

bool RobotMgr::InitNetwork()
{
	CNetPacketCoder *poPacketCoder = new CNetPacketCoder();
	gRobotPacketCoder	= poPacketCoder;
	CNetProcessor::CreateInstance();
	m_Network = CreateNetWork(CNetProcessor::Instance());
	CNetProcessor::Instance()->Init(m_Network);
	m_dwTaskState		= eCONNECT;
	CKeepAliveMgr::CreateInstance();
	CKeepAliveMgr::Instance()->Init();
	return true;
}


void RobotMgr::ProcessNetMessage()
{
	CTimerMgr::Instance()->Update();
	m_Network->ProcessMsg(2000);
	CRpc::Update();
	Log_Flush();
}



char * RobotMgr::BuildActorName()
{
	static char szActorName[128] =  {0}; 
	static UINT64 llActId = 0;
	sprintf(szActorName,"cbr_%u_%llu",TimeUtil::GetTime(),llActId);
	llActId++;
	return szActorName;
}

char * RobotMgr::BuildUserName()
{
	static char szUserName[128] =  {0}; 
	static UINT64 llId = 0;
	sprintf(szUserName,"jukengdewa_%u_%llu",TimeUtil::GetTime(),llId);
	llId++;
	return szUserName;
}

LoginLink* RobotMgr::FindLinker(UINT32 dwConnID)
{
	for (auto iter = m_LoginHandler.begin(); iter != m_LoginHandler.end(); iter++)
	{
		LoginLink* pLinker = dynamic_cast<LoginLink*>(*iter);
		if (pLinker)
		{
			UINT32 dwConId = pLinker->GetConnId();
			if (dwConId == dwConnID)
			{
				return pLinker;
			}
		}
	}
	return NULL;
}


//连接跟请求在一起好好点..
void RobotMgr::SendLogin()
{
	UINT64 qwCurTime = TimeUtil::GetMilliSecond();
	if (qwCurTime < m_qwLoginExcuteTime)
		return;
	m_qwLoginExcuteTime = qwCurTime + 100;
	UINT32 dwNum = 0;
	for (auto iter = m_LoginHandler.begin(); iter != m_LoginHandler.end(); iter++)
	{
		dwNum++;
		if (dwNum >= m_dwLoginExcuteNum)
			break;	
	}
	m_dwLoginBatchNum++;
	if (m_dwLoginBatchNum>= 10)
	{
		m_dwTaskState = eLOGINIED;
	}
}



void RobotMgr::Update()
{
	switch(m_dwTaskState)
	{
	case eCONNECT:
		{
			ConnectLoginTask();
			m_dwTaskState = eCONNECTING;

		}
		break;
	case eCONNECTING:
		{
			TimeUtil::Sleep(3000);
			m_dwTaskState = eLOGINING;
		}
		break;
	case eLOGINING:
		{
			
			if (RobotMgr::Instance()->m_dwPressActor)
			{
				m_dwTaskState = eCONNECT;
			}else
			{
				m_dwTaskState = eEND;
			}
		}
		break;
	default:
		{
			
		}
	}
}



//第一步连接mysql 获取账号密码
//第二步骤发起任务 连接loginserver 
//第三步骤
void RobotMgr::CheckLinkState()
{

}


