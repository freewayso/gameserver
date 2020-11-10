#ifndef __PTCT2H_BUYITEMPTF_H__
#define __PTCT2H_BUYITEMPTF_H__

// generate by ProtoGen at date: 2020/8/25 15:45:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2H_BUYITEMPTF_TYPE 981

class PtcT2H_BuyItemPtf : public CProtocol
{
public:
    explicit PtcT2H_BuyItemPtf() : CProtocol(PTCT2H_BUYITEMPTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2H_BuyItemPtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::npcdata m_Data;
};

#endif