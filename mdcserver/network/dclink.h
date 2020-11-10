#ifndef __DC_LINK_H__
#define __DC_LINK_H__

#include "linelink.h"
#include "pb/enum.pb.h"


struct DCInfo
{
	UINT32 dwLineId;
	std::string szIp;
	UINT32 dwPort;
};


class DCLink:public ListenLink
{

	DCLink();
	virtual ~DCLink();
	DECLARE_SINGLETON(DCLink)
public:
	virtual const std::string & GetName();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	bool SendToSession(const CProtocol &roPtc);
	CBR::ErrorCode AddDCServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwZoneid);
	const std::map<UINT32, DCInfo>& GetDcInfoMap() const { return m_oDmInfoMap; }
private:
	typedef std::map<UINT32, DCInfo>	CGateServerInfoMap;
	CGateServerInfoMap	m_oDmInfoMap;

};





#endif