#include "pch.h"
#include "db/rpch2d_chickdinnerdata2db.h"
#include "mysql/task/actorinserttask.h"
#include "mysql/mysqlmgr.h"

// generate by ProtoGen at date: 2020/8/7 10:57:52

RPC_SERVER_IMPLEMETION(RpcH2D_ChickDinnerData2DB, ChickDinnerSaveDataArg, ChickDinnerSaveDataRes)

void RpcH2D_ChickDinnerData2DB::OnCall(const ChickDinnerSaveDataArg &roArg, ChickDinnerSaveDataRes &roRes)
{
	switch(m_oArg.type())
	{
	case CBR::DBOper_Insert:
		{
			//ActorInsertTask* task = new ActorInsertTask;
			//task->m_rpcId = DelayRpc();
			//task->m_id = roArg.data().roleid();
			//task->m_data = roArg.data();
			//CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	case CBR::DBOper_Update:
		{
			break;
		}
	default:
		LogError("invalid db type[%u]", roArg.type());
		roRes.set_result(CBR::ERR_FAILED);
	}
}

void RpcH2D_ChickDinnerData2DB::OnDelayReplyRpc(const ChickDinnerSaveDataArg &roArg, ChickDinnerSaveDataRes &roRes, const CUserData &roUserData)
{
	CBR::ErrorCode nResult = (CBR::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
