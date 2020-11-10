#include "pch.h"
#include "common/rpch2d_registerhalltodb.h"
#include "network/halllink.h"
#include "LogInit.h"

// generate by ProtoGen at date: 2020/8/5 19:35:39

RPC_SERVER_IMPLEMETION(RpcH2D_RegisterHallToDb, RegisterHallToDbReq, RegisterHallToDbRes)

void RpcH2D_RegisterHallToDb::OnCall(const RegisterHallToDbReq &roArg, RegisterHallToDbRes &roRes)
{
	//网关注册到大厅服务器
	LogInfo("hall server reg serverid = 【%d】 zoneid=【%d】",roArg.serverid(),roArg.name().c_str());
	UINT32 nErrCode = HallLink::Instance()->AddServerInfo(m_dwConnID, roArg.serverid(), roArg.name());
	roRes.set_ret((CBR::ErrorCode)nErrCode);
}

void RpcH2D_RegisterHallToDb::OnDelayReplyRpc(const RegisterHallToDbReq &roArg, RegisterHallToDbRes &roRes, const CUserData &roUserData)
{
}
