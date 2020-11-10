#ifndef __GLOGIN_LINK_H__
#define __GLOGIN_LINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;


class LoginLink: public BaseLink
{
	LoginLink();
	~LoginLink();
	DECLARE_SINGLETON(LoginLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	void RemoveConnId(UINT32 connId);
	virtual void OnClose(UINT32 dwConnID, int err);
	bool IsConnected();
	virtual const std::string &GetName();
	void AddDBServer(UINT32 dwConnID);
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(CRpc& roRpc);

private:
	std::vector<UINT32> m_connIds;

};

#endif
