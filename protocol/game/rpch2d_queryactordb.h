#ifndef __RPCH2D_QUERYACTORDB_H__
#define __RPCH2D_QUERYACTORDB_H__

// generate by ProtoGen at date: 2020/8/22 16:20:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2D_QUERYACTORDB_TYPE 38460

class RpcH2D_QueryActorDb : public CRpc
{
private:

	RpcH2D_QueryActorDb():CRpc(RPCH2D_QUERYACTORDB_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2D_QueryActorDb *CreateRpc()
	{
		return new RpcH2D_QueryActorDb;
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
