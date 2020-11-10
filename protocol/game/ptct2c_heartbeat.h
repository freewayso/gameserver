#ifndef __PTCT2C_HEARTBEAT_H__
#define __PTCT2C_HEARTBEAT_H__

// generate by ProtoGen at date: 2020/9/11 18:13:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2C_HEARTBEAT_TYPE 65080

class PtcT2C_Heartbeat : public CProtocol
{
public:
    explicit PtcT2C_Heartbeat() : CProtocol(PTCT2C_HEARTBEAT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2C_Heartbeat()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::HeartbeatData m_Data;
};

#endif