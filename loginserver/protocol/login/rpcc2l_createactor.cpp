#include "pch.h"
#include "login/rpcc2l_createactor.h"
#include "session/sessionmgr.h"
#include "util/XCommon.h"
#include "dbtask/createactortask.h"
#include "login/verifymgr.h"
#include "network/gatelink.h"
#include "dbtask/userauth.h"
#include "login/ptcl2t_loginsession2gatentf.h"
// generate by ProtoGen at date: 2020/8/21 11:09:13

RPC_SERVER_IMPLEMETION(RpcC2L_CreateActor, CreateActorReq, CreateActorRes)

void RpcC2L_CreateActor::OnCall(const CreateActorReq &roArg, CreateActorRes &roRes)
{
	//if (SessionManager::Instance()->GetSessionState(m_dwConnID)==E_INVALID_STATE)
	//{
	//	roRes.set_result(CBR::ERR_SESSION_INVALID);
	//	return;
	//}
	//CBR::ErrorCode code = (CBR::ErrorCode)XCommon::ValidNameCheck(roArg.name());
	//if (code !=CBR::ERR_SUCCESS)
	//{
	//	roRes.set_result(code);
	//	return;
	//}
	//CreateActorTask* pTask = new CreateActorTask(DelayRpc());
	//pTask->SetData(m_oArg);
	//VerifyMgr::Instance()->AddDBTask(pTask);
}

void RpcC2L_CreateActor::OnDelayReplyRpc(const CreateActorReq &roArg, CreateActorRes &roRes, const CUserData &roUserData)
{
	//LogInfo("--------RpcC2L_CreateActor----OnDelayReplyRpc------%s",roArg.name().c_str());
	//if (roUserData.m_pUserPtr == NULL)
	//{
	//	roRes.set_result(CBR::ERR_PASSWARD_ERROR);
	//	return;
	//}else
	//{
	//	GateServerInfo info = GateLink::Instance()->GetServer();
	//	roRes.set_ip(info.ip);
	//	roRes.set_port(info.port);
	//	UINT64 session = SessionManager::Instance()->MakeSessionID();
	//	roRes.set_session(session);
	//	roRes.set_result(CBR::ERR_SUCCESS);
	//	roRes.set_accoutid( roArg.accountid());
	//	PtcL2T_LoginSession2GateNtf ntf;
	//	ntf.m_Data.set_session(session);
	//	ntf.m_Data.set_time(TimeUtil::GetTime());
	//	GateLink::Instance()->SendToLine(info.dwServerid,ntf);
	//	SessionManager::Instance()->SetSessionState(m_dwConnID,E_LOGIN_FIN_STATE);
	//	roRes.set_result(ERR_SUCCESS);
	//}
}
