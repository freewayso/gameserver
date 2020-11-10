#ifndef __HALLCONFIG_H__
#define __HALLCONFIG_H__

#include "baseconfig.h"
#include "singleton.h"


struct RedisInfo {
	std::string m_ip;
	std::string m_port;
	std::string m_password;
	UINT32 m_threads;
};


class HallConfig : public Config
{
public:
	HallConfig();
	~HallConfig();
	DECLARE_SINGLETON(HallConfig)
public:
	const RedisInfo& GetRedisInfo() { return m_RedisInfo; }
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	bool GetDebugEnv() { return  m_bDebug; }
	std::string& GetKeySuffix(){return m_keySuffix;}
private:
	RedisInfo m_RedisInfo;
	bool  m_bDebug;
	std::string m_keySuffix;
};

#endif