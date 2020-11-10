#include "pch.h"
#include "common/rpch2d_uproledata.h"

// generate by ProtoGen at date: 2020/8/6 11:54:12

RPC_CLIENT_IMPLEMETION(RpcH2D_UpRoleData, UpRoleDataReq, UpRoleDataRes)

void RpcH2D_UpRoleData::OnReply(const UpRoleDataReq &roArg, const UpRoleDataRes &roRes, const CUserData &roUserData)
{
	LogDebug("RpcH2D_UpRoleData");

}

void RpcH2D_UpRoleData::OnTimeout(const UpRoleDataReq &roArg, const CUserData &roUserData)
{

}
