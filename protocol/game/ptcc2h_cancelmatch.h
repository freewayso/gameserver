#ifndef __PTCC2H_CANCELMATCH_H__
#define __PTCC2H_CANCELMATCH_H__

// generate by ProtoGen at date: 2020/9/15 20:40:10

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2H_CANCELMATCH_TYPE 56530

class PtcC2H_CancelMatch : public CProtocol
{
public:
    explicit PtcC2H_CancelMatch() : CProtocol(PTCC2H_CANCELMATCH_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2H_CancelMatch()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::CancelMatchArg m_Data;
};

#endif