#include "pch.h"
#include "game/rpct2h_getactors.h"

// generate by ProtoGen at date: 2020/8/20 11:10:02

RPC_CLIENT_IMPLEMETION(RpcT2H_GetActors, ActorDataArg, ActorDataRes)

void RpcT2H_GetActors::OnReply(const ActorDataArg &roArg, const ActorDataRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	oUser.m_dwUserData = 0;
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
}

void RpcT2H_GetActors::OnTimeout(const ActorDataArg &roArg, const CUserData &roUserData)
{
}
