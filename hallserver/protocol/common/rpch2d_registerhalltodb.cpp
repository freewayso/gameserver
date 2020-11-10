#include "pch.h"
#include "common/rpch2d_registerhalltodb.h"
#include "dblink.h"

// generate by ProtoGen at date: 2020/8/5 19:35:39

RPC_CLIENT_IMPLEMETION(RpcH2D_RegisterHallToDb, RegisterHallToDbReq, RegisterHallToDbRes)

void RpcH2D_RegisterHallToDb::OnReply(const RegisterHallToDbReq &roArg, const RegisterHallToDbRes &roRes, const CUserData &roUserData)
{
	if (roRes.ret()==CBR::ERR_SUCCESS)
	{
		LogInfo("dblink remote link sucess!!!");
	}
}

void RpcH2D_RegisterHallToDb::OnTimeout(const RegisterHallToDbReq &roArg, const CUserData &roUserData)
{
}
