#ifndef __GSCONFIG_H__
#define __GSCONFIG_H__

#include "baseconfig.h"

#include "timer.h"

class GateConfig : public Config
{
	GateConfig();
	~GateConfig();
	DECLARE_SINGLETON(GateConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);

	// µ•Œª: √Î
	int GetReconnectTimeout() { return m_nReconnectTimeout; }
	UINT32 GetLine() const { return m_dwLine; }
	bool	IsCompress() const	{ return m_bCompress; }
	bool	IsEncrypt() const	{ return m_bEncrypt; }
	UINT32	GetMinCompressSize() const { return m_dwMinCompressSize; }
	UINT32 GetNewConnPerSecond() { return m_newConnPerSecond; }
	UINT32 GetTpsPerConn() { return m_tpsPerConn; }
	UINT32 GetMaxConnNum() { return m_maxConnection; }
private:
	int			m_nReconnectTimeout;
	UINT32		m_dwLine;

	bool		m_bCompress;
	bool		m_bEncrypt;
	UINT32		m_dwMinCompressSize;
	UINT32		m_newConnPerSecond;
	UINT32		m_tpsPerConn;
	UINT32		m_maxConnection;
};

#endif