#ifndef __PTCC2C_HEARTBEAT_H__
#define __PTCC2C_HEARTBEAT_H__

// generate by ProtoGen at date: 2020/9/11 16:46:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2C_HEARTBEAT_TYPE 8235

class PtcC2C_Heartbeat : public CProtocol
{
public:
    explicit PtcC2C_Heartbeat() : CProtocol(PTCC2C_HEARTBEAT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2C_Heartbeat()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::HeartbeatData m_Data;
};

#endif