#ifndef __RPCC2H_ENTERCBR_H__
#define __RPCC2H_ENTERCBR_H__

// generate by ProtoGen at date: 2020/8/19 16:19:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCC2H_ENTERCBR_TYPE 30026

class RpcC2H_EnterCBR : public CRpc
{
private:

	RpcC2H_EnterCBR():CRpc(RPCC2H_ENTERCBR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2H_EnterCBR *CreateRpc()
	{
		return new RpcC2H_EnterCBR;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterGameArg &roArg, const EnterGameRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterGameArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterGameArg &roArg, EnterGameRes &roRes);
	void OnDelayReplyRpc(const EnterGameArg &roArg, EnterGameRes &roRes, const CUserData &roUserData);

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
	EnterGameArg m_oArg;
	EnterGameRes m_oRes;
};

#endif
