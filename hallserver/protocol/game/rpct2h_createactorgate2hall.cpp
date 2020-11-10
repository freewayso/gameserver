#include "pch.h"
#include "game/rpct2h_createactorgate2hall.h"
#include "game/rpch2d_createactorhall2db.h"
#include "network/dblink.h"
#include "util/XCommon.h"

// generate by ProtoGen at date: 2020/9/8 10:36:32

RPC_SERVER_IMPLEMETION(RpcT2H_CreateActorGate2Hall, CreateActorReq, CreateActorRes)

void RpcT2H_CreateActorGate2Hall::OnCall(const CreateActorReq &roArg, CreateActorRes &roRes)
{
	//判断名字是否合法
	CBR::ErrorCode dwErrCode = (CBR::ErrorCode)XCommon::ValidNameCheck(roArg.name());
	if (dwErrCode == ERR_SUCCESS)
	{
		RpcH2D_CreateActorHall2Db *pRpc = RpcH2D_CreateActorHall2Db::CreateRpc();
		pRpc->m_oArg.CopyFrom(roArg);
		pRpc->m_oArg.set_rpcid(DelayRpc());
		DBLink::Instance()->SendTo(*pRpc);

	}else
	{
		LogError("invalid name=[%s]",roArg.name().c_str());
		roRes.set_result(dwErrCode);
	}
}
   


void RpcT2H_CreateActorGate2Hall::OnDelayReplyRpc(const CreateActorReq &roArg, CreateActorRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == 0 && roUserData.m_pUserPtr)
	{
		CBR::CreateActorRes* pInfo = (CBR::CreateActorRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);
	}
}
