#ifndef __LOGCONFIG_H__
#define __LOGCONFIG_H__

#include "baseconfig.h"
#include "singleton.h"
#include "DBInfo.h"
#include "pb/enum.pb.h"

class LoginConfig : public Config
{
	LoginConfig();
	~LoginConfig();
	DECLARE_SINGLETON(LoginConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	const DBInfo &GetDBInfo() { return m_DBInfo; }
	std::string& GetDB() { return m_DBInfo.m_database; }
	bool IsDebug() const { return m_bDebug; }
	void SetDebug(bool isDebug) { m_bDebug = isDebug; }
	std::string GetAllocIp(){ return m_szAllocIp;}
	UINT32 GetAllocPort(){return m_dwAllocPort;}
private:
	std::string m_szAllocIp;
	UINT32 m_dwAllocPort;

	DBInfo	m_DBInfo;
	bool	m_bDebug;

};

#endif