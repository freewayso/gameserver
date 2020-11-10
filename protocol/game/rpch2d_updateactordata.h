#ifndef __RPCH2D_UPDATEACTORDATA_H__
#define __RPCH2D_UPDATEACTORDATA_H__

// generate by ProtoGen at date: 2020/9/9 15:12:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2D_UPDATEACTORDATA_TYPE 32066

class RpcH2D_UpdateActorData : public CRpc
{
private:

	RpcH2D_UpdateActorData():CRpc(RPCH2D_UPDATEACTORDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2D_UpdateActorData *CreateRpc()
	{
		return new RpcH2D_UpdateActorData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpdateActorDataArg &roArg, const UpdateActorDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpdateActorDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpdateActorDataArg &roArg, UpdateActorDataRes &roRes);
	void OnDelayReplyRpc(const UpdateActorDataArg &roArg, UpdateActorDataRes &roRes, const CUserData &roUserData);

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
	UpdateActorDataArg m_oArg;
	UpdateActorDataRes m_oRes;
};

#endif
