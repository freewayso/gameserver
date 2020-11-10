#ifndef __HALL_LINK_H__
#define __HALL_LINK_H__

#include "linelink.h"
#include "pb/enum.pb.h"


struct HallInfo
{
	UINT32 dwConnId;
	std::string szIp;
	HallInfo()
	{
		szIp = "";
		dwConnId = 0;
	}
};


class HallLink:public ListenLink
{

	HallLink();
	virtual ~HallLink();
	DECLARE_SINGLETON(HallLink)
public:
	virtual const std::string & GetName();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	bool SendToSession(const CProtocol &roPtc);
	CBR::ErrorCode AddHallServer(UINT32 dwConnID, UINT32 dwServerID);
	const std::map<UINT32, HallInfo>& GetDcInfoMap() const { return m_oDmInfoMap; }
private:
	typedef std::map<UINT32, HallInfo>	HallServerInfoMap;
	HallServerInfoMap	m_oDmInfoMap;

};



#endif