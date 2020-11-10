#include "pch.h"
#include "common/rpct2l_registergate2login.h"

// generate by ProtoGen at date: 2020/8/8 16:13:52

RPC_CLIENT_IMPLEMETION(RpcT2L_RegisterGate2Login, RegisterGateToLoginArg, RegisterGateToLoginRes)

void RpcT2L_RegisterGate2Login::OnReply(const RegisterGateToLoginArg &roArg, const RegisterGateToLoginRes &roRes, const CUserData &roUserData)
{
}

void RpcT2L_RegisterGate2Login::OnTimeout(const RegisterGateToLoginArg &roArg, const CUserData &roUserData)
{
}
