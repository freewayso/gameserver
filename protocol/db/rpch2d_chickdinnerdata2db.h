#ifndef __RPCH2D_CHICKDINNERDATA2DB_H__
#define __RPCH2D_CHICKDINNERDATA2DB_H__

// generate by ProtoGen at date: 2020/8/7 15:10:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace CBR;

#define RPCH2D_CHICKDINNERDATA2DB_TYPE 39646

class RpcH2D_ChickDinnerData2DB : public CRpc
{
private:

	RpcH2D_ChickDinnerData2DB():CRpc(RPCH2D_CHICKDINNERDATA2DB_TYPE)
	{
        m_dwTimeout = 50000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2D_ChickDinnerData2DB *CreateRpc()
	{
		return new RpcH2D_ChickDinnerData2DB;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChickDinnerSaveDataArg &roArg, const ChickDinnerSaveDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChickDinnerSaveDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChickDinnerSaveDataArg &roArg, ChickDinnerSaveDataRes &roRes);
	void OnDelayReplyRpc(const ChickDinnerSaveDataArg &roArg, ChickDinnerSaveDataRes &roRes, const CUserData &roUserData);

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
	ChickDinnerSaveDataArg m_oArg;
	ChickDinnerSaveDataRes m_oRes;
};

#endif
