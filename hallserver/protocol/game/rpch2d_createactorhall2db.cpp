#include "pch.h"
#include "game/rpch2d_createactorhall2db.h"

// generate by ProtoGen at date: 2020/9/8 14:07:25

RPC_CLIENT_IMPLEMETION(RpcH2D_CreateActorHall2Db, CreateActorReq, CreateActorRes)

void RpcH2D_CreateActorHall2Db::OnReply(const CreateActorReq &roArg, const CreateActorRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	oUser.m_dwUserData = 0;
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
}

void RpcH2D_CreateActorHall2Db::OnTimeout(const CreateActorReq &roArg, const CUserData &roUserData)
{
}
