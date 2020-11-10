#ifndef __CLI_LINK_H__
#define __CLI_LINK_H__

#include "baselink.h"
#include "reconnetmgr.h"

class CRpc;
class CProtocol;

//


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

struct PeerInfo;

class ClientLink : public BaseLink
{
public:
	ClientLink();
	ClientLink(PeerInfo* peer);
	~ClientLink();

public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();
	virtual bool Send(UINT32 dwConnID, CRpc &roRpc);
	virtual bool Send(UINT32 dwConnID, const CProtocol& roPtc);

	void RpcNullReply(UINT32 dwConnID, UINT32 dwRpcType, UINT32 dwRpcTagID);
	virtual void GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);
	virtual void GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);
	virtual HeadStruct DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen);
	virtual UINT32 MinSize();

	bool IsConnected() const { return m_dwConnID != INVALID_CONN_ID; }

	INT32 GetConnId()
	{
		return m_dwConnID;
	}
	void SetConnId(INT32 dwConnid)
	{
		m_dwConnID = dwConnid;
	}

	UINT64 GetUserId()
	{
		return m_qwUserId;
	}
	void SetUserId(UINT64 qwUserId)
	{
		m_qwUserId = qwUserId;
	}

private:
	UINT32 m_dwConnID;
	ClientRpcHead RpcChangeHead;
	ClientProtocolHead PtcChangeHead;

	PeerInfo *m_pPeerInfo; //用id 来区别
	UINT64 m_qwUserId;



};

#endif