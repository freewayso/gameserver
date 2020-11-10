#ifndef __RPCH2M_REGHALL2MDC_H__
#define __RPCH2M_REGHALL2MDC_H__

// generate by ProtoGen at date: 2020/9/14 21:14:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2M_REGHALL2MDC_TYPE 12160

class RpcH2M_RegHall2Mdc : public CRpc
{
private:

	RpcH2M_RegHall2Mdc():CRpc(RPCH2M_REGHALL2MDC_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2M_RegHall2Mdc *CreateRpc()
	{
		return new RpcH2M_RegHall2Mdc;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterHallToDcmArg &roArg, const RegisterHallToDcmRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterHallToDcmArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterHallToDcmArg &roArg, RegisterHallToDcmRes &roRes);
	void OnDelayReplyRpc(const RegisterHallToDcmArg &roArg, RegisterHallToDcmRes &roRes, const CUserData &roUserData);

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
	RegisterHallToDcmArg m_oArg;
	RegisterHallToDcmRes m_oRes;
};

#endif
