#ifndef __RPCC2H_STARTMATCH_H__
#define __RPCC2H_STARTMATCH_H__

// generate by ProtoGen at date: 2020/9/15 10:08:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCC2H_STARTMATCH_TYPE 62459

class RpcC2H_StartMatch : public CRpc
{
private:

	RpcC2H_StartMatch():CRpc(RPCC2H_STARTMATCH_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2H_StartMatch *CreateRpc()
	{
		return new RpcC2H_StartMatch;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const StartMatchArg &roArg, const StartMatchRes &roRes, const CUserData &roUserData);
	void OnTimeout(const StartMatchArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const StartMatchArg &roArg, StartMatchRes &roRes);
	void OnDelayReplyRpc(const StartMatchArg &roArg, StartMatchRes &roRes, const CUserData &roUserData);

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
	StartMatchArg m_oArg;
	StartMatchRes m_oRes;
};

#endif
