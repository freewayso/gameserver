#ifndef __PTCC2T_CREATENPC_H__
#define __PTCC2T_CREATENPC_H__

// generate by ProtoGen at date: 2020/8/4 14:27:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2T_CREATENPC_TYPE 61586

class PtcC2T_createnpc : public CProtocol
{
public:
    explicit PtcC2T_createnpc() : CProtocol(PTCC2T_CREATENPC_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2T_createnpc()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::npcdata m_Data;
};

#endif