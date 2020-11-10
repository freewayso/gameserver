#ifndef __PTCT2H_LOGINOUTNTF_H__
#define __PTCT2H_LOGINOUTNTF_H__

// generate by ProtoGen at date: 2020/8/25 15:43:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2H_LOGINOUTNTF_TYPE 59474

class PtcT2H_LoginOutNtf : public CProtocol
{
public:
    explicit PtcT2H_LoginOutNtf() : CProtocol(PTCT2H_LOGINOUTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2H_LoginOutNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::LoginOutData m_Data;
};

#endif