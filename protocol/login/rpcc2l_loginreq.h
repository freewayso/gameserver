#ifndef __RPCC2L_LOGINREQ_H__
#define __RPCC2L_LOGINREQ_H__

// generate by ProtoGen at date: 2020/9/5 11:55:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCC2L_LOGINREQ_TYPE 11211

class RpcC2L_LoginReq : public CRpc
{
private:

	RpcC2L_LoginReq():CRpc(RPCC2L_LOGINREQ_TYPE)
	{
        m_dwTimeout = 20000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2L_LoginReq *CreateRpc()
	{
		return new RpcC2L_LoginReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginDataArg &roArg, const LoginDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginDataArg &roArg, LoginDataRes &roRes);
	void OnDelayReplyRpc(const LoginDataArg &roArg, LoginDataRes &roRes, const CUserData &roUserData);

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
	LoginDataArg m_oArg;
	LoginDataRes m_oRes;
};

#endif
