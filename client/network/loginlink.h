#ifndef _RLOGIN_LINK_H
#define _RLOGIN_LINK_H

#include "baselink.h"

#pragma pack (4)
struct RClientProtocolHead
{

	UINT32 dwLen;
	UINT32 dwType;
	UINT32 dwFlag;
	SessionType dwSessionID;
};

struct RClientRpcHead
{
	UINT32 dwLen;
	UINT32 dwType;
	UINT32 dwFlag;
	SessionType dwSessionID;
	UINT32 dwTagID;

};

#pragma pack ()



struct PeerInfo;


class LoginLink:public BaseLink
{
public:
	LoginLink();
	LoginLink(PeerInfo* peer);
	~LoginLink();
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual void OnPassiveConnect(UINT32 dwConnID) { assert(false); }

	virtual const std::string &GetName();
	virtual	bool Send(UINT32 dwConnID, const CProtocol& roPtc);
	virtual bool Send(UINT32 dwConnID, CRpc &roRpc);
	void RpcNullReply(UINT32 dwConnID, UINT32 dwRpcType, UINT32 dwRpcTagID);
	virtual void GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);
	virtual void GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);
	virtual HeadStruct DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen);
	UINT32 MinSize();
	
	INT32 GetConnId()
	{
		return m_dwConnId;
	}
	void SetConnId(INT32 dwConnid)
	{
		m_dwConnId = dwConnid;
	}


private:
	PeerInfo *m_pPeerInfo; //用id 来区别

	RClientRpcHead RpcChangeHead;
	RClientProtocolHead PtcChangeHead;
	int m_dwConnId;

};







#endif
