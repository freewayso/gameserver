#include "pch.h"
#include "game/rpch2d_queryactordb.h"
#include "LogInit.h"

// generate by ProtoGen at date: 2020/8/22 16:20:49

RPC_CLIENT_IMPLEMETION(RpcH2D_QueryActorDb, ActorDataArg, ActorDataRes)

void RpcH2D_QueryActorDb::OnReply(const ActorDataArg &roArg, const ActorDataRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	oUser.m_dwUserData = 0;
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
	//加入到大厅里面

}

void RpcH2D_QueryActorDb::OnTimeout(const ActorDataArg &roArg, const CUserData &roUserData)
{
}
