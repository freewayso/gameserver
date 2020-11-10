#ifndef _ClientLink_H__
#define _ClientLink_H__

#include "baselink.h"

class CProtocol;
#pragma pack (4)


struct ClientProtocolHead
{

	UINT32 dwLen;
	UINT32 dwType;
	UINT32 dwFlag;
	SessionType dwSessionID;
};

struct ClientRpcHead
{
	UINT32 dwLen;
	UINT32 dwType;
	UINT32 dwFlag;
	SessionType dwSessionID;
	UINT32 dwTagID;

};
#pragma pack ()


class CClientLink : public ListenLink //, public ForwardInterface
{
	CClientLink();
	virtual ~CClientLink();
	DECLARE_SINGLETON(CClientLink)

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string & GetName();
	virtual UINT32 MinSize();
	virtual bool IsChangeHead() { return true; }
	virtual HeadInfo GetNewHeadInfo(const ProtocolHead &head, UINT32 dataLen);
	virtual HeadInfo GetNewHeadInfo(const RpcHead &head, UINT32 dataLen);

	void Close(UINT32 dwConnID);

	bool Send(UINT32 dwConnID, const CProtocol& roPtc);
	bool Send(UINT32 dwConnID, CRpc &roRpc);

	virtual bool UseKeepAlive() const { return false; }
private:


	ClientRpcHead RpcChangeHead;
	ClientProtocolHead PtcChangeHead;
};

#endif
