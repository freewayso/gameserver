#ifndef __PTCH2C_SENDITEMPTF_H__
#define __PTCH2C_SENDITEMPTF_H__

// generate by ProtoGen at date: 2020/8/25 15:46:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCH2C_SENDITEMPTF_TYPE 27035

class PtcH2C_SendItemPtf : public CProtocol
{
public:
    explicit PtcH2C_SendItemPtf() : CProtocol(PTCH2C_SENDITEMPTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcH2C_SendItemPtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::npcdata m_Data;
};

#endif