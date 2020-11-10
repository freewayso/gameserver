#include "pch.h"
#include "account/rpcc2l_regaccount.h"
#include "util/XCommon.h"
#include "dbtask/userregtask.h"
#include "login/verifymgr.h"
#include "session/sessionmgr.h"




// generate by ProtoGen at date: 2020/8/12 11:19:18

RPC_SERVER_IMPLEMETION(RpcC2L_RegAccount, RegAccountArg, RegAccountRes)

void RpcC2L_RegAccount::OnCall(const RegAccountArg &roArg, RegAccountRes &roRes)
{
	/*LogInfo("--------RpcC2L_RegAccount----------%s",roArg.name().c_str());

	CBR::ErrorCode code = (CBR::ErrorCode)XCommon::ValidNameCheck(roArg.name());
	if (code !=CBR::ERR_SUCCESS)
	{
		roRes.set_error(code);
		return;
	}
	
    code = (CBR::ErrorCode)XCommon::ValidPassswardCheck(roArg.password());
	if (code !=CBR::ERR_SUCCESS)
	{
		roRes.set_error(code);
		return;
	}


	UINT32 dwIP;
	UINT16 wPort;
	CNetProcessor::Instance()->GetRemoteAddr(m_dwConnID, dwIP, wPort);
	char IPBuf[64] = {0};
	unsigned char *p = (unsigned char *)&dwIP;
	sprintf(IPBuf, "%u.%u.%u.%u:%d", p[0], p[1], p[2], p[3], wPort);
	m_oArg.set_ip(IPBuf);
	UserRegTask* pTask = new UserRegTask(DelayRpc());
	pTask->SetData(m_oArg);
	VerifyMgr::Instance()->AddDBTask(pTask);
*/
}

void RpcC2L_RegAccount::OnDelayReplyRpc(const RegAccountArg &roArg, RegAccountRes &roRes, const CUserData &roUserData)
{
	/*
	LogInfo("--------OnDelayReplyRpc----------%s",roArg.name().c_str());
	if (roUserData.m_pUserPtr == NULL)
	{
		roRes.set_error(CBR::ERR_NAME_INVALID);
		return;
	}else
	{
		CBR::RegAccountRes  *pInfo = (CBR::RegAccountRes*)roUserData.m_pUserPtr;
		if (pInfo)
		{
			SessionManager::Instance()->SetSessionState(m_dwConnID,E_LOGIN_STATE);
			roRes.set_error(CBR::ERR_SUCCESS);
			roRes.set_accountid(pInfo->accountid());
		}

	}
	*/
}


