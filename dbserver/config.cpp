#include "pch.h"
#include "config.h"
#include "network/halllink.h"
#include "pb/enum.pb.h"


INSTANCE_SINGLETON(DBConfig)

DBConfig::DBConfig()
{
	SetServerTag("dbServer");
	HallLink::CreateInstance();
	AddLinkHandler(HallLink::Instance());
}

DBConfig::~DBConfig()
{
	HallLink::DestroyInstance();

}


bool DBConfig::ReadSettings(tinyxml2::XMLElement *root)
{
	tinyxml2::XMLElement *elem = root->FirstChildElement("DB");
	if(elem == NULL)
	{
		LogError("Can't find DB element in DBServer config");
		return false;
	}

	m_DBInfo.m_ip = elem->Attribute("ip");
	m_DBInfo.m_user = elem->Attribute("user");
	m_DBInfo.m_pswd = elem->Attribute("password");
	m_DBInfo.m_database = elem->Attribute("database");
	m_DBInfo.m_threads = convert<UINT32>(elem->Attribute("threads"));

	if (elem->Attribute("port") != NULL)
	{
		m_DBInfo.m_port = elem->Attribute("port");
	}
	else
	{
		m_DBInfo.m_port = "3306";
	}

	elem = root->FirstChildElement("mysqlreconnect");
	if (elem == NULL)
	{
		LogError("Can't find reconnect element in config");
		return false;
	}

	INT32 nVal1 = 0;
	INT32 nVal2 = 0;
	elem->QueryIntAttribute("interval", &nVal1);
	if(nVal1 <= 0)
	{
		LogError("interval [%d] is invalid", nVal1);
		return false;
	}

	elem->QueryIntAttribute("maxtry", &nVal2);
	if(nVal2 <= 0)
	{
		LogError("maxtry [%d] is invalid", nVal2);
		return false;
	}


	elem = root->FirstChildElement("AutoCreateDB");
	if(elem)
	{
		INT32 nVal = 0;
		elem->QueryIntAttribute("enabled", &nVal);
		m_bAutoCreateDB = (nVal == 0 ? false : true);
	}


	elem = root->FirstChildElement("Redis");
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


	//elem = root->FirstChildElement("CacheGC");
	//if(elem->Attribute("threshold")!=NULL)
	//{
	//	m_dwThreshold =   convert<UINT32>(elem->Attribute("threshold"));
	//}else
	//{
		m_dwThreshold =  100000;
	//}


	return true;
}

