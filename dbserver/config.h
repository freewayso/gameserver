#ifndef __DBCONFIG_H__
#define __DBCONFIG_H__

#include "baseconfig.h"
#include "singleton.h"
#include "pb/enum.pb.h"
#include "DBInfo.h"


struct RedisInfo {
	std::string m_ip;
	std::string m_port;
	std::string m_password;
	UINT32 m_threads;
};

class DBConfig : public Config
{
	DBConfig();
	~DBConfig();
	DECLARE_SINGLETON(DBConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	const DBInfo& GetDBInfo() { return m_DBInfo; }
	const RedisInfo& GetRedisInfo() { return m_RedisInfo; }
	std::string& GetKeySuffix(){return m_keySuffix;}
	bool AutoCreateDB() const { return m_bAutoCreateDB; }
	UINT32 GetReconTryInterval() const { return m_dwTryInterval; }
	UINT32 GetReconMaxTryTimes() const { return m_dwMaxTryTimes; }
	UINT32 GetCacheGCThreshold() const { return m_dwThreshold; }

private:
	DBInfo m_DBInfo;
	RedisInfo m_RedisInfo;
	std::string m_keySuffix;
	bool m_bAutoCreateDB;
	UINT32				m_dwTryInterval;
	UINT32				m_dwMaxTryTimes;
	UINT32				m_dwThreshold;

};

#endif