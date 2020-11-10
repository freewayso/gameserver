#ifndef _audio_Gate_Link_H__
#define _audio_Gate_Link_H__

#include "pb/enum.pb.h"
#include "linelink.h"

struct GateServerInfo
{
	UINT32		dwZoneid;
	UINT32		dwServerid;
	UINT32		port;
	std::string ip;
	UINT32      dwPlayers;//在线玩家人数
	UINT32		dwConnector;


	GateServerInfo()
		:dwZoneid(0)
		,dwServerid(0),port(0),ip(""),dwPlayers(0),dwConnector(0)
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
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnLineConnected(UINT32 dwLine);
	virtual void OnLineClosed(UINT32 dwLine);

	bool SendToSession(const CProtocol &roPtc);
	CBR::ErrorCode AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwZoneid,std::string ip,UINT32 port);

	GateServerInfo GetServer();
	const std::map<UINT32, GateServerInfo>& GetGateInfoMap() const { return m_oGtInfoMap; }
private:
	typedef std::map<UINT32, GateServerInfo>	CGateServerInfoMap;
	CGateServerInfoMap	m_oGtInfoMap;
};

#endif
