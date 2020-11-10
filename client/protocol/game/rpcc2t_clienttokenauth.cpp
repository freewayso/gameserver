#include "pch.h"
#include "game/rpcc2t_clienttokenauth.h"
#include "session/usermgr.h"
#include "game/rpcc2t_createactorcli2gate.h"
#include "robot/robotmgr.h"
#include "network/clientlink.h"
#include "game/rpcc2t_entergametest.h"
// generate by ProtoGen at date: 2020/9/9 19:17:44

RPC_CLIENT_IMPLEMETION(RpcC2T_ClientTokenAuth, ClientAuthArg, ClientAuthRes)

void RpcC2T_ClientTokenAuth::OnReply(const ClientAuthArg &roArg, const ClientAuthRes &roRes, const CUserData &roUserData)
{
	AccountSession *pSession = SessionMgr::Instance()->GetUserSession(roArg.userid());
	if (pSession)
	{
		if (pSession->qwActorId == 0)
		{
			char *pszActor =  RobotMgr::Instance()->BuildActorName();
			RpcC2T_CreateActorCli2Gate* pRpc = RpcC2T_CreateActorCli2Gate::CreateRpc();
			pRpc->m_oArg.set_accountid(pSession->qwAccountId);
			pRpc->m_oArg.set_name(pszActor);
			pRpc->m_oArg.set_sex(1);
			pRpc->m_oArg.set_faceid(10);
			pRpc->m_oArg.set_seqindex(2);
			pRpc->m_oArg.set_actortype(10);
			ClientLink* pClient = RobotMgr::Instance()->getClientLink(m_dwConnID);
			if (pClient)
			{
				pClient->Send(m_dwConnID, *pRpc);
			}else
			{
				LogError("client=[%u] link is lost",m_dwConnID);
			}
	
		}else
		{
			LogInfo("connect to gate server ok! and enter hall, send client login accountid=%llu token=%llu",pSession->qwAccountId,pSession->qwToken);
			RpcC2T_EnterGameTest *pRpc = RpcC2T_EnterGameTest::CreateRpc();
			pRpc->m_oArg.set_accountid(pSession->qwAccountId);
			pRpc->m_oArg.set_token(pSession->qwToken);
			pRpc->m_oArg.set_actorid(pSession->qwActorId);
			ClientLink* pClient = RobotMgr::Instance()->getClientLink(m_dwConnID);
			if (pClient)
			{
				pClient->Send(m_dwConnID, *pRpc);
			}else
			{
				LogError("client=[%u] link is lost",m_dwConnID);
			}
		}
	}
}

void RpcC2T_ClientTokenAuth::OnTimeout(const ClientAuthArg &roArg, const CUserData &roUserData)
{
}
