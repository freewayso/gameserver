#include "pch.h"
#include "login/rpcc2l_loginreq.h"
#include "network/gatelink.h"
#include "dbtask/userauth.h"
#include "config.h"

#ifdef LOCK_THREAD_PERF
	#include "login/verifymgr.h"
#else
	#include "dbtask/queryusertask.h"
	#include "login/mysqlmgr.h"
#endif
#include "timeutil.h"

#include "login/ptcl2t_loginsession2gatentf.h"
#include "session/sessionmgr.h"

// generate by ProtoGen at date: 2020/8/14 14:45:42

RPC_SERVER_IMPLEMETION(RpcC2L_LoginReq, LoginDataArg, LoginDataRes)

void RpcC2L_LoginReq::OnCall(const LoginDataArg &roArg, LoginDataRes &roRes)
{
#ifdef LOCK_THREAD_PERF
	//UserAuthTask *pUserAuthTask = new UserAuthTask(DelayRpc());
	//pUserAuthTask->SetData(m_oArg);
	//VerifyMgr::Instance()->AddDBTask(pUserAuthTask);
#else
	QueryUserTask* task = new QueryUserTask;
	task->m_szStoreKey = m_oArg.deviceid();
	task->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(task, GetUserQueryThreadIndex());
#endif

}


void RpcC2L_LoginReq::OnDelayReplyRpc(const LoginDataArg &roArg, LoginDataRes &roRes, const CUserData &roUserData)
{
	
	if (roUserData.m_pUserPtr == NULL)
	{
		roRes.set_result(CBR::ERR_FAILED);
		return;
	}else
	{
			CBR::LoginDataRes  *pAccount = (CBR::LoginDataRes*)roUserData.m_pUserPtr;
			roRes.CopyFrom(*pAccount);
			LogInfo("--------OnDelayReplyRpc----------name=[%s]",roArg.name().c_str());
			SessionManager::Instance()->SetSessionState(m_dwConnID,E_LOGIN_FIN_STATE);
			UINT64 qwSession = SessionManager::Instance()->MakeSessionID();
			roRes.set_ip(LoginConfig::Instance()->GetAllocIp());
			roRes.set_port(LoginConfig::Instance()->GetAllocPort());
			roRes.set_state(E_LOGIN_FIN_STATE);
			roRes.set_session(qwSession);
			roRes.set_result(CBR::ERR_SUCCESS);
			roRes.set_accoutid(pAccount->accoutid());
			roRes.set_accountname(pAccount->accountname());
			PtcL2T_LoginSession2GateNtf ntf;
			ntf.m_Data.set_session(qwSession);
			ntf.m_Data.set_accountname(pAccount->accountname());
			ntf.m_Data.set_accountid(pAccount->accoutid());
			ntf.m_Data.set_time(TimeUtil::GetTime());
			GateServerInfo info = GateLink::Instance()->GetServer();
			GateLink::Instance()->SendToLine(info.dwServerid,ntf);
	}

}
