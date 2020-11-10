#include "pch.h"
#include "game/rpcc2t_createactorcli2gate.h"
#include "game/rpcc2t_entergametest.h"
#include "robot/robotmgr.h"
#include "network/clientlink.h"
#include "session/usermgr.h"

// generate by ProtoGen at date: 2020/9/8 10:35:29

RPC_CLIENT_IMPLEMETION(RpcC2T_CreateActorCli2Gate, CreateActorReq, CreateActorRes)

void RpcC2T_CreateActorCli2Gate::OnReply(const CreateActorReq &roArg, const CreateActorRes &roRes, const CUserData &roUserData)
{
	//创建角色成功，进入大厅
	LogInfo(" create actor is sucessfull send client login accountid=%llu token=%llu",roArg.accountid(),roRes.actorid());
	RpcC2T_EnterGameTest *pRpc = RpcC2T_EnterGameTest::CreateRpc();
	AccountSession *pSession =  SessionMgr::Instance()->GetConnSession(m_dwConnID);
	if (pSession)
	{
		pRpc->m_oArg.set_token(pSession->qwToken);
		pRpc->m_oArg.set_accountid(roArg.accountid());
		pRpc->m_oArg.set_actorid(roRes.actorid());
		ClientLink* pClient = RobotMgr::Instance()->getClientLink(m_dwConnID);
		if (pClient)
		{
			pClient->Send(m_dwConnID, *pRpc);
		}
	}
}

void RpcC2T_CreateActorCli2Gate::OnTimeout(const CreateActorReq &roArg, const CUserData &roUserData)
{

}