#include "pch.h"
#include "common/rpct2l_registergate2login.h"
#include "network/gatelink.h"

// generate by ProtoGen at date: 2020/8/8 16:13:52

RPC_SERVER_IMPLEMETION(RpcT2L_RegisterGate2Login, RegisterGateToLoginArg, RegisterGateToLoginRes)

void RpcT2L_RegisterGate2Login::OnCall(const RegisterGateToLoginArg &roArg, RegisterGateToLoginRes &roRes)
{
	//网关注册到大厅服务器
	LogInfo("gateway server reg serverid = 【%d】 zoneid=【%d】",roArg.serverid(),0);
	CBR::ErrorCode nErrCode = GateLink::Instance()->AddGateServer(m_dwConnID, roArg.serverid(), 0,roArg.ip(),roArg.port());
	roRes.set_result(nErrCode);
}

void RpcT2L_RegisterGate2Login::OnDelayReplyRpc(const RegisterGateToLoginArg &roArg, RegisterGateToLoginRes &roRes, const CUserData &roUserData)
{
}
