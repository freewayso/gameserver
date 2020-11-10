#include "pch.h"
#include "game/rpcc2h_startmatch.h"

// generate by ProtoGen at date: 2020/9/15 10:08:33

RPC_CLIENT_IMPLEMETION(RpcC2H_StartMatch, StartMatchArg, StartMatchRes)

void RpcC2H_StartMatch::OnReply(const StartMatchArg &roArg, const StartMatchRes &roRes, const CUserData &roUserData)
{
}

void RpcC2H_StartMatch::OnTimeout(const StartMatchArg &roArg, const CUserData &roUserData)
{
}
