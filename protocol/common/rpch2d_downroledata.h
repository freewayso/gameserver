#ifndef __RPCH2D_DOWNROLEDATA_H__
#define __RPCH2D_DOWNROLEDATA_H__

// generate by ProtoGen at date: 2020/8/6 14:19:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2D_DOWNROLEDATA_TYPE 48480

class RpcH2D_DownRoleData : public CRpc
{
private:

	RpcH2D_DownRoleData():CRpc(RPCH2D_DOWNROLEDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2D_DownRoleData *CreateRpc()
	{
		return new RpcH2D_DownRoleData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DownRoleDataReq &roArg, const DownRoleDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DownRoleDataReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DownRoleDataReq &roArg, DownRoleDataRes &roRes);
	void OnDelayReplyRpc(const DownRoleDataReq &roArg, DownRoleDataRes &roRes, const CUserData &roUserData);

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
	DownRoleDataReq m_oArg;
	DownRoleDataRes m_oRes;
};

#endif
