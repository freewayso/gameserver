#ifndef __RPCC2T_CLIENTTOKENAUTH_H__
#define __RPCC2T_CLIENTTOKENAUTH_H__

// generate by ProtoGen at date: 2020/9/9 19:17:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCC2T_CLIENTTOKENAUTH_TYPE 49697

class RpcC2T_ClientTokenAuth : public CRpc
{
private:

	RpcC2T_ClientTokenAuth():CRpc(RPCC2T_CLIENTTOKENAUTH_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2T_ClientTokenAuth *CreateRpc()
	{
		return new RpcC2T_ClientTokenAuth;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ClientAuthArg &roArg, const ClientAuthRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ClientAuthArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ClientAuthArg &roArg, ClientAuthRes &roRes);
	void OnDelayReplyRpc(const ClientAuthArg &roArg, ClientAuthRes &roRes, const CUserData &roUserData);

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
	ClientAuthArg m_oArg;
	ClientAuthRes m_oRes;
};

#endif
