#ifndef _Gate_Link_H__
#define _Gate_Link_H__

#include "pb/enum.pb.h"
#include "linelink.h"

struct GateServerInfo
{
	UINT32		dwZoneid;
	UINT32		dwServerid;

	GateServerInfo()
		:dwZoneid(0)
		,dwServerid(0)
	{
	}
};

class GateLink : public LineLink
{
	GateLink();
	virtual ~GateLink();
	DECLARE_SINGLETON(GateLink)

public:
	virtual const std::string & GetName();

	virtual void OnLineConnected(UINT32 dwLine);
	virtual void OnLineClosed(UINT32 dwLine);

	bool SendToSession(const CProtocol &roPtc);
	CBR::ErrorCode AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwZoneid);

	const std::map<UINT32, GateServerInfo>& GetGateInfoMap() const { return m_oGtInfoMap; }
private:
	typedef std::map<UINT32, GateServerInfo>	CGateServerInfoMap;
	CGateServerInfoMap	m_oGtInfoMap;
};

#endif
