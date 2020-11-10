#ifndef __MDCLINK__
#define __MDCLINK__
 
class CRpc;
class CProtocol;

#include "baselink.h"


class MdcLink: public BaseLink
{
	MdcLink();
	~MdcLink();
	DECLARE_SINGLETON(MdcLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	void RemoveConnId(UINT32 connId);
	virtual void OnClose(UINT32 dwConnID, int err);
	bool IsConnected();
	virtual const std::string &GetName();
	void AddServer(UINT32 dwConnID);
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(CRpc& roRpc);
private:
	std::vector<UINT32> m_connIds;
};



#endif