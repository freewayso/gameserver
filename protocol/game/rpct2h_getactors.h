#ifndef __RPCT2H_GETACTORS_H__
#define __RPCT2H_GETACTORS_H__

// generate by ProtoGen at date: 2020/8/20 11:10:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCT2H_GETACTORS_TYPE 11146

class RpcT2H_GetActors : public CRpc
{
private:

	RpcT2H_GetActors():CRpc(RPCT2H_GETACTORS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2H_GetActors *CreateRpc()
	{
		return new RpcT2H_GetActors;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ActorDataArg &roArg, const ActorDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ActorDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ActorDataArg &roArg, ActorDataRes &roRes);
	void OnDelayReplyRpc(const ActorDataArg &roArg, ActorDataRes &roRes, const CUserData &roUserData);

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
	ActorDataArg m_oArg;
	ActorDataRes m_oRes;
};

#endif
