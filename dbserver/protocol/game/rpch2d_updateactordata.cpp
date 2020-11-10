#include "pch.h"
#include "game/rpch2d_updateactordata.h"
#include "mysql/task/actorupdatetask.h"
#include "mysql/mysqlmgr.h"
#include "cache/memorysyncdb.h"
// generate by ProtoGen at date: 2020/9/9 15:09:21


RPC_SERVER_IMPLEMETION(RpcH2D_UpdateActorData, UpdateActorDataArg, UpdateActorDataRes)

void RpcH2D_UpdateActorData::OnCall(const UpdateActorDataArg &roArg, UpdateActorDataRes &roRes)
{
	MemorySyncDb::Instance()->SyncData2Redis((CBR::ActorBase)roArg.act());
}

void RpcH2D_UpdateActorData::OnDelayReplyRpc(const UpdateActorDataArg &roArg, UpdateActorDataRes &roRes, const CUserData &roUserData)
{
	CBR::ErrorCode nResult = (CBR::ErrorCode)roUserData.m_dwUserData;
	roRes.set_error(nResult);
}
