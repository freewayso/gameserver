#ifndef __RPCH2D_REGISTERHALLTODB_H__
#define __RPCH2D_REGISTERHALLTODB_H__

// generate by ProtoGen at date: 2020/8/5 20:56:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2D_REGISTERHALLTODB_TYPE 10080

class RpcH2D_RegisterHallToDb : public CRpc
{
private:

	RpcH2D_RegisterHallToDb():CRpc(RPCH2D_REGISTERHALLTODB_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2D_RegisterHallToDb *CreateRpc()
	{
		return new RpcH2D_RegisterHallToDb;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterHallToDbReq &roArg, const RegisterHallToDbRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterHallToDbReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterHallToDbReq &roArg, RegisterHallToDbRes &roRes);
	void OnDelayReplyRpc(const RegisterHallToDbReq &roArg, RegisterHallToDbRes &roRes, const CUserData &roUserData);

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
	RegisterHallToDbReq m_oArg;
	RegisterHallToDbRes m_oRes;
};

#endif
