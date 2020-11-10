#include "baseconfig.h"
#include "netproc.h"
#include "netpacketcoder.h"
#include "network.h"
#include "baselink.h"
#include "timermgr.h"
#include "LogInit.h"
#include "reconnetmgr.h"
#include "crpc.h"
#include "protocol.h"
#include "keepalivemgr.h"


using namespace tinyxml2;

CNetPacketCoder *gPacketCoder = NULL;

static Config *gConfig = NULL;

Config::Config()
{
	m_Network = NULL;
	gConfig = this;
	m_ServerID = 0;
	m_ZoneID = 0;
}

Config::~Config()
{
	gConfig = NULL;
}

void Config::AddLinkHandler(BaseLink *link)
{
	m_LinkHandler.push_back(link);
}


tinyxml2::XMLElement * Config::FindServerNode(tinyxml2::XMLDocument &doc, const char *config)
{
	UINT32 dwZoneID = 0;
	UINT32 dwServerID = 0;
	const char *sep = strchr(config, '-');
	if (sep != NULL)
	{
		sscanf(config, "%d-%d", &dwZoneID, &dwServerID);
	}
	else
	{
		sscanf(config, "%d", &dwServerID);
	}

	m_ZoneID = dwZoneID;
	LogInfo("try to find server [%d - %d] in xml", dwZoneID, dwServerID);

	XMLElement *node = NULL;
	XMLElement *SettingsNode =  doc.FirstChildElement("Settings");
	if (dwZoneID == 0)
	{
		for (node = SettingsNode->FirstChildElement(m_ServerTag); node != NULL; node = node->NextSiblingElement(m_ServerTag))
		{
			if (node->IntAttribute("id") == (int)dwServerID)
			{
				return node;
			}
		}
	}
	else
	{
		for (node = SettingsNode->FirstChildElement("Zone"); node != NULL; node = node->NextSiblingElement("Zone"))
		{
			if (node->IntAttribute("id") == (int)dwZoneID)
			{
				LogInfo("scan zone [%d:%s]", node->IntAttribute("id"), node->Attribute("name"));

				XMLElement *elem = NULL;
				for (elem = node->FirstChildElement(m_ServerTag); elem != NULL; elem = elem->NextSiblingElement(m_ServerTag))
				{
					LogInfo("scan server [%d:%s]", elem->IntAttribute("id"), elem->Attribute("name"));

					if (elem->IntAttribute("id") == (int)dwServerID)
					{
						return elem;
					}
				}
				break;
			}
		}
	}

	return NULL;
}


bool Config::Init(const char *config)
{
	if(config == NULL) return false;

	CTimerMgr::CreateInstance();
	CTimerMgr::Instance()->Init();

	ReconnectManager::CreateInstance();
	ReconnectManager::Instance()->Init();

	CKeepAliveMgr::CreateInstance();
	CKeepAliveMgr::Instance()->Init();

	tinyxml2::XMLDocument doc;
	XMLError error = doc.LoadFile(config);
	if (error != XML_SUCCESS)
	{
		LogError("parser file %s get XMLError code: %d", config, error);
		return false;
	}

	XMLElement *ServerNode = doc.RootElement();
	if (!LoadServerNode(ServerNode))
	{
		LogError("Load Setting Node Failed!");
		return false;
	}

	CKeepAliveMgr::Instance()->ReadKeepAliveSetting(ServerNode);
	if (!InitNetwork())
	{
		LogError("Initialize Network failed!\n");
		return false;
	}

	m_strConfFile = config;
	return true;
}


bool Config::LoadServerNode(tinyxml2::XMLElement *ServerNode)
{
	XMLElement *node;

	m_ServerID = ServerNode->IntAttribute("id");
	m_ZoneID = ServerNode->IntAttribute("zone");

	if (m_ServerID > 65535)
	{
		LogError("ServerID out of range %u/65535", m_ServerID);
		return false;
	}

	m_ServerName = ServerNode->Attribute("name");

	LogDebug("Read server name [%s] server id [%u]", m_ServerName.c_str(), m_ServerID);

	// Listen和Connect配置
	PeerInfo peer;
	XMLElement *elem = ServerNode->FirstChildElement("Listen");
	if (elem)
	{
		for (node = elem->FirstChildElement("peer"); node != NULL; node = node->NextSiblingElement("peer"))
		{
			peer.m_action = NA_LISTEN;
			peer.m_address.m_ip = node->Attribute("ip");
			peer.m_address.m_port = node->IntAttribute("port");
			peer.m_handler = node->Attribute("handler");
			peer.m_dwSendBuffSize = node->UnsignedAttribute("sendBufSize");
			peer.m_dwRecvBuffSize = node->UnsignedAttribute("recvBufSize");
			peer.m_trigger = node->UnsignedAttribute("trigger");
			ReconnectManager::Instance()->AddPeerInfo(peer);
		}
	}

	elem = ServerNode->FirstChildElement("Connect");
	if (elem)
	{
		for (node = elem->FirstChildElement("peer"); node != NULL; node = node->NextSiblingElement("peer"))
		{
			peer.m_action = NA_CONNECT;
			peer.m_address.m_ip = node->Attribute("ip");
			peer.m_address.m_port = node->IntAttribute("port");
			peer.m_handler = node->Attribute("handler");
			peer.m_dwSendBuffSize = node->UnsignedAttribute("sendBufSize");
			peer.m_dwRecvBuffSize = node->UnsignedAttribute("recvBufSize");
			peer.m_trigger = node->UnsignedAttribute("trigger");
			ReconnectManager::Instance()->AddPeerInfo(peer);
		}
	}

	elem = ServerNode->FirstChildElement("RootPath");
	m_RootPath = elem->Attribute("dir");
	if (*m_RootPath.rbegin() != '/')
	{
		m_RootPath += '/';
	}

	elem = ServerNode->FirstChildElement("RpcTimeout");
	if(elem != NULL)
	{
		INT32 nRpcTimeoutVal = elem->IntAttribute("enabled");
		CRpc::SetTimeoutEnabled(nRpcTimeoutVal != 0);
	}

	if (!ReadSettings(ServerNode))
	{
		LogError("Read custom settings failed!");
		return false;
	}

	return true;
}

void Config::Uninit()
{
	CRpc::UnRegister();
	CProtocol::UnRegister();

	ReconnectManager::Instance()->UnInit();
	ReconnectManager::DestroyInstance();

	CKeepAliveMgr::Instance()->Uninit();
	CKeepAliveMgr::DestroyInstance();

	CNetProcessor::Instance()->UnInit();
	CNetProcessor::DestroyInstance();

	if(m_Network != NULL)
	{
		m_Network->Release();
		m_Network = NULL;
	}
	if(gPacketCoder != NULL)
	{
		delete gPacketCoder;
		gPacketCoder = NULL;
	}

	CTimerMgr::Instance()->UnInit();
	CTimerMgr::DestroyInstance();
}

// 文件读取的路径转换
// 比如配置的根目录是 ~/hello/root
// 要读的文件时 script/a.txt
// 最终路径转换为 ~/hello/root/script/a.txt
std::string Config::GetFilePath(const char *filePath)
{
	if (m_RootPath.empty())
		return filePath;

	return m_RootPath + filePath;
}

std::string Config::ReadFileContent(const char *filePath, const char *mode)
{
	std::string fullPath = GetFilePath(filePath);
	FILE *fp = fopen(fullPath.c_str(), mode);
	if (fp == NULL)
	{
		return "";
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if(fileSize == 0)
	{
		fclose(fp);
		return "";
	}

	std::string content;
	content.resize(fileSize);
	int readCount = fread(&content[0], fileSize, 1, fp);
	fclose(fp);

	if (readCount != 1)
	{
		return "";
	}

	return content;
}

bool Config::InitNetwork()
{
	CNetPacketCoder *poPacketCoder = new CNetPacketCoder();
	gPacketCoder = poPacketCoder;
	CNetProcessor::CreateInstance();
	m_Network = CreateNetWork(CNetProcessor::Instance());
	CNetProcessor::Instance()->Init(m_Network);

	if(!ReconnectManager::Instance()->InitNetwork(m_Network, gPacketCoder))
	{
		return false;
	}
	
	return true;
}


void Config::ProcessNetMessage()
{
	CTimerMgr::Instance()->Update();
	m_Network->ProcessMsg(1000);
	CRpc::Update();
	Log_Flush();
}

BaseLink * Config::FindLinkHandler(const std::string &linkName)
{
	for (auto i = m_LinkHandler.begin(); i != m_LinkHandler.end(); ++i)
	{
		BaseLink *link = *i;
		if (link->GetName() == linkName)
		{
			return *i;
		}
	}

	return NULL;
}



// 服务器子类实现这个读自己需要的内容
bool Config::ReadSettings(tinyxml2::XMLElement *element)
{
	return true;
}

Config * Config::GetConfig()
{
	return gConfig;
}

UINT32 Config::GetServerID()
{
	return m_ServerID;
}

const std::string & Config::GetServerName()
{
	return m_ServerName;
}

UINT32 Config::GetZoneID()
{
	return m_ZoneID;
}

