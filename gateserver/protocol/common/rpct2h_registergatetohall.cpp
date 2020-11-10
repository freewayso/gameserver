#include "pch.h"
#include "common/rpct2h_registergatetohall.h"
#include "halllink.h"
#include "LogInit.h"

// generate by ProtoGen at date: 2020/8/5 11:37:00

RPC_CLIENT_IMPLEMETION(RpcT2H_RegisterGateToHall, RegisterGateToHallReq, RegisterGateToHallRes)

void RpcT2H_RegisterGateToHall::OnReply(const RegisterGateToHallReq &roArg, const RegisterGateToHallRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == CBR::ERR_SUCCESS)
	{
		LogInfo("Gate server serverid [%u] register to hallserver success", roArg.serverid());
		HallLink::Instance()->SetRegistered(true);
	}
	else
	{
		LogError("Gate server serverid [%u] register to hallserver failed", roArg.serverid());
	}

}

void RpcT2H_RegisterGateToHall::OnTimeout(const RegisterGateToHallReq &roArg, const CUserData &roUserData)
{
		LogError("Gate server serverid [%u] register to hallserver timeout", roArg.serverid());
}
