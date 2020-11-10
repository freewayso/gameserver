#include "pch.h"
#include "config.h"
#include "network/gatelink.h"
#include "network/dblink.h"
#include "network/mdclink.h"

INSTANCE_SINGLETON(HallConfig)

HallConfig::HallConfig()
{
	SetServerTag("HallServer");
	GateLink::CreateInstance();
	AddLinkHandler(GateLink::Instance());
	DBLink::CreateInstance();
	AddLinkHandler(DBLink::Instance());
	MdcLink::CreateInstance();
	AddLinkHandler(MdcLink::Instance());

	m_bDebug = false;
}

HallConfig::~HallConfig()
{
	MdcLink::DestroyInstance();
	GateLink::DestroyInstance();
	DBLink::DestroyInstance();
}

bool HallConfig::ReadSettings(tinyxml2::XMLElement *element)
{
	tinyxml2::XMLElement *elem = NULL;
	elem = element->FirstChildElement("Redis");
	if (elem)
	{
		m_RedisInfo.m_ip = elem->Attribute("ip");
		if (elem->Attribute("port") != NULL)
		{
			m_RedisInfo.m_port = elem->Attribute("port");
		}
		else
		{
			m_RedisInfo.m_port = "6379";
		}

		if (elem->Attribute("password") != NULL)
		{
			m_RedisInfo.m_password = elem->Attribute("password");
		}
		else
		{
			m_RedisInfo.m_password = "123456";
		}

		if (elem->Attribute("threads") != NULL)
		{
			m_RedisInfo.m_threads =  convert<UINT32>(elem->Attribute("threads"));
		}
		else
		{
			m_RedisInfo.m_threads = 3;
		}

		if (elem->Attribute("keysuffix") != NULL)
		{
			m_keySuffix = elem->Attribute("keysuffix");
		}
	}


	return true;
}

