#ifndef __RPCT2H_REGISTERGATETOHALL_H__
#define __RPCT2H_REGISTERGATETOHALL_H__

// generate by ProtoGen at date: 2020/8/20 19:22:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCT2H_REGISTERGATETOHALL_TYPE 4002

class RpcT2H_RegisterGateToHall : public CRpc
{
private:

	RpcT2H_RegisterGateToHall():CRpc(RPCT2H_REGISTERGATETOHALL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2H_RegisterGateToHall *CreateRpc()
	{
		return new RpcT2H_RegisterGateToHall;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGateToHallReq &roArg, const RegisterGateToHallRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGateToHallReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGateToHallReq &roArg, RegisterGateToHallRes &roRes);
	void OnDelayReplyRpc(const RegisterGateToHallReq &roArg, RegisterGateToHallRes &roRes, const CUserData &roUserData);

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
	RegisterGateToHallReq m_oArg;
	RegisterGateToHallRes m_oRes;
};

#endif
