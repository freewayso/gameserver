﻿#ifndef __RPCC2T_CREATEACTORCLI2GATE_H__
#define __RPCC2T_CREATEACTORCLI2GATE_H__

// generate by ProtoGen at date: 2020/9/8 10:51:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCC2T_CREATEACTORCLI2GATE_TYPE 60776

class RpcC2T_CreateActorCli2Gate : public CRpc
{
private:

	RpcC2T_CreateActorCli2Gate():CRpc(RPCC2T_CREATEACTORCLI2GATE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2T_CreateActorCli2Gate *CreateRpc()
	{
		return new RpcC2T_CreateActorCli2Gate;
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
