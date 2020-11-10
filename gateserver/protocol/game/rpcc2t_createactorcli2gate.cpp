#include "pch.h"
#include "game/rpcc2t_createactorcli2gate.h"
#include "game/rpct2h_createactorgate2hall.h"
#include "halllink.h"
// generate by ProtoGen at date: 2020/9/8 10:35:29

RPC_SERVER_IMPLEMETION(RpcC2T_CreateActorCli2Gate, CreateActorReq, CreateActorRes)

void RpcC2T_CreateActorCli2Gate::OnCall(const CreateActorReq &roArg, CreateActorRes &roRes)
{
	//验证下seesion是否合法
	RpcT2H_CreateActorGate2Hall *pRpc = RpcT2H_CreateActorGate2Hall::CreateRpc();
	pRpc->m_oArg.CopyFrom(roArg);
	pRpc->m_oArg.set_rpcid(DelayRpc());
	HallLink::Instance()->SendTo(*pRpc);
}

void RpcC2T_CreateActorCli2Gate::OnDelayReplyRpc(const CreateActorReq &roArg, CreateActorRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == 0 && roUserData.m_pUserPtr)
	{
		CBR::CreateActorRes* pInfo = (CBR::CreateActorRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);
	}
}

