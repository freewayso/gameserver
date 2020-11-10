#include "pch.h"
#include "game/rpch2d_createactorhall2db.h"
#include "mysql/task/actorinserttask.h"
#include "mysql/mysqlmgr.h"
#include "cache/memorysyncdb.h"

// generate by ProtoGen at date: 2020/9/8 14:07:25

RPC_SERVER_IMPLEMETION(RpcH2D_CreateActorHall2Db, CreateActorReq, CreateActorRes)

void RpcH2D_CreateActorHall2Db::OnCall(const CreateActorReq &roArg, CreateActorRes &roRes)
{
	ActorInsertTask *pTask = new ActorInsertTask;
	pTask->m_data.CopyFrom(roArg);
	pTask->m_rpcId = DelayRpc();
	CMysqlMgr::Instance()->PushTask(pTask);

}


void RpcH2D_CreateActorHall2Db::OnDelayReplyRpc(const CreateActorReq &roArg, CreateActorRes &roRes, const CUserData &roUserData)
{
	CreateActorRes& res = *(CreateActorRes*)roUserData.m_pUserPtr;
	roRes.CopyFrom(res);
}
