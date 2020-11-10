#include "pch.h"
#include "config.h"
#include "clientlink.h"
#include "gatelink.h"
#include "util.h"
#include <fstream>

INSTANCE_SINGLETON(LoginConfig)

LoginConfig::LoginConfig()
{
	SetServerTag("LoginServer");

	GateLink::CreateInstance();
	AddLinkHandler(GateLink::Instance());
	CClientLink::CreateInstance();
	AddLinkHandler(CClientLink::Instance());


	m_bDebug = false;


}

LoginConfig::~LoginConfig()
{
	CClientLink::DestroyInstance();
	GateLink::DestroyInstance();
}

bool LoginConfig::ReadSettings(tinyxml2::XMLElement *root)
{
	tinyxml2::XMLElement* elem = root->FirstChildElement("DB");
	ReadDBInfo(m_DBInfo, elem);
	elem = root->FirstChildElement("AllocGate");
	m_szAllocIp = elem->Attribute("ip");
	m_dwAllocPort = SafeConvert<UINT32>(elem->Attribute("port"));
	return true;
}

