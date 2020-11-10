#ifndef __RPCH2D_CHICKDINNERQUERYDATA_H__
#define __RPCH2D_CHICKDINNERQUERYDATA_H__

// generate by ProtoGen at date: 2020/8/11 10:50:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2D_CHICKDINNERQUERYDATA_TYPE 31180

class RpcH2D_ChickDinnerQueryData : public CRpc
{
private:

	RpcH2D_ChickDinnerQueryData():CRpc(RPCH2D_CHICKDINNERQUERYDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2D_ChickDinnerQueryData *CreateRpc()
	{
		return new RpcH2D_ChickDinnerQueryData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChickDinnerQueryDataArg &roArg, const ChickDinnerQueryDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChickDinnerQueryDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChickDinnerQueryDataArg &roArg, ChickDinnerQueryDataRes &roRes);
	void OnDelayReplyRpc(const ChickDinnerQueryDataArg &roArg, ChickDinnerQueryDataRes &roRes, const CUserData &roUserData);

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
	ChickDinnerQueryDataArg m_oArg;
	ChickDinnerQueryDataRes m_oRes;
};

#endif
