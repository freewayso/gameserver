#include "pch.h"
#include "account/rpcc2l_regaccount.h"
#include "util/XCommon.h"
#include "dbtask/userregtask.h"
#include "login/verifymgr.h"


// generate by ProtoGen at date: 2020/8/12 11:19:18

RPC_SERVER_IMPLEMETION(RpcC2L_RegAccount, RegAccountArg, RegAccountRes)

void RpcC2L_RegAccount::OnCall(const RegAccountArg &roArg, RegAccountRes &roRes)
{
	CBR::ErrorCode ret = (CBR::ErrorCode)XCommon::ValidNameCheck(roArg.name());
	if (ret != CBR::ERR_SUCCESS)
	{
		roRes.set_error(ret); 
		return;
	}
	int d = m_dwConnID;
	UINT32 rdwIP;
	UINT16 rwPort;
	CNetProcessor::Instance()->GetRemoteAddr(m_dwConnID, rdwIP, rwPort);
	char ip[64] = {0};
	unsigned char *p = (unsigned char *)&rdwIP;
	sprintf(ip, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
	m_oArg.set_name(ip);
	UserRegTask *poTask = new UserRegTask(DelayRpc());
	poTask->SetData(m_oArg);
	VerifyMgr::Instance()->AddDBTask(poTask);
}

void RpcC2L_RegAccount::OnDelayReplyRpc(const RegAccountArg &roArg, RegAccountRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_pUserPtr == NULL)
	{
		roRes.set_error(CBR::ERR_NAME_INVALID);
		return;
	}
	roRes.set_error(CBR::ERR_SUCCESS);
}
