#ifndef __RPCC2L_REGACCOUNT_H__
#define __RPCC2L_REGACCOUNT_H__

// generate by ProtoGen at date: 2020/8/13 17:01:16

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCC2L_REGACCOUNT_TYPE 30779

class RpcC2L_RegAccount : public CRpc
{
private:

	RpcC2L_RegAccount():CRpc(RPCC2L_REGACCOUNT_TYPE)
	{
        m_dwTimeout = 10000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2L_RegAccount *CreateRpc()
	{
		return new RpcC2L_RegAccount;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegAccountArg &roArg, const RegAccountRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegAccountArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegAccountArg &roArg, RegAccountRes &roRes);
	void OnDelayReplyRpc(const RegAccountArg &roArg, RegAccountRes &roRes, const CUserData &roUserData);

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
	RegAccountArg m_oArg;
	RegAccountRes m_oRes;
};

#endif
