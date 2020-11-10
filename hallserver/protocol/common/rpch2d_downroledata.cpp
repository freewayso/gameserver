#include "pch.h"
#include "common/rpch2d_downroledata.h"

// generate by ProtoGen at date: 2020/8/6 14:19:50

RPC_CLIENT_IMPLEMETION(RpcH2D_DownRoleData, DownRoleDataReq, DownRoleDataRes)

void RpcH2D_DownRoleData::OnReply(const DownRoleDataReq &roArg, const DownRoleDataRes &roRes, const CUserData &roUserData)
{
		LogDebug("RpcH2D_DownRoleData....[%s] [%llu] [%u] [%u]",roRes.name().c_str(),roRes.roleid(),roRes.score(),roRes.killer());
}

void RpcH2D_DownRoleData::OnTimeout(const DownRoleDataReq &roArg, const CUserData &roUserData)
{
}
