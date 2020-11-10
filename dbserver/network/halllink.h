#ifndef __HSLINK_H__
#define __HSLINK_H__

#include "baselink.h"


class CProtocol;
class CRpc;
struct HallServerInfo
{
	UINT32 m_dwConnID;
	UINT32 m_dwServerID;
	std::string m_serverName;
}; 

enum HallRegDbServer
{
	HALLREGDBSERVER_ERR_SUCCESS = 0,
	HALLREGDBSERVER_ERR_FAILED = 1
};


class HallLink : public ListenLink
{
	HallLink();
	~HallLink();
	DECLARE_SINGLETON(HallLink)

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	UINT32 AddServerInfo(UINT32 dwConnID, UINT32 dwServerID, const std::string &serverName);
	HallServerInfo *GetServerInfo(UINT32 dwConnID);
	HallServerInfo *GetServerInfoByServerID(UINT32 dwServerID);
	void Close(UINT32 dwConnID);
	const std::list<HallServerInfo>& getGSConnections(){return m_gsConnections;}
	void ShowHSLinkInfo();

private:

	std::list<HallServerInfo> m_gsConnections;
};

#endif