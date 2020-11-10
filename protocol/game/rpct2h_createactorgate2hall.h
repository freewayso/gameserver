#ifndef __RPCT2H_CREATEACTORGATE2HALL_H__
#define __RPCT2H_CREATEACTORGATE2HALL_H__

// generate by ProtoGen at date: 2020/9/8 10:36:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCT2H_CREATEACTORGATE2HALL_TYPE 19439

class RpcT2H_CreateActorGate2Hall : public CRpc
{
private:

	RpcT2H_CreateActorGate2Hall():CRpc(RPCT2H_CREATEACTORGATE2HALL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2H_CreateActorGate2Hall *CreateRpc()
	{
		return new RpcT2H_CreateActorGate2Hall;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateActorReq &roArg, const CreateActorRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateActorReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateActorReq &roArg, CreateActorRes &roRes);
	void OnDelayReplyRpc(const CreateActorReq &roArg, CreateActorRes &roRes, const CUserData &roUserData);

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
	CreateActorReq m_oArg;
	CreateActorRes m_oRes;
};

#endif
