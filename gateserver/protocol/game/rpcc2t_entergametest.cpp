#include "pch.h"
#include "game/rpcc2t_entergametest.h"
#include "game/rpct2h_getactors.h"
#include "session/sessionmanager.h"
#include "network/halllink.h"

// generate by ProtoGen at date: 2020/8/18 21:08:29

RPC_SERVER_IMPLEMETION(RpcC2T_EnterGameTest, EnterGameArg, EnterGameRes)

void RpcC2T_EnterGameTest::OnCall(const EnterGameArg &roArg, EnterGameRes &roRes)
{
	SessionInfo *pSession = SessionManager::Instance()->FindByConnID(m_dwConnID);
	if (pSession)
	{
		RpcT2H_GetActors *pRpc = RpcT2H_GetActors::CreateRpc();
		pRpc->m_oArg.set_rpcid(DelayRpc());
		pRpc->m_oArg.set_actorid(roArg.actorid());
		pRpc->m_oArg.set_accountid(roArg.accountid());
		pRpc->m_oArg.set_sessionid(pSession->m_sessionID);
		HallLink::Instance()->SendTo(*pRpc);
	
	}else
	{
		LogError("-----------RpcC2T_EnterGameTest-------- cannot find session [%llu] connid[%u]",roArg.token(),m_dwConnID);
	}

}

void RpcC2T_EnterGameTest::OnDelayReplyRpc(const EnterGameArg &roArg, EnterGameRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == 0 && roUserData.m_pUserPtr)
	{
		SessionManager::Instance()->SetSessionState(m_dwConnID,SESSION_INGAME);
		CBR::EnterGameRes* pInfo = (CBR::EnterGameRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);
	}

}

