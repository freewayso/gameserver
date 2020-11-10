#ifndef __RPCT2L_REGISTERGATE2LOGIN_H__
#define __RPCT2L_REGISTERGATE2LOGIN_H__

// generate by ProtoGen at date: 2020/8/8 16:13:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCT2L_REGISTERGATE2LOGIN_TYPE 8654

class RpcT2L_RegisterGate2Login : public CRpc
{
private:

	RpcT2L_RegisterGate2Login():CRpc(RPCT2L_REGISTERGATE2LOGIN_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2L_RegisterGate2Login *CreateRpc()
	{
		return new RpcT2L_RegisterGate2Login;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGateToLoginArg &roArg, const RegisterGateToLoginRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGateToLoginArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGateToLoginArg &roArg, RegisterGateToLoginRes &roRes);
	void OnDelayReplyRpc(const RegisterGateToLoginArg &roArg, RegisterGateToLoginRes &roRes, const CUserData &roUserData);

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
	RegisterGateToLoginArg m_oArg;
	RegisterGateToLoginRes m_oRes;
};

#endif
