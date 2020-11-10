#include "pch.h"
#include "game/rpct2h_createactorgate2hall.h"

// generate by ProtoGen at date: 2020/9/8 10:36:32

RPC_CLIENT_IMPLEMETION(RpcT2H_CreateActorGate2Hall, CreateActorReq, CreateActorRes)

void RpcT2H_CreateActorGate2Hall::OnReply(const CreateActorReq &roArg, const CreateActorRes &roRes, const CUserData &roUserData)
{
	UINT32 dwRpcID =roArg.rpcid();
	CUserData oUser;
	oUser.m_dwUserData = 0;
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(dwRpcID, oUser);
}

void RpcT2H_CreateActorGate2Hall::OnTimeout(const CreateActorReq &roArg, const CUserData &roUserData)
{

}
