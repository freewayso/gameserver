#ifndef __RPCH2D_UPROLEDATA_H__
#define __RPCH2D_UPROLEDATA_H__

// generate by ProtoGen at date: 2020/8/6 11:54:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2D_UPROLEDATA_TYPE 52868

class RpcH2D_UpRoleData : public CRpc
{
private:

	RpcH2D_UpRoleData():CRpc(RPCH2D_UPROLEDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2D_UpRoleData *CreateRpc()
	{
		return new RpcH2D_UpRoleData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpRoleDataReq &roArg, const UpRoleDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpRoleDataReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpRoleDataReq &roArg, UpRoleDataRes &roRes);
	void OnDelayReplyRpc(const UpRoleDataReq &roArg, UpRoleDataRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	UpRoleDataReq m_oArg;
	UpRoleDataRes m_oRes;
};

#endif
