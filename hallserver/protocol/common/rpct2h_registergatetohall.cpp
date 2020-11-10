#include "pch.h"
#include "common/rpct2h_registergatetohall.h"
#include "gatelink.h"

// generate by ProtoGen at date: 2020/8/5 11:37:00

RPC_SERVER_IMPLEMETION(RpcT2H_RegisterGateToHall, RegisterGateToHallReq, RegisterGateToHallRes)

void RpcT2H_RegisterGateToHall::OnCall(const RegisterGateToHallReq &roArg, RegisterGateToHallRes &roRes)
{
	//网关注册到大厅服务器
	LogInfo("gateway server reg serverid = 【%d】 zoneid=【%d】",roArg.zoneid(),roArg.serverid());
	CBR::ErrorCode nErrCode = GateLink::Instance()->AddGateServer(m_dwConnID, roArg.serverid(), roArg.zoneid());
	roRes.set_result(nErrCode);

}

void RpcT2H_RegisterGateToHall::OnDelayReplyRpc(const RegisterGateToHallReq &roArg, RegisterGateToHallRes &roRes, const CUserData &roUserData)
{

}
