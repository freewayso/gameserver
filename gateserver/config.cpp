#include "pch.h"
#include "config.h"
#include "clientlink.h"
#include "halllink.h"
#include "loginlink.h"
#include "util.h"

INSTANCE_SINGLETON(GateConfig)

GateConfig::GateConfig()
{
	LoginLink::CreateInstance();
	AddLinkHandler(LoginLink::Instance());


	HallLink::CreateInstance();
	AddLinkHandler(HallLink::Instance());

	CClientLink::CreateInstance();
	AddLinkHandler(CClientLink::Instance());
	m_nReconnectTimeout = 300;

	SetServerTag("GateServer");

	m_dwLine = 0;

	m_bCompress = false;
	m_bEncrypt = false;
	m_dwMinCompressSize = 500;

	m_newConnPerSecond = 1000;
	m_tpsPerConn = 100;
	m_maxConnection = 20000;
}

GateConfig::~GateConfig()
{
    LoginLink::DestroyInstance();
	CClientLink::DestroyInstance();
	HallLink::DestroyInstance();
}

bool GateConfig::ReadSettings(tinyxml2::XMLElement *element)
{
	const char* pcLine = element->Attribute("line");
	if(pcLine == NULL)
	{
		LogError("can not find 'line' config");
		//return false;
	}
	m_dwLine = convert<UINT32>(pcLine);

	tinyxml2::XMLElement *elem = element->FirstChildElement("Reconnect");
	if(elem != NULL)
	{
		m_nReconnectTimeout = elem->IntAttribute("timeout");
	}

	elem = element->FirstChildElement("ConnectLimit");
	if (elem)
	{
		m_newConnPerSecond = elem->UnsignedAttribute("nps");
		m_tpsPerConn = elem->UnsignedAttribute("tps");
		m_maxConnection = elem->UnsignedAttribute("maxConnection");
		LogInfo("nps: %u, tps: %u, maxconn: %u", m_newConnPerSecond, m_tpsPerConn, m_maxConnection);
	}


	elem = element->FirstChildElement("Compress");
	if(elem != NULL)
	{
		INT32 nEnabled = elem->IntAttribute("enabled");
		m_bCompress = (nEnabled == 1);

		INT32 nThreshold = elem->IntAttribute("threshold");
		if(nThreshold > 0)
		{
			m_dwMinCompressSize = (UINT32)nThreshold;
		}
	}

	elem = element->FirstChildElement("Encrypt");
	if(elem != NULL)
	{
		INT32 nEnabled = elem->IntAttribute("enabled");
		m_bEncrypt = (nEnabled == 1);
	}
	return true;
}

