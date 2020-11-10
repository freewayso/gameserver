#include "pch.h"
#include "db/rpch2d_chickdinnerquerydata.h"
//#include "mysql/task/chickdinnerquerytask.h"
#include "mysql/mysqlmgr.h"

// generate by ProtoGen at date: 2020/8/11 10:50:46

RPC_SERVER_IMPLEMETION(RpcH2D_ChickDinnerQueryData, ChickDinnerQueryDataArg, ChickDinnerQueryDataRes)

void RpcH2D_ChickDinnerQueryData::OnCall(const ChickDinnerQueryDataArg &roArg, ChickDinnerQueryDataRes &roRes)
{
	//CChickDinnerQueryTask* task = new CChickDinnerQueryTask;
	//task->m_dwRpcDelayed = DelayRpc();
	//task->m_dwStoreKey = roArg.roleid();
	//CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_dwStoreKey));
}

void RpcH2D_ChickDinnerQueryData::OnDelayReplyRpc(const ChickDinnerQueryDataArg &roArg, ChickDinnerQueryDataRes &roRes, const CUserData &roUserData)
{
	//ChickDinnerQueryDataRes& res = *(ChickDinnerQueryDataRes*)roUserData.m_pUserPtr;
	//roRes.CopyFrom(res);
}
