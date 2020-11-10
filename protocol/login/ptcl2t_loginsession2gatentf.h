#ifndef __PTCL2T_LOGINSESSION2GATENTF_H__
#define __PTCL2T_LOGINSESSION2GATENTF_H__

// generate by ProtoGen at date: 2020/8/17 11:52:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCL2T_LOGINSESSION2GATENTF_TYPE 56089

class PtcL2T_LoginSession2GateNtf : public CProtocol
{
public:
    explicit PtcL2T_LoginSession2GateNtf() : CProtocol(PTCL2T_LOGINSESSION2GATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcL2T_LoginSession2GateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	CBR::LoginSession2Gate m_Data;
};

#endif