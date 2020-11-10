#ifndef __RPCH2M_REGISTERHALLTODCM_H__
#define __RPCH2M_REGISTERHALLTODCM_H__

// generate by ProtoGen at date: 2020/9/14 20:54:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2M_REGISTERHALLTODCM_TYPE 1683

class RpcH2M_RegisterHallToDcm : public CRpc
{
private:

	RpcH2M_RegisterHallToDcm():CRpc(RPCH2M_REGISTERHALLTODCM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2M_RegisterHallToDcm *CreateRpc()
	{
		return new RpcH2M_RegisterHallToDcm;
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
